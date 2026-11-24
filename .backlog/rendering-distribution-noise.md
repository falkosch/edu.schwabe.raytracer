# Noise of the distribution ray tracing ("glitter")

## Motivation

Glossy reflections and sphere lights (`DONE/rendering-test-scene-findings.md`) trace a few rays per hit: up to
`glossySamples` VNDF-sampled reflection rays and up to `sphereLightSamples` shadow rays, fewer the less they add to
the pixel. Where few of those rays reach a small, very bright region, single rays dominate their pixel and the image
glitters:

- Cornell box: the rough mirror sphere (roughness 0.45, about 14 rays per hit) reflects the ceiling's hotspot. The
  light hangs 0.02 below the white ceiling with the falloff 1/(0.13 d + d^2), so the ceiling right above it gets about
  330 times the irradiance of the rest. With the light moved down to y = 0.7 the glitter disappears. The box light
  that replaced the point light (`DONE/rendering-cornell-box-area-light.md`) removed the hotspot and this glitter.
- Showcase: the gold bunny, the glass dragon and the floor. With `--glossy-samples=0` the floor and the bunny are
  smooth and the dragon keeps its sparkles; `--sphere-light-samples=0` changes little. Re-checked with the rectangular
  fill softbox (a box light from the upper right since 2026-11-23, `DONE/rendering-cornell-box-area-light.md`):
  `--glossy-samples=0` lowers the full render noise of the bunny from 1.66 to 0.31 and of the floor from 3.05 to 0.35.
  The dragon's sparkles stay but no longer change with the seed (noise 6.70 -> 0.44, RMS error against the reference
  9.9 -> 12.9): they are aliasing (cause 2). `--sphere-light-samples=0`, which now also takes the softbox's shadow
  rays, changes the noise by at most 0.06.

The sparkle of the glass dragon is a real optical effect (sharp refraction of a small, bright light) and nice to keep.
The goal is less noise for the same number of rays, or the same noise for fewer rays, not a dull image.

The measurement (`--seed`, references, metric), joint stratification, the firefly clamp (default 4 in `raytracerui`),
path regularization (an option), the bias of the visibility cutoff, multiple importance sampling, rough transmission
and adaptive sampling (on in `raytracerui`'s full renders) are done: `DONE/rendering-distribution-noise.md`, with the
numbers per candidate.

## Causes

1. Glossy rays that reach small, bright regions: the ceiling hotspot, or the softbox seen through the dragon. The
   reflection rays already follow the BRDF (VNDF importance sampling), but they can't anticipate where the radiance
   is. Paths glossy -> specular (glass, mirror) -> emitter are the worst: `Raytrace::glossy` hides a sphere light's
   emissive sphere only from the glossy rays themselves, not from the specular chain behind them. Light sampling
   can't reach these paths through the specular chain, so MIS doesn't lower their noise; the path regularization
   (done, an option) removes them, with their light.
2. Specular refraction through the dragon's many facets: these Whitted paths are deterministic per sub-sample, so the
   sparkles are aliasing of a chaotic refraction pattern with a light of radiance 14 behind it. The dragon's
   roughness 0.05 blurs its reflections; its refracted lobe, half a degree wide, stays a mirror refraction.
3. Few rays where the reflection is weak: the floor's plastic gets one or two glossy rays (Fresnel at normal
   incidence about 4 %), which at grazing angles hit the dragon's sparkles.

## Current noise

With the defaults (joint stratification, clamp 4, the cutoff fix, MIS, rough transmission, adaptive sampling of 3
rounds at 4/255), full render / preview noise, 8-bit display values, 4 seeds (method in
`DONE/rendering-distribution-noise.md`), measured 2026-11-23: Cornell rough mirror 1.51 / 5.91, teapot 0.70 / 3.19,
Showcase bunny 1.51 / 4.78, dragon 4.23 / 9.42, floor 2.03 / 5.91, Materials metals 2.24 / 10.59. Before MIS, rough
transmission and adaptive sampling: 1.98 / 5.91, 0.92 / 3.19, 1.66 / 4.18, 6.70 / 9.42, 3.05 / 5.65 and
3.23 / 10.59. The fast preview has one sample per pixel, so only the clamp lowers its noise, and adaptive sampling
doesn't apply.

Brightness shift of the full renders against the references: Cornell rough mirror -0.4 %, teapot -0.0 %, bunny
-0.3 %, dragon -3.4 %, floor -1.6 %, metals -0.3 %. The references' own noise (two seeds): Cornell rough mirror 0.68,
teapot 0.33, bunny 1.07, dragon 5.06, floor 3.01, metals 1.25. The teapot's crop is (150, 160, 250, 215).

## Scope

### 1. Candidates, to evaluate with the metric

- **Denoising** of full renders: an edge-aware filter guided by depth, normals and albedo (a-trous wavelet filter), or
  Intel Open Image Denoise as a dependency.

### 2. Measuring the candidates

The open candidates are measured like the done parts (method and numbers in `DONE/rendering-distribution-noise.md`).
The scripts lived in a session's scratchpad and stay out of the repository (the user's decision, 2026-11-22): rebuild
them when a candidate is evaluated.

What they did:

- Render each candidate's options with `--exit-after-render`, preview and `--full-render`, with `--seed=0` to `3`, the
  references with `--full-render --super-sampling=7 --glossy-clamp=0` (seeds 0 and 1).
- Render from a working directory of their own (in the session's scratchpad, never outside the project otherwise)
  that links `data/meshes` and `data/textures` (directory junctions): the user often runs `raytracerui` in `data/`,
  and every run overwrites `ray-traced.png` and `raytracer.log` there.
- Start the app with a visible window: a hidden one (`Start-Process -WindowStyle Hidden`) renders 1 x 1. Show it
  without the focus (`CreateProcess` with `STARTF_USESHOWWINDOW` and `SW_SHOWNOACTIVATE`, which the app's
  `SW_SHOWDEFAULT` takes over): a focused window gets the user's keystrokes, and one reference saved the depth view.
  `--super-sampling=7` applies to the first fast preview too, so a reference renders twice (Showcase about 2
  minutes); a full render's duration is the log's last `Duration` line. Check the logs for extra `Raytrace` runs.
- Read the 8-bit PNGs (512 x 512) and compute per crop: the noise (the per-pixel and per-channel sample variance
  across the seeds, averaged over the crop, then the square root; the mean of the per-pixel roots comes out about half
  as large), the RMS error and the mean brightness shift in % against the references.

### 3. Constraints

- `--glossy-samples=0 --sphere-light-samples=0` (Whitted) stays byte-identical.
- Images stay deterministic per build (integer seeds, `shading/sampling.h`).
- Tests for the estimators (e.g. MIS weights summing to 1, the clamping bound, the stratification of the joint
  sequence) next to `SamplingTest` and `SceneShaderTest`.

## Key files

- `sources/raytracing/src/raytracing/Raytracer.cpp` (`traceGlossyReflection`, `traceTransmission`, `trace`)
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (`computeLitSphereFraction`, highlights)
- `sources/raytracing/src/raytracing/shading/sampling.cpp`, `RaytracerPackets.cpp` (sub-samples)
- `sources/raytracerui/src/tests/ShowcaseScene.cpp`, `CornellBoxScene.cpp`
