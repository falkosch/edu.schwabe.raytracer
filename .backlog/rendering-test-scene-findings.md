# Defects and limitations found with the test scenes

## Motivation

The test scenes added to `raytracerui` on 2026-11-22 (Forms, Lighting, Shading, Materials & Noise, Meshes, Showcase;
see `sources/raytracerui/CLAUDE.md`) exercise every form, shader, emitter kind, refraction index, noise generator and
mesh. Rendering them exposed the defects and limitations below. Each section names the scene that shows it: render
it with `raytracerui.exe --scene=<id> --exit-after-render` from `data/` (root `CLAUDE.md`, "Rendering a scene") before
and after the change.

## Scope

### 1. Debug assert in `KDTreeBalancer::calculateSplitCost` fires

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

### 2. `ConcatenatingShader` doesn't compile

`sources/raytracing/include/raytracing/shading/shaders/ConcatenatingShader.h:22` declares
`const OutputType sample(...) const override`, but `Shader::sample` returns `OutputType`: MSVC rejects the override
with C2555 as soon as the template is instantiated. No scene used it before.

To do:

- Return `OutputType` (drop the `const`).
- Add a test that combines two shaders (e.g. the product of two `ConstShader`s).
- Use it in the Shading scene: replace the plain marble box (`marble`) with "checker times marble" and remove the
  comment in `ShadingScene.cpp` that explains why it's missing.

### 3. `SimplexNoiseGenerator` returns almost only zeros

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

### 4. Environment maps: the shader fits only fisheye domes

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

### 5. Limitations: investigate, then fix or extend

1. **Rough reflections stay sharp.** `Raytracer::traceReflection` traces only the mirrored direction; roughness only
   widens the GGX highlights of point lights (Materials row 1, Cornell box "rough mirror sphere"). `brdf::sampleGGX`
   (`shading/brdf/ggx.cpp`) exists but is never called, although the root `CLAUDE.md` describes "GGX microfacet BRDF
   with VNDF importance sampling". Decide on glossy reflections (VNDF-sampled reflection rays, with several samples per
   pixel or progressive accumulation), or correct the description.
2. **Shadows are binary.** `SceneShader::computeLitAreaFraction` returns 0 for any occluder, so glass casts black
   shadows (Materials, Showcase). Attenuate shadow rays through transmissive objects (transmittance and Fresnel, at
   least as an approximation) for coloured glass shadows.
3. **Emissive objects don't light the scene.** Emittance only colours the object itself (Lighting: the orange
   emitter). There are no area lights; `illumination-non-thermal-emitters.md` lists light geometry as out of scope.
   Decide whether to support emissive geometry as light sources (light sampling, soft shadows) or document it.
4. **Box texture coordinates are spherical.** `Box.cpp:34` uses `mapSpherical(normalize(orientation))`, so a checker
   swirls on the faces (Forms, Shading) and a flat box can't serve as a textured floor. Consider per-face planar
   (cube) mapping, or offer both.
5. **Unused light fields.** `LightInfo::glare` is set (default `Two<Float4>()`, Cornell box) but never read;
   `attenuationFactors.w` is ignored (only x, y, z enter `attenuateDiffuseIntensity`). Implement them (glare: visible
   light sprites or halos?) or remove them.
6. **Dark values lift to mid-grey.** AgX maps a radiance of 0.02 to about mid-grey (an RGB 0.02 background, a mirror
   reflecting it), so scenes need very dark backdrops (the Showcase uses an albedo of 0.02 and quadratic falloff).
   Check the AgX input scaling against the reference and consider an exposure control in the UI.
7. **One sample per pixel aliases.** Distant checker planes and mirror rims sparkle at the default super-sampling 0
   (J/K raise it). Consider a higher default for full-quality renders, or adaptive sampling at high-contrast edges.

### 6. Tooling

1. **Asserts block unattended runs.** A failed assert in the debug `raytracerui` opens a modal CRT dialog. The test
   DLLs redirect asserts to stderr in `tests/*/src/test_module.cpp`; do the same in `raytracerui` (at least with
   `--exit-after-render`), to stderr and the log.
2. **`clang-release` needs `libomp.dll` on `PATH`.** The executable fails with 0xC0000135 when started outside a
   VS developer shell. Copy the DLL next to the executable after the build, or document it (root `CLAUDE.md` mentions
   it).
3. **Outdated regression block.** The Cornell box counters in the root `CLAUDE.md` ("Regression Tests") don't match
   the current scene (9 objects, not 8). Current release values, the same for the commit before the vectorization
   unification: `secondaryRays 51138/12321`, `shadowRays 524940/524711`, `objectShadowRays 310199/278787`,
   `kdTree nodesVisited=1048004 rootOverlapMisses=857161`. Update the block from a fresh log.

## Key files

- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp`
- `sources/raytracing/include/raytracing/shading/shaders/ConcatenatingShader.h`
- `sources/raytracing/src/raytracing/common/noise/SimplexNoiseGenerator.cpp`
- `sources/raytracing/src/raytracing/shading/shaders/EnvironmentShader.cpp`
- `sources/raytracing/src/raytracing/Raytracer.cpp` (reflection, transmission, `applyBRDF`)
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (lights, shadows)
- `sources/raytracing/src/raytracing/geometry/forms/Box.cpp`
- `sources/raytracing/include/raytracing/shading/LightInfo.h`
- `sources/raytracerui/src/main.cpp` (assert redirect), `sources/raytracerui/src/tests/*Scene.cpp`
