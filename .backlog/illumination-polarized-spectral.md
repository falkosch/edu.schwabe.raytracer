# Implementation: switch ray throughput to PolarizedSpectrum with Mueller-matrix BRDFs

## Sequencing

- **Type:** implementation
- **Level:** 3 (polarized spectral)
- **Order:** 3
- **Depends on:**
    - `illumination-polarized-spectral-enabler.md` (Stokes spectrum, Mueller, frame transport, polarized Fresnel)
    - Level 2 unpolarized spectral pipeline (shipped — the integrator already operates
      on a per-λ `Spectrum` carried by rays; this item upgrades that carrier to Stokes
      form rather than introducing spectra from scratch)
- **Blocks:** `illumination-wave-optics.md`

## Motivation

`illumination-polarized-spectral-enabler.md` ships `PolarizedSpectrum`, `MuellerSpectrum`,
ray reference-frame transport, and polarized Fresnel — all unwired. This item flips the
integrator to carry Stokes-vector radiance, applies Mueller matrices at every interaction,
and parallel-transports the Stokes reference frame along ray paths.

Visible payoff (modest): Brewster-angle dimming on dielectrics at grazing incidence,
correct polarized-filter behavior, accurate metal reflectance at high incidence angles,
polarized skylight and water reflections.

**Honest assessment:** for most everyday scenes the visual delta vs. unpolarized spectral is
small. This level is worthwhile if and only if the project specifically wants polarization
phenomena. Otherwise stop at level 2.

## Scope

### 1. Illumination type swap across the shader chain

Replace `Spectrum` → `PolarizedSpectrum` in all illumination-carrying types (the same
set upgraded from RGBS → Spectrum at level 2):

- `SurfaceShading`, `LightShading`, `BRDFParameters`, `IlluminatedIntersection`,
  `Raytracer::applyBRDF`, Shader outputs — all become `PolarizedSpectrum`.
- `BackgroundShader` typedef (`Shader<SceneShader, Float4, Float4>`) and `MaterialShader`
  typedef (`Shader<ObjectShader, FacetIntersection, Float4>`) — output type changes from
  `Float4`/`Spectrum` to `PolarizedSpectrum`. `EnvironmentShader` and `HDRImageShader`
  sample RGB and return `unpolarized(upsampled_spectrum)` at the shader boundary.
- `Raytrace` gains a `Float4 stokesReference` field — unit vector ⊥ to ray direction
  defining S₁'s "vertical". `Raytrace` carries `heroLambda` from level 2;
  this adds a second per-ray field. Ray and RayCast remain geometry-only.
- Primary-ray spawn: Spectrum from level-2 becomes
  `unpolarized(Spectrum::constant(1))` with
  `stokesReference = canonicalReference(direction)`.
- Per-ray packet, the AoS layout (one `Spectrum`/`PolarizedSpectrum` per ray, contiguous
  in the packet array — as specified by the unpolarized enabler) grows from level 2's
  1× `v_f32_8` per ray to 4× `v_f32_8` per ray for the Stokes channels, plus the
  reference vector.

### 2. Frame transport at every interaction

This is the largest source of correctness bugs in polarized renderers. The discipline:

1. On entering a surface interaction, **rotate** the incoming Stokes vector from the
   ray's reference frame into the surface's local s/p frame (`rotateStokesIntoFrame`).
2. Apply the Mueller-matrix BRDF in the local frame.
3. Spawn the outgoing ray with a new `stokesReference` derived from the local frame and
   the outgoing direction.
4. On free-flight (no interaction), the reference is parallel-transported along the
   straight ray — no rotation needed in flat media (relevant only for curved spacetime,
   which is out of scope).

Add debug-only asserts that `dot(stokesReference, direction) < 1e-5f` after every spawn.

### 3. Mueller-matrix BRDFs

Replace every per-λ scalar BRDF call site with the Mueller-matrix variant from the enabler:

- `fresnelDielectricMueller` for glass/water/plastic.
- `fresnelConductorMueller` for metals (Cu, Au, Al, Fe — IORs from the unpolarized enabler).
- `evaluateGGXMueller` for rough specular. (If the enabler shipped only the depolarizing
  fallback, that is acceptable for now; track the proper Mueller derivation as a follow-up
  item.)
- Lambertian diffuse: `MuellerSpectrum::depolarizer()` — diffuse scattering randomizes
  polarization completely, so multiplying through `(1, 0, 0, 0; 0,0,0,0; ...)` is the
  correct model.
- Beer-Lambert volume absorption: `S₀, S₁, S₂, S₃` all attenuate by the same
  `exp(-σ_a · d)`; no off-diagonal coupling unless the medium is birefringent (out of
  scope).

### 4. Light sources

- Most lights (incandescent, LED, sun, area lights) are unpolarized. Initialize as
  `unpolarized(spectrum)`.
- Optional polarized-skylight model: Rayleigh scattering produces partially polarized sky.
  If the existing renderer ships a procedural sky shader, extend it to emit `PolarizedSpectrum`
  with `S₁` non-zero perpendicular to the sun-zenith plane. Otherwise log a follow-up.
- HDR environment maps stay unpolarized (information loss is unavoidable; the maps don't
  carry polarization data).

### 5. Framebuffer output

- Per-pixel: `extractIntensity()` returns the unpolarized `Spectrum`; downstream is
  identical to level 2 (XYZ → display RGB → view transform → 8-bit encode).
- `S₁`, `S₂`, `S₃` are discarded at the framebuffer.
- **Optional polarization-filter rendering:** add a debug polarizing-filter Shader
  that, instead of `extractIntensity`, applies `linearPolarizer(angleRad)` first. This
  shows the actual visual benefit of polarization (sky darkening, glare reduction).
  The filter is a Shader, not a Material — consistent with the existing architecture.

## Acceptance criteria

- Brewster-angle test scene: a glass plane viewed at ~56° (Brewster for n=1.5) shows
  visibly reduced reflection of one polarization. Compare with/without a debug polarizing
  filter — the filter should darken the reflection close to zero at exactly Brewster.
- Crossed polarizers test: light passing through `linearPolarizer(0)` then
  `linearPolarizer(π/2)` produces a black image. Asserts both the Mueller math and the
  frame transport (the second filter is in a different orientation than the first).
- Metals reflect with correct off-diagonal Fresnel terms — measured reflectance ratio
  between s and p polarization at 60° incidence matches reference data within 5% for Au.
- Regression baseline scene runs to completion. Performance budget: ≤ 4× current baseline
  `Duration` (~0.07s → ≤ 0.28s), which is ~1.6× the level-2 result. Illumination is only
  part of per-ray cost (intersection testing dominates), so 4 Stokes channels vs. 1 spectral
  channel does not imply 4× overhead. Beyond that budget, file follow-up perf items.
- No frame-transport assertion fires on any pixel of any test scene over a full render.

## Out of scope

- Wave optics (phase, coherence, thin-film interference).
- Birefringent media (calcite, quartz crystals) — needs ray-splitting at boundaries.
- Polarization-resolved output formats.

## Risk / fallback

- **Frame-transport bugs are silent and visually subtle** — wrong rotations look like
  slightly miscolored highlights, not obviously wrong renders. Recommend a "polarization
  visualizer" debug shader that maps S₁/S₂/S₃ to false color so frame errors are visible.
- **Performance regression** is the more likely blocker than correctness. If the 4× budget
  is exceeded, options:
    - Drop GGX to depolarizing fallback (skip the proper Mueller derivation for rough
      surfaces).
    - Skip frame transport on diffuse-only paths (depolarizing ⇒ no rotation needed
      after the diffuse interaction).
    - Compile-time toggle to revert to level 2 for benchmark builds.
- **If the project decides this level is not worth the cost,** document the decision in
  this file and close it; the enabler still has standalone value as a polarization
  toolkit for future use.

## Key files (modified)

- `sources/raytracing/include/raytracing/shading/Raytrace.h` — add `stokesReference` field
- `sources/raytracing/include/raytracing/shading/SurfaceShading.h` — Spectrum → PolarizedSpectrum
- `sources/raytracing/include/raytracing/shading/LightShading.h` — Spectrum → PolarizedSpectrum
- `sources/raytracing/include/raytracing/BRDFParameters.h` — Spectrum → PolarizedSpectrum
- `sources/raytracing/include/raytracing/IlluminatedIntersection.h` — Spectrum → PolarizedSpectrum
- `sources/raytracing/src/raytracing/Raytracer.cpp` — primary-ray spawn, framebuffer extract
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` — every BRDF callsite to Mueller form
- `sources/raytracing/include/raytracing/shading/shaders/` — optional polarizing-filter Shader
- `sources/raytracing/src/raytracing/scene/Resources.cpp` — light initialization
- `sources/raytracerui/src/main.cpp` — Brewster + crossed-polarizer test scenes
