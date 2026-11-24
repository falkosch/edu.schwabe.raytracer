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
4. Diffuse rays (`DONE/rendering-diffuse-interreflection.md`): up to 16 per hit, so at one sample per pixel the
   preview is grainy where the bounce lights a surface (the Cornell box's ceiling). Paths diffuse -> mirror or glass ->
   emitter (caustics, 0.4 % of the Cornell box's mean display value) speckle it; the path regularization removes
   them, with their light.

## Current noise

With the defaults (joint stratification, clamp 4, the cutoff fix, MIS, rough transmission, adaptive sampling of 3
rounds at 4/255), full render / preview noise, 8-bit display values, 4 seeds (method in section 1), measured
2026-11-23: Cornell rough mirror 1.51 / 5.91, teapot 0.70 / 3.19,
Showcase bunny 1.51 / 4.78, dragon 4.23 / 9.42, floor 2.03 / 5.91, Materials metals 2.24 / 10.59. Before MIS, rough
transmission and adaptive sampling: 1.98 / 5.91, 0.92 / 3.19, 1.66 / 4.18, 6.70 / 9.42, 3.05 / 5.65 and
3.23 / 10.59. The fast preview has one sample per pixel, so only the clamp lowers its noise, and adaptive sampling
doesn't apply. After Lambert's 1/pi (`DONE/rendering-physical-material-model.md`: highlights and emitters about 3 times
brighter against diffuse surfaces), measured 2026-11-24: Cornell rough mirror 1.41 / 5.63, teapot 0.87 / 3.52; the
Showcase and Materials numbers predate it.

Brightness shift of the full renders against the references: Cornell rough mirror -0.4 %, teapot -0.0 %, bunny
-0.3 %, dragon -3.4 %, floor -1.6 %, metals -0.3 %. The references' own noise (two seeds): Cornell rough mirror 0.68,
teapot 0.33, bunny 1.07, dragon 5.06, floor 3.01, metals 1.25. The teapot's crop is (150, 160, 250, 215). After
Lambert's 1/pi: Cornell rough mirror -1.0 %, teapot -0.1 %, the references' noise 1.28 and 0.65.

With diffuse interreflection (16 diffuse rays, the Cornell box without its ambient light) and adaptive sampling by the
neighbours' spread at 2/255 (section 7), measured 2026-11-24 against
references without adaptive sampling, noise full render / preview, shift of the full render (of the preview), the
references' noise in brackets:

| Crop                                          | Noise        | Shift             | Reference |
|-----------------------------------------------|--------------|-------------------|-----------|
| Cornell rough mirror                          | 2.13 / 8.73  | -2.5 % (-7.7 %)   | 2.76      |
| Cornell teapot                                | 1.54 / 5.56  | -0.5 % (-2.8 %)   | 1.56      |
| Cornell ceiling (120, 5, 400, 45)             | 2.40 / 12.46 | -2.7 % (-6.6 %)   | 3.92      |
| Cornell back wall (285, 120, 395, 300)        | 1.87 / 5.05  | -0.8 % (-1.6 %)   | 1.94      |
| Cornell tall box's side (128, 230, 142, 430)  | 1.54 / 6.36  | -0.1 % (-7.5 %)   | 1.20      |
| Showcase bunny                                | 2.21 / 6.64  | -0.8 % (-2.7 %)   | 2.31      |
| Showcase dragon                               | 4.23 / 10.83 | -7.9 % (-10.8 %)  | 9.56      |
| Showcase floor                                | 3.42 / 7.60  | -4.3 % (-5.3 %)   | 7.00      |
| Materials metals                              | 1.65 / 10.11 | -0.2 % (-4.3 %)   | 1.65      |

- The clamp makes most of the full renders' shift: without adaptive sampling, clamped against unclamped, Cornell
  rough mirror -2.6 against -0.8 %, ceiling -2.8 against -1.1 % (it limits the diffuse rays too,
  `rendering-physical-material-model.md`, point 9).
- The previews' shift falls with the number of sub-samples, each at hero wavelengths of its own (unclamped, without
  adaptive sampling, ceiling and box side): -6.2 and -7.4 % at 1, -3.7 and -6.6 % at 2 x 2, -1.1 and +0.0 % at 3 x 3.
  Before the diffuse rays the preview was darker too (rough mirror -4.9 %, ceiling -1.8 %).

The defaults since section 8 (many-light sampling, irradiance caching, denoised previews) against these: the last
tables of section 8.

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

## 6. Candidates (done parts, 2026-11-23)

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

### Resolution

All three are on by default; Whitted ray tracing renders byte-identical (every test scene, preview and full render),
images stay deterministic. The measurements re-measured the defaults first (open file, "Current noise"); the metric's
noise is the root of the crop's mean per-pixel variance, which the open file now spells out.

- **Multiple importance sampling**, for sphere and box lights, on rough surfaces with shadow rays and glossy rays:
  `SceneShader::sampleLighting` takes the hit's number of glossy rays (`glossyRaysOf` in `Raytracer.cpp`, the same
  count `traceGlossyReflection` takes) and weighs each shadow ray's highlight by the power heuristic
  (`power_heuristic.h`) against the glossy rays' density (`brdf::pdfGGX`). A glossy ray that hits an area light's
  emitter (`Raytrace::weighsEmitters`, `ObjectShader::setLightEmitter` now takes the light,
  `IlluminatedIntersection::emitterLight`) adds the light's highlight weighed the other way: F * G2 / G1(V) with the
  emitter's radiance, unclamped, against the shadow rays' density (`SceneShader::lightPointDensity`; a box face that
  doesn't emit adds nothing, a sphere below the horizon isn't sampled and gets the whole weight). Both sides estimate
  the same light: the glossy side takes the light's radiance as the shadow rays do, and density 0 wherever they don't
  sample its highlight (horizon, cutoff, an intersection seen from inside); a point off the light (an emitter moved
  away from it) is no light, F0 = 0 has no highlight on either side, and the shadow rays' highlight stays where the
  cutoff drops the diffuse light. Sphere lights got a
  sampled path (`sampleSphereLight`, the cap as `SphereLightCap` with its density per solid angle): their highlight
  is light sampling with the sphere's radiance (its intensity spread over its cross section) instead of Karis's
  widened lobe, which stays for 0 shadow rays. Tests: `PowerHeuristicTest`, `SphereLightCapTest` (the density
  integrates to 1 and matches the sampled points), `GGXTest::pdfGGXIsTheDensityOfSampleGGX`, and
  `SceneShaderTest::sphereLightHighlightIsTheIntegratedBRDF` and `boxLightHighlightWithGlossyRaysIsTheIntegratedBRDF`
  (the shadow rays' part plus the glossy rays' part integrate the BRDF over the light within 3 %, also for a lobe of
  0.05 under a 2 x 2 light), `lightPointDensityOfAPointOffTheLightIsNoLight` and
  `lightPointDensityIsZeroWhereTheShadowRaysDontSampleTheHighlight`.
- **Rough transmission**: a rough dielectric interface whose refracted lobe is wider than a mirror's reflects and
  refracts through VNDF-sampled microfacet normals (`brdf::sampleGGXNormal`), each with its own Fresnel reflectance
  (`brdf::fresnelReflectance(incident, normal, eta)`, Walter et al. 2007): `traceGlossyReflection` weighs a ray by its
  microfacet's reflectance, `traceRoughTransmission` by its transmission, both per the smooth surface's that
  `applyBRDF` multiplies with, the refracted rays also by G1 of their direction and their own absorption. A microfacet
  that reflects totally refracts nothing and reflects all; a direction that doesn't cross the surface is masked and
  counts as black. The refracted lobe is |1 - eta| / 2 as wide as a reflected lobe of the same roughness and takes as
  many rays as one of that roughness. A narrower refracted lobe refracts as a mirror with the smooth surface's Fresnel
  for both lobes (the approximation reflections make up to `MirrorRoughness`), as do the smooth surface's total
  reflection and glossy samples 0. Tests: `GGXTest::sampleGGXReflectsAboutTheSampledNormal` and
  `roughRefractionSpreadsAroundTheMirrorRefraction`, `FresnelReflectanceTest::roughDielectricLobesKeepTheEnergy` (the
  lobes sum to at most 1, a few percent less from the masking) and `totallyReflectingMicrofacetsReflectTheirLight`.
  Three first versions were wrong: with the reflection's ray count, Test Scene 2 (a water sphere of roughness 0.2)
  traced 7 times the secondary rays; with the smooth surface's Fresnel, totally reflecting microfacets counted as
  black, and the Cornell box's glass teapot (roughness 0.05, a refracted lobe of half a degree, one ray per hit) turned
  speckled: noise 3.19 -> 11.45 in the preview, 0.92 -> 1.78 in the full render (crop (150, 160, 250, 215)); taking
  the mirror refraction instead of black hid the speckles without being physical.
- **Adaptive sampling**: `RaytraceBaseParameters::adaptiveRounds` and `adaptiveThreshold` (library default off,
  `raytracerui` 3 rounds and 4/255, `--adaptive-rounds=<n>`, `--adaptive-threshold=<x>`, Config sliders). After its
  sub-samples, a pixel of at least 2 x 2 whose display luminance (`SubSampleSpread`: y / (1 + y), exponent 1 / 2.2)
  has a standard error of the mean above the threshold takes another round of sub-samples, on the grid shifted by
  Roberts' R2 sequence within the cells and at hero wavelengths shifted within their strata (one set of spectral
  contexts per round), continuing the pixel's sample sequence; up to the rounds. Whitted ray tracing doesn't sample
  adaptively, so its full renders stay byte-identical. Fast previews (one sub-sample) don't change. Tests:
  `SubSampleSpreadTest`.

Full render noise / shift in % against the references of the same build, 4 seeds; render times of the full render
(Showcase's vary by up to 2x between runs on this machine). Rough transmission doesn't apply to these crops (their
glass is too smooth). The rows below the default (4/255) were measured with the first rough transmission, which
refracted the dragon rough too (default 4.43 / -3.7 and 1.96 / -1.6 on the floor then):

| Variant                       | Cornell mirror | Bunny       | Dragon       | Floor       | Metals      |
|-------------------------------|----------------|-------------|--------------|-------------|-------------|
| before, Karis highlight       | 1.98 / -0.2    | 1.66 / -5.1 | 6.70 / -4.4  | 3.05 / -1.8 | 3.23 / -0.3 |
| + MIS                         | 1.98 / -0.2    | 1.92 / -0.2 | 6.52 / -3.0  | 3.16 / -1.3 | 3.23 / -0.3 |
| + adaptive 3 rounds, 4/255    | 1.51 / -0.4    | 1.51 / -0.3 | 4.23 / -3.4  | 2.03 / -1.6 | 2.24 / -0.3 |
| + adaptive 3 rounds, 2/255    | 1.30 / -0.4    | 1.24 / -0.4 | 4.30 / -2.9  | 1.63 / -1.7 | 1.82 / -0.2 |
| + adaptive 3 rounds, 1/255    | 1.00 / -0.2    | 1.12 / -0.3 | 4.19 / -2.6  | 1.55 / -1.8 | 1.66 / -0.2 |
| uniform 4 x 4 instead         | 1.42 / +0.1    | 1.47 / -0.2 | 5.77 / -3.0  | 2.47 / -1.4 | 2.41 / -0.3 |
| uniform 5 x 5 instead         | 1.09 / -0.1    | 1.23 / -0.1 | 4.89 / -2.6  | 2.04 / -1.2 | 1.92 / -0.1 |

| Variant                    | Cornell box | Showcase | Materials |
|----------------------------|-------------|----------|-----------|
| 3 x 3                      | 1.1 s       | 5.8 s    | 1.1 s     |
| adaptive 4/255 (default)   | 1.2 s       | 7.6 s    | 1.9 s     |
| adaptive 2/255             | 1.5 s       | 14.0 s   | 3.2 s     |
| adaptive 1/255             | 2.2 s       | 16.9 s   | 3.9 s     |
| uniform 4 x 4              | 2.2 s       | 12.9 s   | 2.4 s     |
| uniform 5 x 5              | 3.2 s       | 18.3 s   | 3.3 s     |

- MIS mostly removes bias: the old widened-lobe highlight of the Showcase's sphere light came out 5 % too dark on the
  gold bunny (RMS error against the MIS references 8.35, now 2.13; the dragon 11.5, now 9.6). The bunny's noise rises
  by 16 %, since the old highlight didn't change with the seed. The Cornell box's small light dominates the weights
  (about 0.997 for the shadow rays): its image changes below 8 bits. Ray counts don't change.
- Rough transmission applies to the rough glass of the older test scenes, whose materials took the roughness for the
  highlights only. Fast preview cost: Materials 0.5 % more secondary rays, Test Scene 1 2.3 times (0.14 -> 0.29 s),
  Test Scene 2 2.9 times (0.13 -> 0.38 s), the Dragon scene 4 times (0.24 -> 0.89 s; its diamond dragon of roughness
  1.0 turns frosted). The Cornell box's and the Showcase's glass refracts as before (lobes of half a degree).
- Adaptive sampling leaves less noise than uniform super-sampling at the same or less time in every crop: 4/255
  against 4 x 4 in the Showcase (7.9 against 12.9 s) 23 % less on the dragon and 21 % less on the floor, 1/255 against
  4 x 4 in the Cornell box (2.2 s both) 30 % less, 4/255 against 4 x 4 in the Materials (2.4 s both) 7 % less. The
  RMS error of the metals stays above uniform sampling's (3.46 at 2/255 against 2.74 at 5 x 5), most likely aliasing
  that more sample positions resolve better. Its shift is 0.1 to 0.5 % darker than uniform sampling's, most likely
  the bias of a stopping rule that depends on the samples themselves.

## 7. Adaptive sampling's bias (done parts, 2026-11-24)

- **Adaptive sampling's bias** on heavy-tailed noise (above): a stopping rule that doesn't judge a pixel by the
  samples it averages (e.g. a separate estimate of the spread), or more rounds before the first decision.

### Resolution

- `AdaptiveRounds`, two passes in `Raytracer::trace`: every pixel takes its first round of sub-samples and keeps the
  standard error of their display luminance (`SubSampleSpread`); then each pixel takes the further rounds its
  neighbours' errors ask for, the root mean square of up to 8 of them: the fewest k for which it divided by
  sqrt(k + 1) is at most the threshold, up to `adaptiveRounds`. The pixel's own sub-samples don't decide, so the
  number of its rounds doesn't depend on them and its mean stays unbiased. All further rounds are decided from the
  first ones: deciding a second round from the neighbours' further rounds would depend on the pixel's own samples
  through its neighbours' decisions.
- `raytracerui`'s threshold 4/255 -> 2/255 (the user's choice, 2026-11-24): a rule that can't single out the pixels
  that caught a rare bright ray barely lowers that noise at 4/255.
- The references of these measurements render with `--adaptive-rounds=0` (the earlier ones sampled adaptively; their
  shifts change by less than 0.1 %). Tests: `AdaptiveRoundsTest`. Previews (one sub-sample) and Whitted renders stay
  byte-identical; full renders get 0.05 to 0.39 % brighter, with no seams between refined and other regions.

Full render noise against references without adaptive sampling (4 seeds; the diffuse rays of
`DONE/rendering-diffuse-interreflection.md` and the clamp on): the Cornell box's ceiling and rough mirror, the
Showcase's floor, bunny and dragon, the Materials' metals; mean render time of the full render (Cornell box /
Showcase / Materials):

| Variant                   | Ceiling | Mirror | Floor | Bunny | Dragon | Metals | Time                 |
|---------------------------|---------|--------|-------|-------|--------|--------|----------------------|
| no adaptive sampling      | 4.01    | 3.15   | 3.90  | 2.80  | 6.61   | 3.11   | 4.2 / 6.5 / 2.9 s    |
| by its own spread, 4/255  | 2.90    | 2.51   | 2.69  | 2.05  | 4.45   | 2.13   | 4.5 / 9.3 / 4.4 s    |
| by the neighbours', 4/255 | 3.95    | 3.07   | 3.88  | 2.68  | 4.93   | 2.03   | 4.6 / 10.3 / 5.0 s   |
| by the neighbours', 2/255 | 2.40    | 2.13   | 3.42  | 2.21  | 4.23   | 1.65   | 8.1 / 14.1 / 6.4 s   |
| by the neighbours', 1/255 | 1.83    | 1.57   | 2.60  | 1.81  | 4.00   | 1.57   | 13.8 / 17.7 / 7.6 s  |

- The bias: judged by its own spread, a pixel came out darker than without adaptive sampling by 0.3 to 0.6 % with the
  clamp (Cornell rough mirror -0.34, ceiling -0.52, Showcase floor -0.48, dragon -0.60, bunny -0.20 percentage
  points) and 1.5 to 1.8 % without it (Cornell rough mirror, ceiling). Judged by its neighbours, the shift equals
  that without adaptive sampling (4/255: within 0.02 points but for the dragon, +0.3, and the metals, +0.2, where
  more samples resolve aliasing), and approaches the references with more rounds.
- The noise: the old rule was efficient on rare bright rays because it refined the pixels that caught one, the same
  selection that biased it. The neighbours' rule refines regions: better on aliasing (metals 2.03 against 2.13 at
  4/255, RMS error 3.30 against 4.05), on rare bright rays only with more rounds (2/255).

## 8. Candidates (done parts, 2026-11-24)

- **Denoising** of full renders: an edge-aware filter guided by depth, normals and albedo (a-trous wavelet filter), or
  Intel Open Image Denoise as a dependency.
- From `DONE/rendering-diffuse-interreflection.md` (cause 4):
    - **More diffuse rays at primary hits only**, where they show directly.
    - **Irradiance caching** (Ward et al. 1988): diffuse rays at sparse points, interpolated between them.
    - **Many-light sampling** at the hits of diffuse rays: the Procedural scene's 343 point lights each take a shadow
      ray at every hit, 7 times its render time (preview 5.5 -> 36 s).

### Resolution

Measured with the method of section 1 (4 seeds; the references with `--full-render --super-sampling=7
--glossy-clamp=0 --adaptive-rounds=0` and neither cache nor denoiser). The library keeps all of them off;
`raytracerui` turns on many-light sampling (16 picks), irradiance caching and the denoising of fast previews, not of
full renders (the user's choice, 2026-11-24). Whitted ray tracing doesn't sample, cache or denoise: its renders stay
byte-identical but for one pixel by one 8-bit step in Test Scene 1 (preview) and Test Scene 2 (full render), rounding
of the restructured light loop (same ray counters). Images stay deterministic, the irradiance cache's too (repeated
runs give the same bytes). Tests: 559, 17 of them new.

- **Many-light sampling**: `RaytraceBaseParameters::manyLightSamples` (`--many-light-samples=<n>`,
  `MANY_LIGHT_SAMPLES` 16, a Config slider). At the hits of diffuse rays and further along their paths,
  `SceneShader::sampleLighting` (its new `pointLightSamples`) picks that many of the point lights (and sphere lights
  without shadow rays) where there are more: systematic sampling of their unshadowed diffuse light, one random offset
  per hit from the ray's seed, each picked light weighed by its picks per their expected number. Unbiased; a light
  whose share exceeds 1 / picks is always picked, and without shadows the estimate of the diffuse light is exact (the
  picks follow it), only highlights and shadows are sampled. Only the Procedural scene has more than 16 point lights;
  every other scene renders as before (Test Scene 2: 5 pixels by one step, rounding). Tests:
  `SceneShaderTest::pointLightPicksAverageToEveryLightsLighting` (4 picks among 25 lights, 4096 seeds: within 1 % for
  the diffuse light, 2 % for the highlights) and `pointLightPicksTakeEveryLightWhereThereAreNoMore`.

  Procedural preview, noise and shift against the mean of the 4 previews with every light (its own noise in the last
  column), the image and a crop (the sphere at (370, 370, 480, 480)):

  | Picks              | 4            | 8            | 16 (default) | 32           | every light |
  |--------------------|--------------|--------------|--------------|--------------|-------------|
  | Render time        | 8.0 s        | 9.1 s        | 11.0 s       | 15.8 s       | 39.3 s      |
  | Image noise, shift | 7.23, -1.0 % | 5.75, -0.5 % | 4.81, -0.2 % | 4.17, -0.1 % | 3.62        |
  | Sphere noise       | 11.13        | 8.79         | 7.36         | 6.32         | 5.51        |

  The shift halves with each doubling of the picks, as the added variance does: noisier images come out darker
  through the display transform (section 1), the estimator isn't biased. 16 picks take the least noise^2 times time
  (254 against 515 with every light, 413 with 4 picks, 301 with 8, 275 with 32). With irradiance caching and
  denoising (the defaults), the preview takes 12.2 s and the full render 120 s.

- **More diffuse rays at primary hits only**: rejected. Measured with a temporary switch (primary hits up to 32 or 64
  rays, the others up to 16) against raising `--diffuse-samples` everywhere, noise preview / full render, render time
  of the Cornell box:

  | Diffuse rays     | Time          | Ceiling      | Back wall   | Box side    | Rough mirror | Showcase floor |
  |------------------|---------------|--------------|-------------|-------------|--------------|----------------|
  | 16 (default)     | 0.50 / 8.1 s  | 12.46 / 2.40 | 5.05 / 1.87 | 6.36 / 1.54 | 8.73 / 2.13  | 7.60 / 3.42    |
  | 32, primary hits | 0.77 / 10.0 s | 8.67 / 2.28  | 3.86 / 1.40 | 5.28 / 1.42 | 7.99 / 2.06  | 7.49 / 3.37    |
  | 32 everywhere    | 0.80 / 10.1 s | 8.67 / 2.28  | 3.86 / 1.40 | 5.03 / 1.34 | 7.68 / 1.93  | 7.49 / 3.36    |
  | 64, primary hits | 1.35 / 14.9 s | 6.01 / 1.83  | 2.80 / 0.97 | 4.57 / 1.24 | 7.56 / 1.97  | 7.44 / 3.35    |
  | 64 everywhere    | 1.39 / 15.7 s | 6.01 / 1.83  | 2.80 / 0.97 | 4.13 / 1.07 | 6.98 / 1.71  | 7.44 / 3.35    |

  The primary hits take most of the diffuse rays already: the walls seen directly get the same noise either way at
  nearly the same cost, the glossy reflections (the rough mirror, the box's side) lose. In the Showcase the diffuse
  rays hardly matter (its noise is glossy). More rays everywhere pay off in previews (the ceiling's noise^2 times
  time 78 at 16 rays, 50 at 64) but slow them, where the cache and the denoiser below do more for less:
  `DIFFUSE_SAMPLES` stays 16.

- **Denoising**: `ATrousDenoiser`, the edge-avoiding a-trous wavelet filter (Dammertz et al. 2010) with SVGF's
  edge-stopping weights (Schied et al. 2017), written for the project rather than Intel Open Image Denoise as a
  dependency. `RaytraceBaseParameters::denoise` (`--denoise=<0|1>` and `--full-render-denoise=<0|1>`, `DENOISE` on and
  `FULL_RENDER_DENOISE` off, Config checkboxes). The primary rays leave their guides (`PixelFeatures` through
  `Raytrace::features`): the shading normal, the diffuse albedo and the path length of the first hit that isn't a
  smooth mirror or glass, whose reflected or refracted image shows instead; summed per pixel over the first round's
  sub-samples. 5 passes of the 5 x 5 B3-spline kernel spread by 1 to 16 pixels; a neighbour's weight falls with the
  normals (cos^128), the depth against the depth gradient (at least 0.1 % of the depth), the albedo (0.1) and the
  luminance (compressed as y / (1 + y)) against 4 standard deviations of the noise, filtered along with the colour:
  with one sub-sample the luminance's variance among the 7 x 7 neighbours of the same surface, with more the variance
  of the pixel's mean from its sub-samples (every round's). About 0.1 s per 512 x 512 image. Tests:
  `ATrousDenoiserTest` (the noise fades, the mean stays within 1 %, nothing bleeds across normals, depths or
  albedos, pixels without a surface stay).
    - Previews: the RMS error falls in every crop (table below).
    - Full renders: the first version took the spatial variance there too, which counts reflected detail as noise
      (RMS error of the metals 2.97 -> 5.91, bunny 3.02 -> 4.29, teapot 2.38 -> 3.82); with the sub-samples'
      variance 4.26, 2.74 and 2.28, better than without denoising in 8 of 9 crops but the metals, whose glossy
      reflections blur. Smaller luminance sigmas keep the metals better (2: 3.68, 1: 3.32) and lose elsewhere (the
      ceiling's preview 6.35 -> 7.66 at 2): the sigma stays 4, and full renders aren't denoised by default.
    - Rejected: clamping a pixel to its brightest neighbour of the same surface before filtering. Fewer speckles,
      but the light of the caustics goes missing (previews: box side -7.7 -> -9.2 %, dragon -9.1 -> -11.6 %) and the
      RMS error rises in every crop. The previews' speckles (diffuse -> mirror or glass -> light) stay;
      `--regularize-paths` removes them, with their light.

- **Irradiance caching**: `IrradianceCache` and `IrradianceRecord`, `RaytraceBaseParameters::irradianceCaching`
  (`--irradiance-caching=<0|1>`, `IRRADIANCE_CACHING` on, a Config checkbox), Ward et al. 1988 without gradients.
  Before the pixels, `Raytracer::trace` records the irradiance of 16 times `diffuseSamples` rays (at least 256), each
  limited to 16 times their mean radiance (twice: the mean of the limited rays again), at the primary
  hits of each first-round sub-sample (each has its own wavelengths, so its own records), on grids of 32, 16, 8 and 4
  pixels where the coarser grids' records don't cover the hit fully yet (`RaytracerCache::recording`). A record's
  radius R is the harmonic mean distance of its rays that hit something, a R clamped to 1.5 pixels up to twice its
  grid; it is valid where eps = |x - x_i| / R_i + sqrt(1 - n . n_i) < a = 0.1 and the point doesn't lie behind it,
  and weighs 1 / eps - 1 / a. Each level clamps the radii of all records by their neighbours'
  (R_i <= R_j + |x_i - x_j|, Krivanek et al. 2006). A level's rays see only the coarser levels' records, so the
  records don't depend on the threads; each level has a grid of cells as large as its largest validity radius. Diffuse
  hits of a first-round sub-sample (also behind mirrors) take the records' irradiance where their coverage (the weight
  sum times a) reaches 0.5, a single record within 2/3 of its validity radius; below it they blend it with their own
  rays' by the coverage. The others and the further rounds of adaptive sampling trace their rays. Tests:
  `IrradianceCacheTest` (8: validity by distance, normal and side, weights, coverage, contexts, levels, the
  neighbours' clamp) and `RaytracerTest::irradianceRecordsGiveTheOneBounceOfTheirRays`.
    - The first version (the radius from all rays, no neighbours' clamp, records taken or not) showed hard-edged,
      caustic-like patches, worst at seams between surfaces (found by the user, 2026-11-24), which the RMS error rated
      better than the noise they replaced. Records exactly in a seam (the ceiling meeting the green wall at x = 0.999)
      sent half their rays through the wall's back faces out of the scene: too dark (0.10 to 0.18 against about 0.2
      next to them), and with those rays as infinitely far, radii of 2.5 to 3.6 instead of 0.1 to 0.4, a quarter of
      the room. They covered their surroundings, so the finer levels added no records there. Where records stopped at
      the 4-pixel grid, the hits' own rays filled in: noise next to smooth areas, in staircases along the grids. More
      rays per record (1024) didn't change the patches; the radius from the rays that hit shrank them, the neighbours'
      clamp and the blending removed them. The first version's variants are below the sweep.
    - With 1 diffuse ray, the glossy clamp off and the path regularization on, the previews looked smeared, with
      small soft caustics (found by the user, 2026-11-24): the records took 16 times 1 rays, and a record that caught
      one unclamped bright path (the light's highlight on the rough mirror sphere) spread it as a blotch. Records take
      at least 256 rays since, and limit their rays to 16 times their mean: a path that fewer than 1 in 16 of a
      record's rays take is left to the hits' own rays (bias: the cached irradiance misses that much of the caustics'
      and highlights' light), a path that more take stays whole.
    - Orange dots next to noisy patches in the Lighting scene, on the wall and the sphere (found by the user,
      2026-11-24): a record's cap (twice its grid in pixels) went into the neighbours' clamp, so that a level's records
      shrank the coarser ones around them to the cap plus a tenth of their distance (the upper wall's coarse records
      from about 60 to about 10 pixels), after the level's candidates had taken them as covering. No level filled
      those holes: their hits traced their own 16 rays, which caught the bright floor around the orange sphere light
      (the records limit such rays). The clamp takes the uncapped radii since (`IrradianceRecord::maxRadius`): the
      upper wall is covered (records 4668 -> 4474, the preview's secondary rays -38 %), its noise in full renders
      1.93 -> 0.22, RMS 2.53 -> 2.16 (preview 2.30 -> 2.48: the records' outlier limit darkens it, -2.4 -> -3.0 %);
      the other crops change by at most 0.01. Gaps stay where the geometry asks for finer records than the 4-pixel
      grid (the sphere's lower half on the floor, its rim, the floor along the wall): a finest grid of 2 closes them
      (the sphere's full render 1.23 -> 1.13) for 60 % more time in the Cornell box; half the normal term's weight
      (records valid up to 16 instead of 8 degrees) closes most on the sphere (1.18) at no cost, and the other crops
      change by at most 0.01; taking the records alone from a coverage of 0.02 leaves the dots and worsens the ceiling
      (3.94 -> 4.07). The user chose the normal term's half weight (`NormalWeight` in `IrradianceCache.cpp`; the error
      estimate above has Ward's full one).
    - The finest grid 2 cost more than tracing where detail asks for records everywhere (a record of 256 rays per 4
      pixels against up to 16 rays per pixel): the Meshes scene took 0.51 instead of 0.24 s for the preview, 4.3
      instead of 2.5 s for the full render (first version); grid 4 brought it back to 0.24 / 2.7 s.
    - With denoising on top in full renders, the ceiling got no better (first version: 3.91 against 3.92) and the
      metals blur as above.

  Settings of the fixed version in the Cornell box (with its ambient light again,
  `DONE/rendering-diffuse-interreflection.md`; against references with it), no denoising: accuracy, rays per record,
  the coverage that takes the records alone; records of the preview, mean render time and RMS error preview / full:

  | a, rays, coverage     | Records | Time          | Ceiling      | Back wall   | Box side    | Rough mirror |
  |-----------------------|---------|---------------|--------------|-------------|-------------|--------------|
  | no cache              | -       | 0.53 / 7.5 s  | 13.23 / 3.47 | 5.94 / 2.18 | 9.83 / 1.73 | 10.86 / 3.43 |
  | 0.1, 256, 1           | 10786   | 0.59 / 6.2 s  | 6.93 / 3.21  | 3.83 / 1.57 | 9.02 / 1.49 | 9.60 / 3.32  |
  | 0.1, 128, 1           | 10793   | 0.50 / 5.3 s  | 7.28 / 3.27  | 3.97 / 1.60 | 9.02 / 1.50 | 9.63 / 3.33  |
  | 0.1, 256, 0.5 (now)   | 9795    | 0.55 / 5.5 s  | 6.56 / 3.18  | 3.79 / 1.56 | 8.91 / 1.46 | 9.56 / 3.32  |
  | 0.15, 256, 1          | 9007    | 0.52 / 5.6 s  | 7.70 / 3.33  | 3.78 / 1.56 | 8.91 / 1.45 | 9.46 / 3.31  |
  | 0.15, 128, 0.5        | 7925    | 0.40 / 4.4 s  | 8.34 / 3.40  | 3.92 / 1.61 | 8.84 / 1.43 | 9.50 / 3.32  |

  All are better than tracing in every crop and without patches; the faster settings leave faint blotches in the
  previews before denoising. The first version's variants (without the Cornell box's ambient light, against
  references without it; a = 0.2 and 0.1, rays per record, the finest grid):

  | a, rays, finest grid  | Records | Time         | Ceiling      | Back wall   | Box side    | Rough mirror |
  |-----------------------|---------|--------------|--------------|-------------|-------------|--------------|
  | no cache              | -       | 0.50 / 8.1 s | 14.15 / 3.98 | 6.34 / 2.41 | 9.89 / 1.75 | 11.34 / 3.72 |
  | 0.2, 64, 2            | 3556    | 0.25 / 3.1 s | 7.82 / 5.55  | 4.23 / 1.89 | 8.35 / 1.46 | 9.79 / 3.81  |
  | 0.2, 256, 2           | 3556    | 0.32 / 3.5 s | 6.85 / 5.51  | 3.96 / 1.77 | 8.28 / 1.43 | 9.68 / 3.86  |
  | 0.1, 256, 2           | 8315    | 0.44 / 4.6 s | 6.18 / 3.92  | 3.99 / 1.75 | 8.31 / 1.37 | 9.71 / 3.60  |
  | 0.1, 256, 4           | 4275    | 0.30 / 3.9 s | 6.18 / 3.92  | 4.00 / 1.75 | 8.66 / 1.49 | 9.88 / 3.61  |
  | 0.1, 256, 8           | 1924    | 0.33 / 3.9 s | 6.17 / 3.92  | 4.02 / 1.76 | 9.23 / 1.62 | 10.21 / 3.63 |

- **Hero wavelengths** (found by the user, 2026-11-24: super-sampling 2 and more brightened the red and green walls):
  the sub-samples' hero wavelengths stratified the whole visible range, but a context's 8 wavelengths lie 50 nm
  apart, so 1, 2 x 2 and 8 sub-samples all took the same 8 wavelengths (380, 430, ..., 730 nm), a fixed quadrature
  that darkened saturated reds and desaturated the greens; from 3 x 3 on they spread. Since `9ecf505` (the spectral
  pipeline). Now the strata divide one wavelength step (`createSpectralContexts`): 2 x 2 sub-samples take 32
  wavelengths, 3 x 3 take 72. The Cornell box's preview at super-sampling 0 to 3 (with its ambient light of that
  day), mean display value of the red wall (red channel) and the green wall (red channel):

  | Super-sampling | Before: red, green | Now: red, green |
  |----------------|--------------------|-----------------|
  | 0 (1)          | 138.6, 77.8        | 163.1, 23.7     |
  | 1 (2 x 2)      | 138.8, 77.9        | 153.8, 57.4     |
  | 2 (3 x 3)      | 153.7, 59.0        | 153.8, 58.6     |
  | 3 (4 x 4)      | 154.1, 63.0        | 153.8, 58.9     |

    - From 2 x 2 on the walls agree. A single sub-sample (the fast preview) still gives every pixel the same 8
      wavelengths, now the steps' midpoints (405 to 755 nm): the red wall +6 %, the green wall more saturated (since
      solved for denoised previews, `DONE/rendering-preview-spectral-bias.md`). The previews' brightness shift
      against the references fell with it (Cornell rough mirror -7.1 -> -0.7 %, ceiling -5.3 -> -2.9 %, the box's
      side -7.5 -> +4.0 % without cache and denoising): much of what "Current noise" put down to the number of
      sub-samples was this quadrature.
    - Whitted renders change too: full renders by at most 0.02 % in mean (1 8-bit step in 10 to 17 % of the pixels),
      previews by -0.8 to +2.0 % (other fixed wavelengths). Test:
      `RaytracerTest::subSampleCountsAgreeOnSaturatedColours` (a saturated red wall at 2 x 2, 3 x 3 and 4 x 4 within
      1 %; before, 2 x 2 came out 24 % darker).

The defaults against the same build without the cache and the denoiser (adaptive sampling at 2/255 and 16 light picks
in both, every scene without ambient light), RMS error preview / full render:

| Crop                 | Without       | Defaults      |
|----------------------|---------------|---------------|
| Cornell rough mirror | 10.65 / 3.71  | 5.55 / 3.77   |
| Cornell teapot       | 10.99 / 2.38  | 8.94 / 2.24   |
| Cornell ceiling      | 13.74 / 3.97  | 4.38 / 3.94   |
| Cornell back wall    | 5.81 / 2.42   | 2.66 / 2.02   |
| Cornell box side     | 8.79 / 1.72   | 5.64 / 1.52   |
| Showcase bunny       | 8.02 / 3.02   | 6.09 / 2.86   |
| Showcase dragon      | 22.98 / 15.52 | 19.71 / 15.49 |
| Showcase floor       | 10.14 / 6.96  | 6.88 / 6.96   |
| Materials metals     | 22.81 / 2.96  | 21.01 / 2.95  |

The full render of the Cornell box's rough mirror comes out a little worse (3.71 -> 3.77; shift -2.5 -> -2.9 %): it
reflects the ceiling, whose records miss the light of the rare paths their limit leaves out.

Render times of all test scenes, preview / full render (one run each, measured before the ambient lights went to 0,
which leaves the rays the same; times vary by up to 10 %): before section 8 (no cache, no denoising, every light),
now, and now without the cache:

| Scene        | Before        | Now            | Without cache |
|--------------|---------------|----------------|---------------|
| Cornell box  | 0.54 / 8.0 s  | 0.61 / 5.8 s   | 0.64 / 7.9 s  |
| Test Scene 1 | 0.41 / 3.4 s  | 0.56 / 3.9 s   | 0.44 / 3.5 s  |
| Test Scene 2 | 1.37 / 12.5 s | 1.51 / 12.9 s  | 1.60 / 13.2 s |
| Dragon       | 2.03 / 11.7 s | 2.57 / 13.1 s  | 2.15 / 12.3 s |
| Procedural   | 37.6 s / -    | 12.2 / 120 s   |               |
| Forms        | 0.22 / 2.8 s  | 0.36 / 2.9 s   | 0.35 / 2.8 s  |
| Lighting     | 0.47 / 4.1 s  | 0.48 / 3.4 s   | 0.55 / 4.7 s  |
| Shading      | 0.30 / 4.6 s  | 0.45 / 5.0 s   | 0.35 / 4.6 s  |
| Materials    | 0.42 / 6.5 s  | 0.54 / 6.4 s   | 0.56 / 6.8 s  |
| Meshes       | 0.24 / 2.5 s  | 0.44 / 3.2 s   | 0.35 / 2.5 s  |
| Showcase     | 1.17 / 13.9 s | 1.45 / 15.4 s  | 1.26 / 14.8 s |

The previews pay for the denoiser (about 0.1 s). Since its fixes, the cache buys quality more than time: it saves time
where diffuse rays dominate (the Cornell box, Lighting and Materials full renders) and costs some elsewhere, most in
detailed scenes (Meshes +26 %, Dragon +6 %, Showcase +4 % against no cache).

## Measuring the candidates

The candidates were measured like the earlier parts (method in section 1). The scripts lived in a session's
scratchpad and stay out of the repository (the user's decision, 2026-11-22): rebuild them for further measurements.

What they did:

- Render each candidate's options with `--exit-after-render`, preview and `--full-render`, with `--seed=0` to `3`, the
  references with `--full-render --super-sampling=7 --glossy-clamp=0 --adaptive-rounds=0` (seeds 0 and 1; since
  section 8 also `--irradiance-caching=0`, previews with `--denoise=0` to see their noise).
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

## Constraints

- `--glossy-samples=0 --sphere-light-samples=0 --diffuse-samples=0` (Whitted) stays byte-identical.
- Images stay deterministic per build (integer seeds, `shading/sampling.h`).
- Tests for the estimators (e.g. MIS weights summing to 1, the clamping bound, the stratification of the joint
  sequence) next to `SamplingTest` and `SceneShaderTest`.

## Key files

- `sources/raytracing/src/raytracing/Raytracer.cpp` (`traceGlossyReflection`, `traceTransmission`,
  `traceDiffuseReflection`, `trace`: adaptive sampling, the irradiance records, the denoiser's guides)
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (`computeLitSphereFraction`, highlights, the point
  light picks)
- `sources/raytracing/src/raytracing/ATrousDenoiser.cpp`, `PixelFeatures.cpp`, `IrradianceCache.cpp`,
  `IrradianceRecord.cpp`, `AdaptiveRounds.cpp`
- `sources/raytracing/src/raytracing/shading/sampling.cpp`, `RaytracerPackets.cpp` (sub-samples)
- `sources/raytracerui/src/tests/ShowcaseScene.cpp`, `CornellBoxScene.cpp`, `ProceduralScene.h`
- `sources/raytracerui/src/main.cpp`, `RaytracerUI.cpp`, `OpenGLWindowsRaytracerUI.cpp` (defaults, options, Config
  panel)

## Commits

- `1fd428a` feat(raytracing): reduce the noise of the distribution ray tracing
- `c07d001` feat(raytracing): add box lights and light the Cornell box with one
- `f2e30b5` feat(raytracing): weigh area light highlights by MIS, refract rough glass, sample adaptively
- `1d485c1` feat(raytracing): bounce diffuse light, cache irradiance, denoise previews
