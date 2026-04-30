# Enabler: linear-RGB pipeline (ACEScg + GGX) without disturbing the current path

## Sequencing

- **Type:** enabler
- **Level:** 1.5 (linear-RGB cleanup)
- **Order:** 0
- **Depends on:** *(none — purely additive; uses vectorization types `m_f32_4x4` / `Float4`
  from the existing library dependency chain)*
- **Blocks:** `illumination-rgb-linear-pipeline.md`
- **Related:** `raytracing-testability.md` §1 (covers exposing Schlick-Fresnel / Phong / `applyBRDF` in a public
  header — useful for side-by-side comparison tests but not required for GGX implementation)

## Motivation

Before touching the spectral track, there is a high-leverage cleanup of the existing RGB path:
declare an explicit working color space (ACEScg-linear or Rec.2020-linear), add an
energy-conserving microfacet BRDF (GGX) alongside the current Phong, and add a proper view
transform at framebuffer write. This brings the look closer to a physically-based renderer
without changing any data type the integrator carries.

This enabler lands every new utility **in parallel** with the existing implementation. Nothing
on the hot path is touched. The follow-up `illumination-rgb-linear-pipeline.md` is what flips
the switch.

## Scope of the enabler

Add the following as new headers/sources, all unused by current code paths:

### 1. Color-space matrices and white-point utilities

- `raytracing/shading/color/primaries.h` — `static const m_f32_4x4` (runtime-initialized;
  `m_f32_4x4` constructors use SSE intrinsics and are not constexpr-eligible) for:
    - `sRGB_linear → XYZ`, `XYZ → sRGB_linear`
    - `ACEScg → XYZ`, `XYZ → ACEScg`
    - `Rec2020_linear → XYZ`, `XYZ → Rec2020_linear`
- `chromaticAdaptation(srcWhite, dstWhite)` — Bradford or CAT16, returns 3×3.
- D65 / D50 / E illuminant constants in xy form.

### 2. View transform

- `raytracing/shading/color/view_transform.h`:
    - `acesFilmic(rgb)` — RRT+ODT approximation (Narkowicz fit is fine for educational use).
    - `agx(rgb)` — Troy Sobotka's AgX (preferred for natural look).
    - `srgbEncode(linear)` / `srgbDecode(encoded)` — the current pipeline writes linear values
      directly to an sRGB display with no transfer function; these add the missing conversion.

### 3. GGX microfacet BRDF (additive, not yet wired)

- `raytracing/shading/brdf/ggx.h`:
    - `ggxD(NoH, alpha)` — Trowbridge-Reitz NDF.
    - `ggxG_smith(NoV, NoL, alpha)` — Smith-correlated geometry term.
    - `evaluateGGX(N, V, L, alpha, F0)` — full BRDF evaluation, RGB-typed.
    - `sampleGGX(N, V, alpha, xi)` — VNDF importance sampling (Heitz 2018).
- Phong stays in place, untouched. GGX simply becomes available.

### 4. Tests

- Round-trip: `sRGB → XYZ → sRGB` should be identity within `1e-6f`.
- Chromatic adaptation D65↔D50 idempotency.
- GGX energy conservation: integrate the BRDF over the hemisphere for a few `(α, F0)` pairs
  and assert ≤ 1.0 within sampling tolerance.

## Out of scope (deferred to the implementation item)

- Switching the framebuffer write to apply a view transform.
- Replacing Phong with GGX in any default material.
- Recalibrating test-scene light intensities for the new linear/tonemapped pipeline.

## Non-degradation guarantee

- Zero changes to existing call sites or data types.
- New headers are not transitively included by anything on the current hot path.
- Regression baseline (~0.07s on teapot, `primaryRays`/`secondaryRays` counters)
  must remain bit-identical after this enabler lands.

## Key files (new)

- `sources/raytracing/include/raytracing/shading/color/primaries.h`
- `sources/raytracing/include/raytracing/shading/color/view_transform.h`
- `sources/raytracing/include/raytracing/shading/brdf/ggx.h`
- `sources/raytracing/src/raytracing/shading/color/*.cpp`
- `sources/raytracing/src/raytracing/shading/brdf/ggx.cpp`
- `tests/raytracing.native-test/src/raytracing/shading/color/*_test.cpp`
- `tests/raytracing.native-test/src/raytracing/shading/brdf/ggx_test.cpp`

## Key files (modified)

- `sources/raytracing/CMakeLists.txt` (register new sources)
- `tests/raytracing.native-test/CMakeLists.txt` (register new test sources)
