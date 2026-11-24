# Scalar code in vectorized hot paths

## Motivation

Much of the per-ray and per-sample code works on `Float4` but drops to scalars on the way: it extracts components
(`x()`, `y()`, `dot3`, `length3`) only to build a vector from them again, chooses vectors by scalar ternaries, and
calls scalar `sqrt`, `cos`, `sin` or `atan2` where the library has vector versions. `brdf::sampleGGXNormal` was such a
case and is rewritten (2026-11-24): the tangent frames as `MFloat4x4` rows (`frame * v` takes a vector into the frame,
`v * frame` back out), `select` instead of the branches, blends and swizzles (`xy_zw`, `yxwz`, `blend<...>`) instead of
scalar construction, the redundant last `normalize3` dropped (the frame is orthonormal). Results within 2e-6 of the
scalar version, images byte-identical where the sampler isn't used (Whitted) and otherwise apart by rounding.

The measurement is the lesson for the rest (temporary micro-benchmark in the test DLL, release, 2^22 calls per round,
the best of 7 rounds over 4 passes in alternating order): the scalar version takes 29.7 to 30.4 ns per call, the vector
version 26.4 to 27.0 ns as a local copy in the benchmark and 27.7 to 28.2 ns called from the library. The gain comes
mostly from the dropped normalization: a first measurement of the vector rewrite alone (the mean of the rounds, one
order) found 34.3 ns against the scalar 33.3 ns. The function is one dependent chain of square roots, divisions and
`sincos`, and the scalar operations already ran in SIMD registers: rewriting a chain gains little, removing
long-latency operations does, and real throughput needs independent work in lanes (several samples at once, as
`deriveSeeds` hashes 8 seeds at once). Later fused multiply-adds (`negativeMultiplyAdd`) and a folded constant
(`Two * Pi`) changed nothing measurable. The style goal stays: no scalar fallbacks where vector operations exist.

## Scope

### 1. Survey

- Search the libraries for:
    - components extracted and put back into a vector (`Float4{x(a), ...}`, `Float4{dot3(...), ...}`)
    - scalar conditions choosing vectors (`cond ? vectorA : vectorB`) where `select` or a blend fits
    - scalar transcendentals and square roots in loops (`vectorization::sqrt`, `cos`, `sin`, `atan2`, `pow` on
      `Float`)
    - loops over independent samples, rays, lights, faces or edges that could fill 4 or 8 lanes (structure of arrays)
- Where: `raytracing` (shading, `Raytracer.cpp`, `RaytracerPackets.cpp`, the forms' intersection tests, the KD-tree
  traversers), `primitives` (`Facet`, `FacetEdges`, `AxisAlignedBoundingBox`, `BoundingSphere`, `SplittingPlane`) and
  `raytracerui`'s per-pixel code (display, tone mapping).
- Rank by how often the code runs: per sample before per ray, per ray before per hit and light, per frame last. Record
  per finding the pattern, the frequency and the kind of gain expected (style only, a shorter chain, throughput in
  lanes).

### 2. Definitive revisits

Found while rewriting `sampleGGXNormal`:

1. **Glossy samples in lanes**: `Raytracer::traceGlossyReflection` takes its VNDF samples one after another, each
   through `sampleGGXNormal`, the masking (`smithG1`), the microfacet's Fresnel and the direction checks; 8 at a time
   in `Float8` lanes would be the throughput gain (the rays themselves stay one by one).
   `traceRoughTransmission` and `traceDiffuseReflection` (the diffuse rays) the same.
2. **Shadow rays' highlights in lanes**: `SceneShader::sampleSphereLight` and `sampleBoxLight` evaluate per shadow ray
   the direction, half vector, cosines, `evaluateGGX_DG`, `glossyRayPdf` and `powerHeuristic` as scalars. The points
   (`SphereLightCap::samplePoint`, `BoxLightFaces::samplePoint`) and the BRDF terms could go 8 at a time, the shadow
   rays one by one.
3. **`SphereLightCap::samplePoint`**: scalar radius, angle, `cos` and `sin`, then `u * (r cos) + v * (r sin)`; a vector
   `sincos` and a frame product as in `sampleGGXNormal`. `computeLitSphereFraction` (the Whitted path) calls it with
   xi = 0: changed rounding there breaks Whitted's byte-identity, so give the cap's center (center - w * radius) its
   own path. `solidAnglePdf` is a `dot3` chain with a scalar result, low priority.
4. **`sampleGGXNormal`'s tangent frame** (Duff et al. 2017, "Building an Orthonormal Basis, Revisited"): branch-free,
   one division instead of `normalize3(cross3(...))`, a shorter chain. It turns the frame about the normal, so default
   renders get other sample patterns (the same distribution): compare the noise, not the bytes.
5. **`BoxLightFaces`**: per hit and box light, `clipAtHorizon` (a `dot3` per corner) and
   `polygonProjectedSolidAngle` (`normalize3`, `cross3`, `length3` and a scalar `atan2` per edge); the 4 edges of a
   face fit 4 lanes per component. `SceneShader::lightPointDensity`'s face matching (`dot3` per face) likewise.
6. **Scalar BRDF chains**: `glossyRayPdf`, `weighedEmitterHighlight` and the point light highlight in
   `SceneShader::sampleLighting` feed scalar BRDF terms from `dot3` chains; with 1. and 2. they move into lanes.
7. **`SubSampleSpread::displayValue`**: the luminance as three scalar products (one `dotv` with a constant) and a scalar
   `pow` per sub-sample (the library has no vector `pow` yet).
8. **`RaytracerPackets::setupRayOfSampleInPixel`**: an integer division and modulo per sub-sample for its grid offset,
   which could be computed once per sub-sample index and frame.
9. **`ATrousDenoiser`'s taps and `IrradianceCache::interpolate`** (`DONE/rendering-distribution-noise.md`, section
   8): per pixel and pass, 25 taps each with a scalar weight (`dot3`, `max3`, divisions and the normals' power) and an
   `exp` of a broadcast `Float4`; 4 taps' exponents in one `Float4` would take one `exp` per 4 taps. The cache's
   error estimates per record likewise, 4 records in lanes. The denoiser takes about 0.1 s of a 512 x 512 preview.

### 3. Constraints

- Only the vectorization library's operations, which carry their SSE 4.2, AVX and AVX2 versions; new operations go into
  the library at both tiers (`sources/vectorization/CLAUDE.md`).
- `--glossy-samples=0 --sphere-light-samples=0` (Whitted) stays byte-identical where the rewritten code is on its path;
  compare the renders of all test scenes, Whitted and default (Cornell box ray counters too).
- Measure each rewrite: a temporary micro-benchmark in the test DLL (removed afterwards) and the render times. A
  rewrite that gains nothing can still stay for the style; note it.
- Benchmark the variants as local copies in one translation unit (a library call adds about 1 ns), take the best of
  several rounds, alternate their order and repeat the run: the variant measured first came out up to 1.5 ns faster,
  more than the differences between the variants. Use every result in a checksum, or the compiler drops the work.
- Conventions: explicit broadcasts (`xxxx`), constant functions (`Zero<Float4>()`), frames as `MFloat4x4` rows.

## Key files

- `sources/raytracing/src/raytracing/shading/brdf/ggx.cpp` (`sampleGGXNormal`, the done example)
- `sources/raytracing/src/raytracing/Raytracer.cpp` (`traceGlossyReflection`, `traceRoughTransmission`)
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp`, `SphereLightCap.cpp`, `BoxLightFaces.cpp`
- `sources/raytracing/src/raytracing/SubSampleSpread.cpp`, `RaytracerPackets.cpp`
- `sources/primitives/src/primitives/`
