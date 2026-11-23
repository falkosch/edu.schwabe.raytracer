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
  smooth and the dragon keeps its sparkles; `--sphere-light-samples=0` changes little. Since 2026-11-23 a rectangular
  fill softbox (a box light, `DONE/rendering-cornell-box-area-light.md`) adds light from the upper right: these findings
  are from before.

The sparkle of the glass dragon is a real optical effect (sharp refraction of a small, bright light) and nice to keep.
The goal is less noise for the same number of rays, or the same noise for fewer rays, not a dull image.

The measurement (`--seed`, references, metric), joint stratification, the firefly clamp (default 4 in `raytracerui`),
path regularization (an option) and the bias of the visibility cutoff are done: `DONE/rendering-distribution-noise.md`,
with the numbers per candidate.

## Causes

1. Glossy rays that reach small, bright regions: the ceiling hotspot, or the softbox seen through the dragon. The
   reflection rays already follow the BRDF (VNDF importance sampling), but they can't anticipate where the radiance
   is. Paths glossy -> specular (glass, mirror) -> emitter are the worst: `Raytrace::glossy` hides a sphere light's
   emissive sphere only from the glossy rays themselves, not from the specular chain behind them.
2. Specular refraction through the dragon's many facets: these Whitted paths are deterministic per sub-sample, so the
   sparkles are aliasing of a chaotic refraction pattern with a light of radiance 14 behind it. The dragon's
   roughness 0.05 blurs its reflections, but not its transmission (rough transmission isn't implemented).
3. Few rays where the reflection is weak: the floor's plastic gets one or two glossy rays (Fresnel at normal
   incidence about 4 %), which at grazing angles hit the dragon's sparkles.

## Current noise

With the defaults (joint stratification, clamp 4, the cutoff fix), full render / preview noise, 8-bit display values,
4 seeds (method in `DONE/rendering-distribution-noise.md`): Cornell rough mirror 2.61 / 6.19 (with the point light:
re-measure with the box light), Showcase (before its rectangular fill softbox: re-measure) bunny
1.57 / 3.89, dragon 5.51 / 7.92, floor 2.79 / 5.44, Materials metals 3.23 / 10.59. The fast preview has one sample
per pixel, so only the clamp lowers its noise.

## Scope

### 1. Candidates, to evaluate with the metric

- **Multiple importance sampling** (Veach 1997) for sphere lights: combine light sampling (points on the visible cap,
  as the soft shadows already pick them) and BRDF sampling (glossy rays that hit the emissive sphere) by the power
  heuristic. Replaces the current split (rough surfaces: highlight only, emitter hidden from glossy rays) by an
  unbiased combination; the standard answer to glossy surfaces under area lights. Box lights take their highlight
  from light sampling already (their shadow rays' points); it gets noisy for narrow lobes under large lights
  (relative standard deviation 0.34 for 16 random rays at roughness 0.05 under a 2 x 2 light 6.7 away), which the
  BRDF samples would cover.
- **Adaptive sampling**: estimate each pixel's variance from its sub-samples and trace more sub-samples where it's
  high (also the "adaptive sampling at high-contrast edges" left open by 5.7 of the findings item).
- **Rough transmission** (GGX BTDF, Walter et al. 2007) for dielectrics with roughness: blurs the dragon's sparkles
  physically, but needs more rays itself.
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
- Read the 8-bit PNGs (512 x 512) and compute per crop: the noise (square root of the per-pixel variance across the
  seeds, averaged), the RMS error and the mean brightness shift in % against the references.

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
