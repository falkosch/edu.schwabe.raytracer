# Defects and limitations found with the test scenes

## Motivation

The test scenes added to `raytracerui` on 2026-11-22 (Forms, Lighting, Shading, Materials & Noise, Meshes, Showcase;
see `sources/raytracerui/CLAUDE.md`) exercise every form, shader, emitter kind, refraction index, noise generator and
mesh. Rendering them exposed the defects and limitations below. Each section names the scene that shows it: render
it with `raytracerui.exe --scene=<id> --exit-after-render` from `data/` (root `CLAUDE.md`, "Rendering a scene") before
and after the change.

## 1. Debug assert in `KDTreeBalancer::calculateSplitCost` fires

`sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp:291`:

```cpp
const Float4 testingLeavesCost = intersectionCostFactor * SAleftAndRight * TleftAndRight;
assert(allTrue(xyxy(testingLeavesCost) > Zero<Float4>()));
```

- Fires in debug builds of every scene with the `teapot`, `bunny_high` or `dragon2` mesh (`cornell-box`, `shading`,
  `meshes`, `showcase`), while the mesh KD-trees are built. Scenes with only `sphere_high` and `cone` pass (`forms`),
  as do `lighting` and `materials`.
- Most likely older than the vectorization unification: the release Cornell box renders byte-identical to the commit
  before it, kd-tree counters included. Not yet confirmed with a debug build of that commit.
- Hypothesis: `TleftAndRight` can't be zero (the sums start at `One<Float>()`), so a child's `surfaceArea` is zero.
  `surfaceArea` (`primitives/src/primitives/AxisAlignedBoundingBox.cpp:40`) is `2 * (zy + zx + yx)`, zero only for an
  empty, point or line bounding. A sweep candidate with an empty side, or a side holding only degenerate facets
  (collinear or coincident vertices), would give exactly that.

To do:

- Find the failing candidates: log the split position, both child boundings and facet counts when the cost is zero.
- Decide what the assert is for. If zero-area children are legitimate SAH candidates (an empty side costs only the
  traversal term), the assert is too strict: relax it to `>= 0` or drop it, and add a balancer test with an empty side
  and with a degenerate facet. If they shouldn't occur (degenerate facets should be filtered when the mesh loads, or
  an empty side should be skipped before costing), fix that and keep the assert.
- Check whether any of the `data/meshes` files have degenerate facets (zero-area triangles) and how the loader and
  `computeNormals` treat them.

### Resolution

Cause: the SAH sweep counts the facets by their bounding boxes, `sort()` tests their exact overlap with each side.
Facets that cross a side's bounding only outside the parent bounding leave that side empty after the sort; its
bounding clamps to a point at `FLT_MAX` (zero area). Release builds then grew an empty leaf (in debug, the next level
also fails `assert(!parentGeometry.empty())`). Reproduced with 2000 random sliver triangles.

- `KDTreeBalancer::build()` continues with the non-empty side's tighter bounding, one level deeper, instead of growing
  an empty child. Test `BalancerTest::sahSplitWithEmptySideGrowsNoEmptyLeaf`. Images unchanged, `nodesVisited` a
  little lower.
- The assert is `>= 0`: a non-empty side can have a zero area too, when its bounding is clamped to geometry that meets
  it only along a line.
- Behind it, debug runs of `meshes` failed `normalize3` of a zero vector: `chair`, `pistol`, `tank` and `tire` have
  facets without area (8, 3, 2 and 2 after scaling). In release their NaN normals spread to the neighbouring vertex
  normals. `removeDegenerateFacets` drops them on load (sine of the angle below 1e-6, logged as a warning), and
  `computeNormals` keeps a zero vertex-normal sum zero (the smooth normal falls back to the flat one). Test
  `MeshTest::loadingRemovesFacetsWithoutArea`.

## 2. `ConcatenatingShader` doesn't compile

`sources/raytracing/include/raytracing/shading/shaders/ConcatenatingShader.h:22` declares
`const OutputType sample(...) const override`, but `Shader::sample` returns `OutputType`: MSVC rejects the override
with C2555 as soon as the template is instantiated. No scene used it before.

To do:

- Return `OutputType` (drop the `const`).
- Add a test that combines two shaders (e.g. the product of two `ConstShader`s).
- Use it in the Shading scene: replace the plain marble box (`marble`) with "checker times marble" and remove the
  comment in `ShadingScene.cpp` that explains why it's missing.

### Resolution

- `sample()` returns `OutputType`. The getters of `ConstShader` and `MultiplyByConstShader` returned non-const
  references from const methods, which didn't compile once used either: now const references.
- Tests `ConcatenatingShaderTest` (product, operand order, the getters).
- The Shading scene's third front box is "checker times marble" (`std::multiplies<Float4>`).

## 3. `SimplexNoiseGenerator` returns almost only zeros

Probed values for six points: Simplex at most +-0.08 (mostly about +-0.001), Perlin up to +-0.65. The Simplex spheres
in the Materials scene (row 4) therefore show a constant mid-grey.

Cause (`sources/raytracing/src/raytracing/common/noise/SimplexNoiseGenerator.cpp`):

- `K()` computes `t = 0.5 - dot(xyz, xyz)` with the 4-component `dot`.
- `noise3()` skews and unskews whole `Float4`s, so the w lane of `uvw` (and of `xyz`) holds
  `(i + j + k) / 6 - floor(s)`, a leftover in `[0, 1)`. Its square makes `t` negative for most points, and those
  kernels return 0. The w lane of the input (1 for texture coordinates) doesn't matter: w = 0 gives the same values.

Compared with Ken Perlin's reference simplex noise ("Improving Noise", hardware implementation):

- the reference uses only x, y, z: `t = 0.6 - x*x - y*y - z*z` (0.6, not 0.5);
- it hashes with `A` before incrementing it (`shuffle(i + A[0], ...)`, then `A[a]++`), while `K()` increments first.

To do:

- Use `dot3` (or zero the w lane), and check the constant and the hash order against the reference.
- Add tests: output range, mean near zero, non-trivial variance over a grid, determinism for a seed, and a few values
  pinned against the reference implementation.
- Check the Materials scene: the three Simplex spheres must show noise like the Perlin ones.
- Check `PerlinNoiseGenerator` for the same full-`Float4` pattern; its output looked plausible.

### Resolution

- `K()` uses `dot3`, the reference's 0.6, and hashes the corner before advancing `A`. `noise3()` sequences the four
  `K()` calls: the operand order of one `+` expression is unspecified.
- 0.6 lets the kernels reach slightly beyond their simplex. A port of the reference measured steps of up to 1.2e-3 at
  the faces, against an output range of about +-0.3; 0.5 would be continuous, but only reach +-0.13.
- Tests `SimplexNoiseGeneratorTest`: values pinned against the reference (its bit-pattern table injected by a test
  subclass), range, mean, variance, continuity, determinism per seed.
- Perlin: `setup()` truncated the cell index while `fract` floors, so (-1, 0) repeated (0, 1); it floors now.
  `noise4()` wasn't 4D (the 3D lattice plus the gradients' w as an offset); it interpolates 16 corners now.
  `NoiseGeneratorMap` samples `noise3()`, since the w of texture coordinates is the homogeneous 1. Tests
  `PerlinNoiseGeneratorTest`. Every Perlin texture changes, the Cornell box's noise box included.
- The Materials scene's Simplex spheres show noise.

## 4. Environment maps: the shader fits only fisheye domes

`EnvironmentShader::sample` (`sources/raytracing/src/raytracing/shading/shaders/EnvironmentShader.cpp`) maps the
direction's x and z to the image (`0.5 + 0.5 * xz`): an upward fisheye with the lower hemisphere mirrored. The
textures in `data/textures` use other projections:

- mirror-ball light probes: `env_spherical_stpeters`, `env_spherical_uffizium`, `env_spherical_forest`,
  `env_spherical_cameraman`, `env_spherical_supernova`, `env_spherical_phong_gradient`, `env_spherical_gradient`
  (check the last two);
- `env_latitude_checker`: likely latitude-longitude;
- `env_eso_bright_night_at_paranal`: all-sky fisheye, the only one that fits. The Shading scene uses it, but with the
  start camera looking at the horizon it shows the stretched rim (observatory ground), not the sky;
- `distorted_environment`: unknown, check.

To do:

- Add background shaders per projection: mirror ball (angular map: direction to the ball normal), latitude-longitude
  (equirectangular), and keep the fisheye dome. Give each a clear name and document the expected image layout.
- Determine each texture's projection; render a test background per projection (a latitude checker makes errors
  obvious) and add tests for the direction-to-texture mapping at the poles, the horizon and the seams.
- Use a fitting probe in the Shading scene (background and mirror sphere) and consider one for the Showcase.

### Resolution

- `EnvironmentShader` takes an `EnvironmentProjection`: `FisheyeDome` (the former mapping, default), `MirrorBall`,
  `AngularMap`, `LatitudeLongitude`, mapped in `environment_coordinates.h`. The image center faces the start
  camera's view direction -z.
- The textures, classified by rendering each one (listed in `sources/raytracing/CLAUDE.md`): mirror balls
  `cameraman`, `forest` (photos of a chrome ball) and the radial `supernova`, `gradient`, `phong_gradient`; angular
  maps `stpeters`, `uffizium` (Debevec's probes: zenith and nadir at half the radius, a mirror ball has them at 0.71);
  latitude-longitude `env_latitude_checker`; fisheye dome `env_eso_bright_night_at_paranal` and
  `distorted_environment`, which was made for that mapping (a room corner comes out straight).
- Tests `EnvironmentCoordinatesTest`: poles, horizon, seams, rims.
- The Shading scene shows St. Peter's as angular map, in the background and the mirror sphere. The Showcase keeps its
  black studio background.

## 5. Limitations: investigate, then fix or extend

1. **Rough reflections stay sharp.** `Raytracer::traceReflection` traces only the mirrored direction; roughness only
   widens the GGX highlights of point lights (Materials row 1, Cornell box "rough mirror sphere"). `brdf::sampleGGX`
   (`shading/brdf/ggx.cpp`) exists but is never called, although the root `CLAUDE.md` describes "GGX microfacet BRDF
   with VNDF importance sampling". Decide on glossy reflections (VNDF-sampled reflection rays, with several samples per
   pixel or progressive accumulation), or correct the description.

### Resolution of 5.1

Up to 16 reflection rays (`RaytraceBaseParameters::glossySamples`, `--glossy-samples=<n>`, a Config slider; 0 is a
mirror ray as before) for surfaces rougher than `brdf::MirrorRoughness` (0.02), sampled by `brdf::sampleGGX` with
the alpha of the highlights (the roughness channel is the alpha), weighted by Smith's G1, directions below the
geometric normal count as black; fewer rays the less the reflection adds to the pixel. The sampling is deterministic:
`Raytrace::seed` comes from the pixel and sub-sample (`shading/sampling.h`: lowbias32 hash, R2 sequence). Tests
`SamplingTest`. The root `CLAUDE.md`'s "VNDF importance sampling" is accurate now.

2. **Shadows are binary.** `SceneShader::computeLitAreaFraction` returns 0 for any occluder, so glass casts black
   shadows (Materials, Showcase). Attenuate shadow rays through transmissive objects (transmittance and Fresnel, at
   least as an approximation) for coloured glass shadows.

### Resolution of 5.2

`SceneShader::transmitShadowRay`: when the any-hit test finds a transmissive occluder, the shadow ray follows the
nearest hits. Each object passes its Fresnel transmission at entry and exit (Schlick) and absorbs along the straight
chord (the exit comes from the same object with flipped culling); an opaque object blocks, and only opaque ones enter
the shadow cache. Without refraction there are no caustics. The lit fraction is per wavelength, so coloured glass
casts coloured light. Tests `SceneShaderTest`.

3. **Emissive objects don't light the scene.** Emittance only colours the object itself (Lighting: the orange
   emitter). There are no area lights; `illumination-non-thermal-emitters.md` lists light geometry as out of scope.
   Decide whether to support emissive geometry as light sources (light sampling, soft shadows) or document it.

### Resolution of 5.3

Sphere lights: `LightInfo::radius` > 0 makes a light a sphere, whose shadow rays go to up to 16 points
(`sphereLightSamples`, `--sphere-light-samples=<n>`, a Config slider; 0 is one ray, a hard shadow) spread over its cap
visible from the intersection (soft shadows; test `SceneShaderTest::sphereLightCastsSoftShadows`).
`addSphereLight` in `raytracerui` adds the light with its emissive sphere, marked by `ObjectShader::setLightEmitter`.
So that the light counts once: mirrors reflect the sphere and get no highlight of the light; rough surfaces get the
highlight, its lobe widened by the light's size (Karis 2013), and their glossy reflection rays skip the sphere. This
reverses the out-of-scope note of `illumination-non-thermal-emitters.md` for spheres (pointer added there). The
Lighting scene's orange emitter and the Showcase's softbox are sphere lights.

Other emissive geometry is documented, not supported: only `LightInfo`s light the scene (point and sphere lights).
The emittance of any other object (boxes, planes, meshes, emissive textures) makes it glow where it is seen
(directly, in mirrors, through glass), but it casts no light (`sources/raytracing/CLAUDE.md`, Shading). Box lights,
for the Cornell box's ceiling light, are done in `DONE/rendering-cornell-box-area-light.md`. Emissive meshes and
textures as light sources would need light sampling over arbitrary shapes and stay out of scope, like the light
geometry of `illumination-non-thermal-emitters.md`.

4. **Box texture coordinates are spherical.** `Box.cpp:34` uses `mapSpherical(normalize(orientation))`, so a checker
   swirls on the faces (Forms, Shading) and a flat box can't serve as a textured floor. Consider per-face planar
   (cube) mapping, or offer both.

### Resolution of 5.4

`BoxTextureMapping::PerFace` (default: every face spans the texture, upright as seen from outside) or `Spherical`
(kept for the Cornell box's noise box). The face follows from the offset to the center relative to the half lengths,
which also fixes the normals near the edges of boxes that aren't cubes in model space. Tests in `BoxTest`.

5. **Unused light fields.** `LightInfo::glare` is set (default `Two<Float4>()`, Cornell box) but never read;
   `attenuationFactors.w` is ignored (only x, y, z enter `attenuateDiffuseIntensity`). Implement them (glare: visible
   light sprites or halos?) or remove them.

### Resolution of 5.5

`glare` is removed: it was the Phong-era specular colour of a light, while the GGX highlights use the light's emittance.
`attenuationFactors.w` is documented as unused.

6. **Dark values lift to mid-grey.** AgX maps a radiance of 0.02 to about mid-grey (an RGB 0.02 background, a mirror
   reflecting it), so scenes need very dark backdrops (the Showcase uses an albedo of 0.02 and quadratic falloff).
   Check the AgX input scaling against the reference and consider an exposure control in the UI.

### Resolution of 5.6

Cause: the AgX polynomial already encodes for the display (18% grey maps to about 0.5), and the UI applied
`srgbEncode` on top (0.02 became 0.44). The UI shows AgX's output directly; images are darker and have more contrast.
`--exposure=<stops>` and a Config slider scale the HDR image before AgX. The Showcase is retuned (backdrop and floor
albedos, a softbox).

7. **One sample per pixel aliases.** Distant checker planes and mirror rims sparkle at the default super-sampling 0
   (J/K raise it). Consider a higher default for full-quality renders, or adaptive sampling at high-contrast edges.

### Resolution of 5.7

Full renders (Enter, "Full Render") use at least `FULL_RENDER_SUPER_SAMPLING_FACTOR` (2, i.e. 3x3, in release).
`--full-render` makes the first image one: `Raytracer::trigger` returns the run id, so the previews before it are
skipped. Adaptive sampling isn't needed for now.

## 6. Tooling

1. **Asserts block unattended runs.** A failed assert in the debug `raytracerui` opens a modal CRT dialog. The test
   DLLs redirect asserts to stderr in `tests/*/src/test_module.cpp`; do the same in `raytracerui` (at least with
   `--exit-after-render`), to stderr and the log.

### Resolution of 6.1

With `--exit-after-render`, `raytracerui` reports failed asserts and CRT checks to stderr, and a SIGABRT handler logs
the abort and stops the logger, so `raytracer.log` is written (exit code 3). `--max-trace-depth=<n>` lets debug runs
trace secondary rays.

2. **`clang-release` needs `libomp.dll` on `PATH`.** The executable fails with 0xC0000135 when started outside a
   VS developer shell. Copy the DLL next to the executable after the build, or document it (root `CLAUDE.md` mentions
   it).

### Resolution of 6.2

`copy_openmp_runtime()` in the root `CMakeLists.txt` copies `libomp.dll` next to `raytracerui`, `benchmarks` and the
test DLLs of clang-cl builds; the root `CLAUDE.md` says so.

3. **Outdated regression block.** The Cornell box counters in the root `CLAUDE.md` ("Regression Tests") don't match
   the current scene (9 objects, not 8). Current release values, the same for the commit before the vectorization
   unification: `secondaryRays 51138/12321`, `shadowRays 524940/524711`, `objectShadowRays 310199/278787`,
   `kdTree nodesVisited=1048004 rootOverlapMisses=857161`. Update the block from a fresh log.

### Resolution of 6.3

Updated from a fresh log; the counters now include the glossy reflections and the shadow rays through glass.

## 7. `NoiseGeneratorMap` weights its octaves the wrong way round

`noise = noise * 0.5 + octave` leaves the finest octave the weight 1 and the coarsest 2^-(n-1), where fractal noise
does the opposite; each octave also passes through `sin()`. Found while checking the noise generators (Materials
scene, row 4).

To do:

- Decide whether that is intended (the Materials scene's octave columns show it), else weight the octaves by
  2^-i and add a test.

### Resolution

Not intended: octave i (frequency 2^i) now weighs 2^-i, so the coarsest octave dominates. The weights add up to the
same total as before, so the mapping to [0, 1] (`0.5 * noise + 0.5`, clamped) stays, and one octave renders
byte-identical. The `sin()` of each octave stays (a soft compression of its value, not part of the finding). Tests
`NoiseGeneratorMapTest`, which fail with the old weights.

Images: the Materials scene's 4- and 8-octave spheres show cloudy fractal noise instead of fine grain. The Cornell
box's noise box (8 octaves over spherical coordinates scaled by 0.6) became almost plain, since its fine grain weighs
2^-7; it now takes the noise of the Materials scene's 8-octave Perlin sphere (texture scale 4). A box face spans
about a quarter of the spherical coordinates' u range and half of their v range, so it shows fewer, larger features
than the sphere. The Cornell box's ray counters are unchanged.

## 8. `HDRImage::sampleBilinear` wraps v like u

Rows wrap around: at the poles of latitude-longitude maps (and of sphere textures) the top row blends with the bottom
row. Found while adding `EnvironmentProjection::LatitudeLongitude`.

To do:

- Clamp v for these lookups (or offer a clamping mode) and add a test at the poles.

### Resolution

A clamping mode: `sampleBilinear(texCoords, TextureWrap)` with `TextureWrap::Repeat` (default, tiled textures) and
`ClampV` (u wraps, v stays between the centers of the bottom and the top row). That also fixes v = 1 exactly (the
zenith of a latitude-longitude map, the top pole of `mapSpherical`), which the modulo took to the bottom row.
`EnvironmentShader` samples `LatitudeLongitude` maps with `ClampV`; `HDRImageShader` takes the mode (default
`Repeat`), and the Shading scene's earth spheres use `ClampV`. Tiled textures (planes, boxes, scaled sphere textures,
meshes) keep `Repeat`. Tests in `HDRImageTest`, `HDRImageShaderTest` and `EnvironmentShaderTest`. Forms, Lighting,
Shading, Showcase and Dragon render byte-identical in the fast preview (no image there shows the poles' change).

Follow-ups in `sampleBilinear`:

- Without branches: `TextureWrap`'s values are bit sets of the clamped axes, which become lane masks for the
  coordinates and the texel indices.
- Texel i's center lies at (i + 0.5) / resolution. It lay at i / resolution, which shifted every texture by half a
  texel and gave the bottom row no band of its own under `ClampV`. Textured scenes change slightly (checker edges,
  the aliasing of distant floors); the Cornell box and Lighting stay byte-identical.
- `TextureWrap::Clamp` clamps both axes. `EnvironmentShader` uses it for the disc projections (fisheye dome, mirror
  ball, angular map), whose left and right border columns blended at the horizon.
- The index math runs in one `Int4` (columns and rows of the four texels): wrapping by compare and subtract, one
  32-bit multiply, and a final clamp that keeps NaN and huge coordinates within the image. Before, it ran in 64-bit
  lanes with four integer divisions and scalar multiplies. About 8 ns instead of 19 ns per sample (release, a
  1024 x 512 image, random coordinates, a temporary timing test).
- Tests `sampleBilinearClampKeepsBothAxesAtTheEdges`, `sampleBilinearStaysWithinTheImageForAnyCoordinates`; the
  others sample at texel centers now.

Minified textures still alias (no mipmaps): `rendering-texture-minification.md`.

## Commits

- `1a0eb92` feat(raytracing): fix the test scene findings and add distribution ray tracing
