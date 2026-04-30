# Enabler: Stokes-vector spectrum, Mueller matrix, ray reference frame, polarized Fresnel

## Sequencing

- **Type:** enabler
- **Level:** 3 (polarized spectral)
- **Order:** 2
- **Depends on:** `illumination-unpolarized-spectral-enabler.md` (the `SpectralSample<N>`
  template, Jakob-Hanika upsampling, metal IOR tables, CIE conversion utilities)
- **Blocks:** `illumination-polarized-spectral.md`,
  `illumination-wave-optics-enabler.md`

## Motivation

The unpolarized spectral enabler delivers `SpectralSample<1>`. To support polarization, we
need:

- A 4-channel spectrum (Stokes vector per λ, packaged as `SpectralSample<4>`).
- A Mueller-matrix type that transforms it.
- Reference-frame bookkeeping on rays so Stokes components have a well-defined basis.
- Polarized variants of the Fresnel and BRDF helpers.

This enabler ships those types and utilities **in parallel** with the unpolarized path. Rays
keep carrying `Spectrum` (= `SpectralSample<1>`); nothing on the active integrator path is
touched.

## Storage container design

```cpp
// sources/raytracing/include/raytracing/shading/spectral/polarized.h
using PolarizedSpectrum = SpectralSample<4>;
//   PolarizedSpectrum::values[0]  → S0 (total intensity per λ)
//   PolarizedSpectrum::values[1]  → S1 (horizontal vs. vertical linear, per λ)
//   PolarizedSpectrum::values[2]  → S2 (diagonal linear, per λ)
//   PolarizedSpectrum::values[3]  → S3 (circular, per λ)
//   PolarizedSpectrum::heroLambda → shared with the unpolarized form

// Mueller matrix as 16 × v_f32_8 (4×4 of per-λ entries)
struct MuellerSpectrum {
    v_f32_8 m[4][4];
};
```

- `PolarizedSpectrum` is 4 × `v_f32_8` + 1 float ≈ 132 B (vs. 36 B unpolarized).
- `MuellerSpectrum` is 16 × `v_f32_8` ≈ 512 B; constructed lazily at each shading event,
  never stored on rays or materials.
- AVX-native: each `v_f32_8` is one register; SSE-only: emulated as 2× `v_f32_4` per channel.

## Scope of the enabler

### 1. Type and basic ops

- `PolarizedSpectrum`:
    - `unpolarized(Spectrum totalIntensity) -> PolarizedSpectrum` — initializes
      `(S0=values, S1=S2=S3=0)`. The conversion from unpolarized to polarized lives here.
    - `extractIntensity() -> Spectrum` — returns `S0`. Used at framebuffer write.
    - `operator+`, `operator*` (component-wise per channel per λ).
- `MuellerSpectrum`:
    - `identity()`, `depolarizer()`, `linearPolarizer(angleRad)`, `quarterWavePlate`,
      `halfWavePlate` — analytic constructors used by tests and by Fresnel implementations.
    - `apply(MuellerSpectrum, PolarizedSpectrum) -> PolarizedSpectrum` — 4×4 matrix-vector
      multiply per λ, vectorized as `v_f32_8` FMAs. **Implementation note:** must tile
      row-by-row (load 4 matrix entries, accumulate against 4 Stokes channels, produce 1
      output channel) to fit within AVX2's 16 YMM registers (~11 live registers per row).
      Loading the full 4×4 matrix (16 registers) plus inputs would exceed the register file.
    - `compose(MuellerSpectrum, MuellerSpectrum) -> MuellerSpectrum` — for stacked optical
      elements. Register spilling is expected (two 16-register matrices + output); tile
      element-by-element.

### 2. Reference-frame transport utilities (standalone, no Ray modification)

Stokes components are defined relative to a "reference vertical" perpendicular to the ray
direction. The enabler ships frame-transport functions as standalone utilities — no existing
type is modified.

- `parallelTransport(stokesReference, oldDir, newDir) -> Float4` — rotate the reference
  along great-circle when ray bends (refraction or reflection).
- `canonicalReference(direction) -> Float4` — deterministic initial Stokes frame for a
  given ray direction.
- `rotateStokesIntoFrame(PolarizedSpectrum, oldRef, newRef, rayDir) -> PolarizedSpectrum`
  — applies the cosine/sine rotation on `(S₁, S₂)`. This is just a 2×2 rotation embedded
  in a Mueller matrix; provide it as a fast path that doesn't allocate the full 4×4.

Note: the implementation item decides where to store the per-ray `stokesReference` (likely
on `Raytrace`, which already carries per-ray state).

### 3. Polarized Fresnel

- `fresnelDielectricMueller(cosThetaI, η₁, η₂) -> MuellerSpectrum` — proper s/p split,
  returns the Mueller form (off-diagonal entries non-zero ⇒ unpolarized incidence
  becomes partially polarized after reflection).
- `fresnelConductorMueller(cosThetaI, n_v, k_v) -> MuellerSpectrum` — conductors with
  complex IOR per λ; reuses `metalIOR` from the unpolarized enabler.
- Tests: at Brewster's angle (`tan θ_B = η₂/η₁`), the polarized Fresnel must produce
  zero p-component reflectance — assert numerically.

### 4. Polarized GGX (Mueller form, optional within the enabler)

- Heitz et al. extension to GGX with polarization. Sketch the API
  (`evaluateGGXMueller(...) -> MuellerSpectrum`) and ship a depolarizing fallback
  (returns `unpolarized(unpolarizedGGX) ∘ depolarizer()`) until the proper Mueller
  derivation lands.
- Mark the depolarizer fallback as TODO with a citation; the implementation item can
  upgrade later.

### 5. Tests

- `MuellerSpectrum::identity()` applied to any `PolarizedSpectrum` is identity within
  `1e-6f`.
- `linearPolarizer(0)` followed by `linearPolarizer(π/2)` produces zero output (crossed
  polarizers).
- Brewster-angle reflectance: `fresnelDielectricMueller` at `θ_B` for glass (η=1.5) gives
  zero p-component.
- `parallelTransport` round-trip on a closed loop returns the original frame within
  `1e-5f` (no geometric phase accumulated for short loops).
- `unpolarized(Spectrum) → extractIntensity() → ≈ Spectrum` round-trip.

## Out of scope (defer to the implementation item)

- Switching ray throughput from `Spectrum` to `PolarizedSpectrum`.
- Carrying `stokesReference` on every actual ray.
- Replacing scalar Fresnel calls in the active BRDF path.
- Frame transport at every interaction.

## Non-degradation guarantee

- Zero changes to `Ray` / `RayCast` / `Raytrace` / integrator on the active path.
- New types are defined in headers but only `#include`d by tests and by the (still-not-
  active) implementation item once it lands.
- Regression baseline bit-identical after this enabler lands.

## Key files (new)

- `sources/raytracing/include/raytracing/shading/spectral/polarized.h`
- `sources/raytracing/include/raytracing/shading/spectral/mueller.h`
- `sources/raytracing/include/raytracing/shading/spectral/frame_transport.h`
- `sources/raytracing/include/raytracing/shading/spectral/fresnel_polarized.h`
- `sources/raytracing/src/raytracing/shading/spectral/*.cpp`
- `tests/raytracing.native-test/src/raytracing/shading/spectral/polarized_*_test.cpp`
