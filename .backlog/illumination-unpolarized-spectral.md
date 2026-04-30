# Implementation: replace RGB ray throughput with unpolarized spectral, hero-wavelength sampled

## Sequencing

- **Type:** implementation
- **Level:** 2 (unpolarized spectral)
- **Order:** 2
- **Depends on:** `illumination-rgbs-type.md`, `illumination-unpolarized-spectral-enabler.md`
- **Soft prerequisite:** `illumination-rgb-linear-pipeline.md` is *not* required — this item
  replaces RGBS with Spectrum across the shader chain — but its GGX/view-transform work is
  reusable here, and doing 1.5 first makes the 2.0 diff smaller. If 1.5 has not landed,
  this item must additionally specify a working RGB display space (recommend ACEScg) for
  the spectrum→display conversion.
- **Blocks:** `illumination-polarized-spectral.md`

## Motivation

`illumination-unpolarized-spectral-enabler.md` ships `Spectrum`, the Jakob-Hanika upsampling
LUT, metal IORs, CIE matching functions, and conversion utilities. This item replaces RGBS
with Spectrum across the entire shader chain (SurfaceShading, LightShading, BRDFParameters,
IlluminatedIntersection, applyBRDF, Shader outputs), evaluates every BRDF/light/texture
per-λ, and adds spectrum→XYZ→display-RGB at the framebuffer write.

Visible payoff: real metals, dispersion in glass, light sources with proper spectral character
(sodium lamps, fluorescents, daylight under different illuminants).

## Scope

### 1. Illumination type swap across the shader chain

Replace `RGBS` → `Spectrum` in all illumination-carrying types. Ray, RayCast remain
geometry-only; illumination continues to flow through the existing shader chain:

- `SurfaceShading`: diffusion, reflectance, specular, emittance → `Spectrum`
- `LightShading`: ambient, diffuse, specular, reflected, transmitted → `Spectrum`
- `BRDFParameters`: reflectanceCoefficient, fractionTransmitted → `Spectrum`
- `IlluminatedIntersection`: color → `Spectrum`
- `Raytracer::applyBRDF` return type → `Spectrum`
- `Shader` output types for color/radiance → `Spectrum`
- `SceneShader::sampleBackground` return → `Spectrum`

Hero-wavelength state goes on `Raytrace` (which already carries per-ray tracing state:
`traceDepth`, `visibilityIndex`). This keeps Ray/RayCast geometry-only while making λ
accessible throughout the recursive `trace()` call:

- `Raytrace` gains a `float heroLambda` field.
- `heroLambda` is sampled per primary ray (stratified across pixel samples for variance
  reduction via `superSamplingFactor` from `RaytraceParameters`):
  `heroLambda = 380 + Δλ * stratum(pixelSample) + jitter`.
- All secondary `Raytrace` instances inherit the parent's `heroLambda` — never re-sample
  mid-path.

### 2. Per-λ BRDFs

Every Shader currently returning an `RGBS` returns a `Spectrum` evaluated at the
Raytrace's wavelengths:

- **Schlick-Fresnel:** `F0` is derived from `metalIOR(material, λ)` for metals (real
  per-λ Fresnel using complex `(n, k)` — *the* main visual win) or from the dielectric
  `(η₁, η₂)` ratio for glass/water.
- **Beer-Lambert:** absorption `σ_a(λ)` is either tabulated per material or upsampled
  from an authored RGB tint via Jakob-Hanika; transmittance is `exp(-σ_a · d)` with one
  `v_f32_8 exp` per shading event.
- **GGX / Phong specular:** angular term stays scalar in λ (microfacet geometry is
  wavelength-independent at this level); `F0` is per-λ (above).
- **Diffuse / Lambert:** albedo evaluated from Jakob-Hanika coefficients at the ray's λ.

### 3. Texture pipeline

Bitmap and HDRImage are auxiliary types used as lookup tables inside Shaders
(HDRImageShader, EnvironmentShader). Their storage stays unchanged:

- **Bitmap**: BGR 8-bit per channel (3 bytes per texel, scanline-padded to 4 B).
- **HDRImage**: `Float4` per texel (RGBA 32-bit float), sampled via `sampleBilinear`.

Spectral upsampling happens **at the Shader level, not in the image type**:

- `HDRImageShader::sample()` and `EnvironmentShader::sample()` currently sample an
  HDRImage and return `Float4` (RGBS). After this change they return `Spectrum`:
  sample the HDRImage → get RGB → compute Jakob-Hanika coefficients → evaluate at
  the Raytrace's λ.
- The coefficient computation (`RGB → (c₀, c₁, c₂)`) is a LUT lookup (from the enabler);
  the evaluation (`sigmoid(c₀ + c₁·λ + c₂·λ²)`) vectorizes over `v_f32_8`.
- Filtered sampling via `sampleBilinear` stays in RGB space (blending before upsampling).
  Standard practice; not strictly equivalent to filtering the underlying spectrum but
  visually indistinguishable.

### 4. Light sources

- Analytic lights (point, area, directional) gain a spectrum descriptor: blackbody T,
  CIE illuminant tag, or an authored RGB upsampled via Meng. Default for existing scenes:
  D65 + RGB-derived intensity, preserving current behavior as closely as possible.
- HDR environment maps are sampled as RGB radiance, then upsampled per-pixel via Jakob-
  Hanika at lookup. Document that this is an approximation (the real env-map spectrum is
  unknown).

### 5. Dispersion handling at refractive surfaces

- Snell's law uses `η(λ_h)` — IOR evaluated at the hero λ. The 7 companion λ in general
  refract in different directions.
- Standard handling: **collapse to hero λ on refraction.** Continue tracing only `λ_h`;
  zero out the other 7 channels of `throughput` and **multiply the hero channel by 8** to
  preserve the expected value of the Monte Carlo estimator (one sample now represents all 8
  wavelengths). Variance is reabsorbed by pixel-sample averaging.
- Document as a known approximation; spawning 8 sub-rays is an explicit non-goal here.

### 6. Framebuffer output

- Per pixel, after accumulating `Spectrum result` across pixel samples:
    1. `xyz = spectrumToXYZ(result, heroLambda)`
    2. `rgb_display = xyzToDisplayRGB(xyz, sRGB_or_ACEScg)`
    3. View transform (AgX/ACES from the 1.5 enabler if available) and 8-bit encode.
- HDRImage output retains the spectrum projected to a chosen display primary set; if
  preserving full spectral information matters, add a `.pfm`/`.exr` writer that stores 8
  channels — explicit non-goal in this item, log a follow-up if requested.

### 7. Scene recalibration

- All scenes in `raytracerui/main.cpp` get explicit spectrum-aware light specifications.
- TestLight in particular was tuned for RGB intensities; recalibrate to either a
  blackbody temperature + power or a D65 + lumens spec.
- Document old vs. new intensity in scene comments.

## Acceptance criteria

- A glass prism scene shows visible color separation (rainbow).
- Gold (`Au`) and copper (`Cu`) metals look distinctly yellow and reddish-pink without any
  manual color tweaking — purely from the IOR tables.
- A monochromatic sodium-lamp scene produces a near-monochromatic orange image (the RGB
  pipeline cannot reproduce this).
- Regression baseline scene (teapot) runs to completion. Performance regression budget:
  ≤ 2.5× current `Duration` (~0.07s → ≤ 0.18s) for the first 512×512 frame. Beyond that
  budget, file follow-up perf items rather than block this one.
- `superSamplingFactor` (in `RaytraceParameters`) documented as needing to increase to
  compensate for hero-λ variance (typical: one step up, e.g. 0→1 doubles sample count).

## Out of scope

- Polarization. `Spectrum` is `SpectralSample<1>`; Stokes form (`SpectralSample<4>`) is the
  next level.
- Wave optics / phase / coherence.
- Path tracing — Whitted topology preserved.
- Spectral output formats (8-channel `.pfm`/`.exr`).

## Risk / fallback

- The single biggest source of regression bugs is **forgetting to inherit `heroLambda`** on
  a secondary Raytrace. The current `Raytrace` constructor takes 4 explicit arguments (not
  copy-construct); adding `heroLambda` requires updating the constructor signature and all
  3 spawn sites: (1) primary rays in `constructPackedRaytracesList` (free function in
  `Raytracer.cpp`), (2) `traceReflection`, (3) `traceTransmission`. Add a debug-only
  assertion that every secondary Raytrace's `heroLambda` matches its parent's.
- If the variance increase from hero-λ sampling proves visually unacceptable on the
  existing AA budget, add a fallback "fixed-wavelengths mode" (no hero, λ = {420, 470, 520,
  570, 620, 670, 720, 770} constant) as a debug toggle. Document that this mode loses
  dispersion correctness.

## Key files (modified)

- `sources/raytracing/include/raytracing/shading/Raytrace.h` — add `heroLambda` field
- `sources/raytracing/include/raytracing/shading/SurfaceShading.h` — RGBS → Spectrum
- `sources/raytracing/include/raytracing/shading/LightShading.h` — RGBS → Spectrum
- `sources/raytracing/include/raytracing/BRDFParameters.h` — RGBS → Spectrum
- `sources/raytracing/include/raytracing/IlluminatedIntersection.h` — RGBS → Spectrum
- `sources/raytracing/src/raytracing/Raytracer.cpp` — primary-ray spawn, `applyBRDF`,
  `schlickFresnel`, `fresnelReflectance`, framebuffer spectrum→RGB
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` — lighting accumulation per-λ
- `sources/raytracing/src/raytracing/shading/ObjectShader.cpp` — material sampling per-λ
- `sources/raytracing/include/raytracing/shading/shaders/HDRImageShader.h` — RGB→spectral
  upsampling at sample time
- `sources/raytracing/include/raytracing/shading/shaders/EnvironmentShader.h` — same
- `sources/raytracing/src/raytracing/scene/Resources.cpp` — spectral LUT / IOR data loading
- `sources/raytracerui/src/tests/*.cpp` — scene light specifications and recalibration
  (light spectrum descriptors live in scene definitions, not Resources)
