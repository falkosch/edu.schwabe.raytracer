# Noise of the distribution ray tracing ("glitter")

## Motivation

Glossy reflections and sphere lights (`DONE/rendering-test-scene-findings.md`) trace a few rays per hit: up to
`glossySamples` VNDF-sampled reflection rays and up to `sphereLightSamples` shadow rays, fewer the less they add to
the pixel. Where few of those rays reach a small, very bright region, single rays dominate their pixel and the image
glitters:

- Cornell box: the rough mirror sphere (roughness 0.45, about 14 rays per hit) reflects the ceiling's hotspot. The
  light hangs 0.02 below the white ceiling with the falloff 1/(0.13 d + d^2), so the ceiling right above it gets about
  330 times the irradiance of the rest. With the light moved down to y = 0.7 the glitter disappears.
- Showcase: the gold bunny, the glass dragon and the floor. With `--glossy-samples=0` the floor and the bunny are
  smooth and the dragon keeps its sparkles; `--sphere-light-samples=0` changes little.

The sparkle of the glass dragon is a real optical effect (sharp refraction of a small, bright light) and nice to keep.
The goal is less noise for the same number of rays, or the same noise for fewer rays, not a dull image.

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
4. No joint stratification: every sub-sample shifts the R2 sequence by its own random offset, so the 9 sub-samples of
   a full render don't fill one evenly spread set of directions.

## Scope

### 1. Measure the noise

- A seed offset parameter (render the same image with other seeds) and a noise metric: the RMS difference between
  two seeds, or against a reference with many rays. Per-pixel variance across the sub-samples is a cheaper indicator.
- Scenes: Cornell box (rough mirror sphere), Showcase (bunny, dragon, floor), Materials (row 1). Fast preview and
  full render. Record noise and render time per approach.

### 2. Candidates, to evaluate with the metric

- **Multiple importance sampling** (Veach 1997) for sphere lights: combine light sampling (points on the visible cap,
  as the soft shadows already pick them) and BRDF sampling (glossy rays that hit the emissive sphere) by the power
  heuristic. Replaces the current split (rough surfaces: highlight only, emitter hidden from glossy rays) by an
  unbiased combination; the standard answer to glossy surfaces under area lights.
- **Joint stratification** across sub-samples: one sequence per pixel, sub-sample s takes its points s * n to
  (s + 1) * n - 1, or an Owen-scrambled Sobol sequence. Cheap, unbiased, helps every sampled effect.
- **Path regularization**: carry `Raytrace::glossy` along the path (emitters invisible after any glossy bounce), or
  raise the roughness after a glossy bounce (Kaplanyan and Dachsbacher 2013). Removes the glossy -> specular -> light
  glitter at the cost of bias (that light goes missing).
- **Firefly clamping**: cap the radiance of a glossy sample (fixed, or relative to the other samples of the hit).
  Simplest and robust, biased (very bright reflections get darker).
- **Adaptive sampling**: estimate each pixel's variance from its sub-samples and trace more sub-samples where it's
  high (also the "adaptive sampling at high-contrast edges" left open by 5.7 of the findings item).
- **Rough transmission** (GGX BTDF, Walter et al. 2007) for dielectrics with roughness: blurs the dragon's sparkles
  physically, but needs more rays itself.
- **Denoising** of full renders: an edge-aware filter guided by depth, normals and albedo (a-trous wavelet filter), or
  Intel Open Image Denoise as a dependency.
- **The Cornell hotspot**: a minimum distance in the light attenuation (or giving the light a radius) bounds the
  irradiance next to a point light; that removes the hotspot itself rather than its noise.

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
