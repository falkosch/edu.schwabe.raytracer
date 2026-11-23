# Noise of the distribution ray tracing ("glitter") (done parts)

Carved out of `.backlog/rendering-distribution-noise.md`. Multiple importance sampling, adaptive sampling, rough
transmission and denoising stay open there.

## 1. Measure the noise

- A seed offset parameter (render the same image with other seeds) and a noise metric: the RMS difference between
  two seeds, or against a reference with many rays. Per-pixel variance across the sub-samples is a cheaper indicator.
- Scenes: Cornell box (rough mirror sphere), Showcase (bunny, dragon, floor), Materials (row 1). Fast preview and
  full render. Record noise and render time per approach.

### Resolution

- `RaytraceBaseParameters::seedOffset` (`--seed=<n>`): 0 renders the default pattern byte-identical, other values
  other samples; Whitted ray tracing ignores it.
- `--super-sampling=<n>` replaces `SUPER_SAMPLING_FACTOR`; `--full-render --super-sampling=7` (8 x 8 sub-samples,
  default ray counts) renders the references, whose own noise is far below the renders' (Cornell rough mirror 3.0,
  bunny 1.2, dragon 6.4, floor 3.4, metals 1.3).
- The metric works on the 8-bit display values of the 512 x 512 PNGs, per crop: noise is the square root of the
  per-pixel variance across seeds 0 to 3, averaged over the crop; the shift is the crop's mean brightness against the
  mean of two reference seeds, in percent, and shows bias. Two seeds alone gave unstable numbers where fireflies
  dominate. Crops (x0, y0, x1, y1): Cornell rough mirror sphere (55, 340, 200, 485), Showcase bunny
  (10, 340, 150, 490), dragon (90, 290, 370, 420), floor (150, 440, 250, 512), Materials row 1 (30, 105, 490, 170).
- The baseline's shift of -0.1 to -3 % is most likely no bias but the display transform: AgX compresses the bright
  samples, so noisy averages come out darker (Jensen; not verified separately). Compare shifts against the baseline's.
- The scripts of the metric stay out of the repository (the user's decision); the open file says how to rebuild them.
- A reference with more rays per hit instead (64 glossy and sphere light rays) is biased: see the visibility cutoff in
  the open file.

## 2. Candidates (done parts)

- **Joint stratification** across sub-samples: one sequence per pixel, sub-sample s takes its points s * n to
  (s + 1) * n - 1, or an Owen-scrambled Sobol sequence. Cheap, unbiased, helps every sampled effect.
- **Path regularization**: carry `Raytrace::glossy` along the path (emitters invisible after any glossy bounce), or
  raise the roughness after a glossy bounce (Kaplanyan and Dachsbacher 2013). Removes the glossy -> specular -> light
  glitter at the cost of bias (that light goes missing).
- **Firefly clamping**: cap the radiance of a glossy sample (fixed, or relative to the other samples of the hit).
  Simplest and robust, biased (very bright reflections get darker).

### Resolution

- **Joint stratification**, always on: a pixel's primary rays share one sequence (`Raytrace::strataSeed`, the seed of
  its first sub-sample) and take one block each (`Raytrace::stratum`, `stratifiedIndex` in `shading/sampling.h`), for
  the glossy rays and the sphere light samples of their hits. The block size is the number of samples the hit takes:
  the most samples as the stride would line one or two samples per hit up (test
  `SamplingTest::stratifiedIndicesSpreadTheSubSamplesJointly`). The rays' own seeds, from which the secondary rays
  derive theirs, stay per sub-sample. Fast previews (one sub-sample) render byte-identical. A modest gain where the two
  seed pairs (0, 1 and 2, 3) agree: -7 % noise on the Cornell rough mirror (6.56 / 6.39 -> 6.08 / 5.98), -4 % on the
  metals. No measurable change on the bunny, the floor and the dragon, whose seed pairs differ by up to 12 %: their
  noise is mostly fireflies. Blocks overlap when a pixel's sub-samples take different numbers of rays (two of them
  then reuse a point), which costs variance only.
- **Firefly clamping**: `RaytraceBaseParameters::glossyClamp` scales a glossy ray's spectrum down to the limit, which
  keeps its colour (`spectral::limitPeak`, tests in `SpectrumTest`). The library's default is 0 (off), `raytracerui`'s
  `GLOSSY_CLAMP` is 4 (`--glossy-clamp=<x>`, a Config slider): half the noise for a darkening of 1 to 3.5 % of the
  affected crops in full renders. In the fast previews of all test scenes it changes only isolated firefly pixels
  (Cornell box 87, Showcase 542, Materials 16, Shading 9, none in Forms, Lighting, Dragon, Test Scene 1), the mean
  brightness by at most 0.08 %.
- **Path regularization**: `RaytraceBaseParameters::regularizePaths` (`--regularize-paths`, a Config checkbox) keeps
  `Raytrace::glossy` along the mirror reflections and refractions after a glossy reflection. Off by default: it
  removes most of the Showcase glitter, but also the golden sparkle inside the dragon (its glossy reflections of the
  softbox through its own glass), which the converged reference shows.
- Whitted ray tracing (`--glossy-samples=0 --sphere-light-samples=0`) renders byte-identical with the defaults; with
  `--regularize-paths` it changes too, since the mirror ray of a rough surface carries `Raytrace::glossy` there as
  well. Images stay deterministic, render times don't change (Showcase full render 2.7 s), the Cornell box's ray
  counters are unchanged. With `--glossy-clamp=0` every test scene's fast preview is byte-identical to before.

Full render noise / shift in % (4 seeds each; the preview noise in the last row), measured before the fix of section 3,
which changes the noise by at most 0.13 (last two rows: after it, against references after it):

| Candidate                 | Cornell mirror | Bunny       | Dragon       | Floor       | Metals        |
|---------------------------|----------------|-------------|--------------|-------------|---------------|
| before                    | 6.48 / -3.2    | 2.44 / -0.1 | 10.52 / -1.7 | 5.66 / -0.5 | 3.54 / -0.4   |
| joint stratification      | 6.03 / -2.9    | 2.33 / -0.1 | 10.47 / -1.7 | 5.87 / -0.4 | 3.42 / -0.3   |
| + clamp 8                 | 3.70 / -5.0    | 1.86 / -0.2 | 8.03 / -2.6  | 4.27 / -1.0 | 3.33 / -0.3   |
| + clamp 4 (default)       | 2.64 / -6.6    | 1.58 / -0.2 | 5.64 / -3.4  | 2.81 / -1.5 | 3.23 / -0.3   |
| + clamp 2                 | 1.78 / -8.4    | 1.35 / -0.3 | 3.83 / -4.0  | 1.93 / -1.8 | 3.13 / -0.4   |
| + clamp 1                 | 1.17 / -10.2   | 1.17 / -0.4 | 2.66 / -4.4  | 1.49 / -1.9 | 3.04 / -0.4   |
| + regularization          | 6.03 / -2.9    | 1.19 / -0.4 | 2.42 / -4.8  | 2.16 / -2.0 | 3.42 / -0.3   |
| + regularization, clamp 4 | 2.64 / -6.6    | 1.11 / -0.4 | 2.04 / -4.8  | 1.59 / -2.1 | 3.23 / -0.3   |
| preview: before / default | 7.91 / 6.26    | 4.07 / 3.83 | 10.60 / 8.04 | 7.27 / 5.48 | 10.65 / 10.62 |
| section 3, no clamp       | 5.98 / -2.8    | 2.37 / -0.1 | 10.31 / -1.6 | 5.84 / -0.4 | 3.41 / -0.3   |
| section 3, clamp 4        | 2.61 / -6.4    | 1.57 / -0.2 | 5.51 / -3.3  | 2.79 / -1.4 | 3.23 / -0.3   |

The Cornell box's glitter comes from its point light's ceiling hotspot, not from a sphere light, so regularization
doesn't touch it; the clamp darkens the mirror's reflection of that hotspot. The Materials row has no bright spots to
reflect: its noise is the plain variance of few glossy rays per pixel.

## 3. The visibility cutoff biases glossy reflections

Found while rendering references: more glossy rays per hit make the image darker, not just smoother. With 64 glossy
and sphere light rays instead of 16 (full render, 8 x 8 sub-samples), the Showcase bunny gets 21 % darker, the floor
and the Cornell rough mirror 10 %, the dragon 8 %. Each glossy ray's `visibilityIndex` is the reflection's divided by
the number of rays, and both cutoffs read it: `traceReflection` and `traceTransmission` drop rays below
`visibilityCutoff`, and `sampleLighting` skips the lights below `adaptedVisibilityCutoff` (cutoff / visibility, 0.125
for a ray of 1/64). So inter-reflections lose their light, the more the more rays the hit takes; 16 rays most
likely lose some too (not measured).

To do:

- Measure the loss at the default (16 rays) against a render without cutoffs.
- Decide on unbiased termination (Russian roulette: continue with probability visibility / cutoff, weighted by its
  reciprocal), or a cutoff that sees the importance of the whole lobe while the sample count still shrinks with depth.

### Resolution

`Raytrace::importance`: the visibility index without the division among the rays of a glossy reflection, since each of
them estimates the whole reflection. The glossy, mirror and transmitted rays take it from their parent's importance;
the cutoffs read it (`traceReflection` and `traceTransmission` against `visibilityCutoff`, `adaptedVisibilityCutoff`
in `sampleLighting`), the sample budgets keep reading `visibilityIndex`, so that the rays don't multiply with depth.
Whitted ray tracing doesn't split rays and renders byte-identical. Test
`SceneShaderTest::lightCutoffReadsTheImportanceNotTheVisibilityIndex` (fails with the visibility index).
`--visibility-cutoff=<x>` replaces `VISIBILITY_CUTOFF` (1/512) for measurements.

64 instead of 16 glossy rays, full render with 8 x 8 sub-samples and no clamp, brightness shift in %:

| Crop                  | Before the fix | After the fix |
|-----------------------|----------------|---------------|
| Cornell rough mirror  | -9.7           | +0.7          |
| Showcase bunny        | -20.8          | +0.0          |
| Showcase dragon       | -7.9           | +0.5          |
| Showcase floor        | -9.6           | +0.1          |
| Materials metals      | -0.3           | +0.0          |

- At the default 16 rays the image gets brighter where light was lost: the bunny by 2.9 %, the Cornell rough mirror by
  0.9 %, the floor by 0.8 %, the dragon by 0.7 %, the metals by 0.05 %. In the fast previews of all test scenes the
  mean brightness rises by 0.45 % in the Showcase and by at most 0.05 % elsewhere (Lighting unchanged).
- What the cutoff 1/512 still loses (against 1/2048 and 1/8192): at most 0.6 % (dragon), 0.5 % on the floor, 0.1 % on
  the bunny, nothing in the Cornell box and on the metals. Russian roulette isn't needed.
- Cost, from the deterministic ray counters of the fast preview: Showcase 18 % more secondary and 7.5 % more shadow
  rays (full render 2.7 -> 3.1 s), Materials 17 % more secondary rays, Cornell box 5 % more shadow rays (the
  regression block in the root `CLAUDE.md` is updated).
- The firefly clamp's darkening didn't change (clamp 4 against no clamp: Cornell mirror -3.6 %, dragon -1.7 %, floor
  -1.0 %), so its default stays 4.

## 4. Sampling hashes in SIMD lanes (follow-up)

Not a section of the item; done in the same session on the sampling code. The lowbias32 hash of `shading/sampling.cpp`
is one template for `UInt_32` and for every lane of `UInt4` and `UInt8_32` (a new API alias for `v_ui32_8`), so that
the lanes hash like the scalar. `deriveSeeds(seed, first)` derives 8 seeds at once (`traceGlossyReflection` takes its
reflected rays' seeds 8 at a time); `sequenceOffset2D(seed)` hashes the sequence shift's two components together, once
per glossy hit and per sphere light instead of once per sample (`sequencePoint2D(offset, index)`). The hash uses the
vectorization library's operators, which carry their AVX2, AVX and SSE 4.2 versions; no new intrinsics.

- Images byte-identical (Cornell box, Showcase, Materials, Lighting; preview and full render). Tests
  `SamplingTest::deriveSeedKeepsItsValues` and `sequenceOffsetIsTheScalarHashOfTheSeed` pin values of the scalar
  implementation, `deriveSeedsHashesEachLaneLikeDeriveSeed` compares the lanes.
- Micro-benchmark (release, temporary test): 16 sequence points 17 ns instead of 35 to 40 ns; 8 seeds 1.4 ns instead of
  2.2 ns, where the benchmark's scalar loop vectorizes by itself, unlike the renderer's scattered calls. Render times
  don't change measurably: hashing is far below 1 % of a render.
- Rejected: CRC32 (`_mm_crc32_u32`) is linear over GF(2), so neighbouring pixels would get correlated sample patterns;
  SHA-NI isn't part of the SSE 4.2, AVX and AVX2 levels and is slower for a 32-bit mix.

## 5. The Cornell hotspot (candidate)

- **The Cornell hotspot**: a minimum distance in the light attenuation (or giving the light a radius) bounds the
  irradiance next to a point light; that removes the hotspot itself rather than its noise.

### Resolution

The Cornell box's point light 0.02 below the ceiling became a box light set into the ceiling
(`DONE/rendering-cornell-box-area-light.md`): the hotspot and the rough mirror sphere's glitter it caused are gone,
without a minimum distance in the attenuation.

## Commits

- `1fd428a` feat(raytracing): reduce the noise of the distribution ray tracing
- `c07d001` feat(raytracing): add box lights and light the Cornell box with one
