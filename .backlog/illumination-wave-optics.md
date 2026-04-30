# Implementation: switch ray throughput to WaveSpectrum with phase + coherence tracking

## Sequencing

- **Type:** implementation
- **Level:** 4 (wave optics — interference, diffraction, thin-film)
- **Order:** 4
- **Depends on:**
    - `illumination-wave-optics-enabler.md` (complex Stokes container, path-length tracker, thin-film utilities)
    - `illumination-polarized-spectral.md` (the integrator already operates on
      `PolarizedSpectrum` carried by rays — this item upgrades that carrier to complex
      amplitudes with phase)
- **Blocks:** *(top of the ladder for this plan)*

## Motivation

`illumination-wave-optics-enabler.md` ships `WaveSpectrum`, optical-path-length tracking,
thin-film BRDFs, and coherence-length bookkeeping. This item flips the integrator to carry
complex-amplitude Stokes radiance, accumulates phase across segments, applies thin-film
BRDFs in materials, and falls back to the polarized (level-3) carrier when coherence is lost.

**Strong recommendation, repeated from the enabler:** unless wave-optics correctness is the
specific goal of this project, do not pursue this item. A parametric thin-film BRDF at
level 2 or 3 produces the same visible iridescence at a fraction of the engineering and
runtime cost. Document the decision, close this item if so, and the enabler retains
standalone value as research-tier infrastructure.

## Scope

### 1. Hybrid illumination type strategy

The illumination type system uses a **hybrid approach**: the shader chain types
(`SurfaceShading`, `LightShading`, `BRDFParameters`, `IlluminatedIntersection`,
`Raytracer::applyBRDF`, Shader outputs) remain `PolarizedSpectrum` (level 3). Wave-optics
computation happens **locally at thin-film/interference surfaces** using `WaveSpectrum`
and `JonesMatrix`, then converts back to `PolarizedSpectrum` via `extractIncoherent()`.

This avoids the cost of carrying complex amplitudes along entire ray paths (which is wasted
for the vast majority of interactions that are incoherent). Only the thin-film BRDF
evaluation itself operates in Jones space.

- `Raytrace` gains `float pathLength` field (in addition to the existing `heroLambda` and
  `stokesReference` from levels 2 and 3). `pathLength` accumulates optical path length
  for coherence-length checks at thin-film surfaces. Ray and RayCast remain geometry-only.
- Per-ray packet layout is unchanged from level 3 (4× `v_f32_8` for Stokes channels).
  `pathLength` adds one scalar per ray.

### 2. Optical path-length tracking

- Every ray segment of length `d` through medium with IOR `η`:
  `pathLength += η * d`. This is a scalar accumulation on `Raytrace`, not a phase rotation
  on the throughput (since throughput remains `PolarizedSpectrum`).
- `pathLength` is used at thin-film surfaces to check coherence: if the accumulated path
  exceeds the source's coherence length, the thin-film interference is suppressed and the
  surface falls back to the level-3 Mueller-matrix Fresnel.

### 3. Coherence-aware thin-film evaluation

- At each thin-film surface, check `pathLength` against the source's coherence length:
    - If coherent: evaluate thin-film interference via Jones matrices (full iridescence).
    - If incoherent: fall back to level-3 Mueller-matrix Fresnel (no interference effects).
- This check is local to thin-film surfaces — no per-segment branching or `isCoherent`
  flag needed. The hot path (non-thin-film surfaces) is unaffected.

### 4. Thin-film BRDFs via Shaders

Thin-film properties are expressed through Shaders, consistent with the existing
architecture (no Material type introduced):

- Add a `ThinFilmShader` — a `Shader<ObjectShader, FacetIntersection, Float4>` that
  samples per-intersection film thickness and IOR. For single-layer films, `Float4`
  encodes `(thickness, η_film, η_substrate, 0)`. Multi-layer stacks require a separate
  `ThinFilmStackShader` returning an index/pointer into a pre-built layer table in
  `Resources`, since variable-length data cannot fit in `Float4`.
- `SurfaceShading` gains a thin-film descriptor: a `Float4 thinFilmParams` field (zero =
  no thin film, matching the existing convention where zero values mean "disabled"). For
  multi-layer stacks, the W component stores a table index.
- When the thin-film descriptor is non-zero, the specular BRDF evaluation locally
  constructs a `WaveSpectrum` from the incoming `PolarizedSpectrum`, applies the
  `JonesMatrix` from `evaluateThinFilm` / `evaluateLayerStack`, and converts the result
  back to `PolarizedSpectrum` via `extractIncoherent()`. The rest of the ray path stays
  in Stokes/Mueller space.
- This is where iridescence shows up visually: soap, oil, beetle wings, anodized metal,
  AR coatings on lenses.

### 5. Lens-side diffraction (optional within this item)

- If aperture diffraction is desired (lens flare, fringing on bokeh edges):
  call `applyApertureDiffraction` on the camera ray as it leaves the aperture.
- Otherwise leave the stub unwired and document as a follow-up.

### 6. Framebuffer output

- Identical to level 3: ray throughput is `PolarizedSpectrum` throughout.
  `extractIntensity` → XYZ → display RGB → tonemap.

## Acceptance criteria

- Soap-film bubble: a thin transparent shell with `thinFilmParams` single layer at
  ~300 nm shows the expected color shift across the bubble surface.
- Anti-reflective coating: a glass plane with a 100 nm MgF₂ thin film exhibits reduced
  reflection at green wavelengths (~ matches the design wavelength).
- Regression baseline scene (no thin-film materials) runs to completion with performance
  identical to level 3 — the hybrid approach adds only `pathLength` accumulation (one
  scalar add per segment). Scenes with thin-film materials: budget ≤ 1.5× level-3
  `Duration` for the thin-film objects.
- No runtime assertion fires on coherence bookkeeping over a full render.

## Out of scope

- Birefringent media, bulk dispersion in volumes, fluorescence, phosphorescence,
  Raman scattering — research-tier additions beyond this item.
- Time-of-flight / transient rendering. Conceptually similar (path length tracked) but
  the use case is sensor simulation, not appearance.
- Quantum treatment of light. We stop at classical wave optics.

## Risk / fallback

- **Coherence bookkeeping is the bug magnet.** Most light sources are incoherent; most
  geometric paths exceed coherence length within nanometers. If the bookkeeping is buggy,
  expect thin-film effects to silently disappear (false coherence loss) or to apply where
  they shouldn't (false coherence retention). The hybrid approach limits the blast radius:
  coherence checks happen only at thin-film surfaces, not at every interaction.
- **Performance.** The hybrid approach adds only `pathLength` accumulation (one scalar add
  per segment) to the hot path. The Jones-matrix thin-film evaluation is local to surfaces
  with thin-film materials. Expect negligible overhead on non-thin-film scenes.
- **Jones calculus limitation:** Jones vectors represent only fully polarized light. At
  thin-film surfaces, `fromIncoherent()` approximates partially polarized input as fully
  polarized (the dominant polarization component). For most rendering scenarios this is
  visually indistinguishable, but it is a known approximation.

## Key files (modified)

- `sources/raytracing/include/raytracing/shading/Raytrace.h` — add `pathLength` field
- `sources/raytracing/include/raytracing/shading/SurfaceShading.h` — add `thinFilmParams` field
- `sources/raytracing/include/raytracing/shading/shaders/ThinFilmShader.h` — new shader
- `sources/raytracing/src/raytracing/Raytracer.cpp` — per-segment pathLength accumulation
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` — Fresnel callsites route
  through thin-film evaluation when `thinFilmParams` is non-zero
- `sources/raytracerui/src/tests/*.cpp` — thin-film bubble scene, AR-coating scene
