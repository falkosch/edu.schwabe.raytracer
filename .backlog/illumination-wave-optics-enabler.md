# Enabler: complex-valued Stokes spectrum, optical path-length tracking, thin-film stack utilities

## Sequencing

- **Type:** enabler
- **Level:** 4 (wave optics — interference, diffraction, thin-film)
- **Order:** 3
- **Depends on:** `illumination-polarized-spectral-enabler.md`
- **Blocks:** `illumination-wave-optics.md`

## Motivation

Geometric ray optics throws away phase. To model interference, diffraction, and thin-film
effects (iridescence on soap, oil, beetle wings, anti-reflective coatings), each spectral
sample must carry a complex amplitude rather than a real-valued intensity, and rays must
track accumulated optical path length.

This enabler adds the complex-valued Stokes container, the path-length accumulator, and
thin-film BRDF utilities **as new types and functions, unwired from the active integrator**.

**Strong recommendation, captured here so it lives in the backlog:** for an educational
hobby renderer, a parametric thin-film BRDF at level 2 (RGB-coded analytical iridescence
on top of the spectral pipeline) gives 95% of the visible value at 5% of the engineering
cost. This level-4 enabler exists to round out the ladder; a separate, lighter "parametric
iridescence" backlog item is what most projects should pursue instead.

## Storage container design

```cpp
// sources/raytracing/include/raytracing/shading/spectral/wave.h
//
// Jones vector representation (fully polarized, coherent light).
// Jones calculus is the natural formalism for coherent wave optics (thin-film
// interference, diffraction). For partially polarized / incoherent light, use
// PolarizedSpectrum (Stokes/Mueller formalism from level 3).
struct WaveSpectrum {
    v_f32_8 values[4];     // Ex_re, Ex_im, Ey_re, Ey_im  (Jones vector per λ)
    float   heroLambda;
    float   pathLength;    // accumulated optical path, n * geometric_distance, in nm
};
```

- 4 × `v_f32_8` per ray = 128 B + 8 B for scalars = 136 B unpadded; with `__m256`
  alignment (32 B), `sizeof(WaveSpectrum)` = **160 B** (same as `PolarizedSpectrum`).
- AVX-native: 4 registers. SSE-only builds are not a target (AVX2 is the minimum).
- Phase is encoded directly in the complex Jones components (`Ex_re + i·Ex_im`, etc.).
  `pathLength` tracks cumulative optical path length for **coherence-length comparison
  only** (not for computing phase — that is already in the complex amplitudes).
- When a ray segment advances, the Jones components are rotated by the phase increment
  `Δφ = 2π · n · d / λ` via complex multiplication by `exp(i·Δφ)`.
- Coherence: photons recombine coherently only if their path-length difference is below the
  source's coherence length (microns for incandescent, hundreds of meters for lasers).
  Beyond that, fall back to incoherent intensity addition (the level-3 polarized form).

## Scope of the enabler

### 1. Type and basic ops

- `WaveSpectrum`:
    - `fromIncoherent(PolarizedSpectrum) -> WaveSpectrum` — sets phase to 0, `pathLength`
      to 0. Conversion from Stokes to Jones: for the fully-polarized component, recover
      `(Ex, Ey)` from `(S0, S1, S2, S3)` up to an arbitrary global phase (set to 0).
      For unpolarized input `(S0, 0, 0, 0)`: set `Ex = sqrt(S0/2)`, `Ey = sqrt(S0/2)`,
      both real. Note: Jones calculus is exact only for fully polarized light; partially
      polarized input loses the unpolarized component.
    - `extractIncoherent() -> PolarizedSpectrum` — compute Stokes parameters from the Jones
      vector: `S0 = |Ex|² + |Ey|²`, `S1 = |Ex|² − |Ey|²`, `S2 = 2·Re(Ex·conj(Ey))`,
      `S3 = 2·Im(Ex·conj(Ey))`. This is always well-defined (no negative sqrt).
    - Complex add/multiply per channel — vectorized over `v_f32_8`. Use the established
      `(a + bi) * (c + di) = (ac − bd) + (ad + bc)i` pattern with two FMA chains.
- Path-length and phase accumulation:
    - `advance(WaveSpectrum, Float ior, Float distance, v_f32_8 wavelengths)` — adds
      `ior * distance` to `pathLength` and rotates Jones components by the phase increment
      `Δφ_k = 2π · ior · distance / λ_k` via complex multiply by `exp(i·Δφ_k)`,
      vectorized across all 8 wavelengths. Called on every ray segment.

### 2. Coherence-length bookkeeping

- `coherenceLength(float sourceBandwidthNm) -> float` — computes coherence length from
  spectral bandwidth (e.g. ~1 μm for thermal/broadband, meters for laser lines). Standalone
  function; does not modify `SurfaceShading` or any active-path type. Per-material coherence
  configuration is deferred to the implementation item.
- `lossOfCoherence(WaveSpectrum, float coherenceLen) -> bool` — predicate that tests
  whether `pathLength` exceeds the coherence length. When true, the integrator should call
  `extractIncoherent()` and continue with the polarized (level-3) carrier for the remainder
  of the path.
- This is the messy bookkeeping the enabler is meant to centralize.

### 3. Thin-film BRDF utilities

- `raytracing/shading/spectral/thinfilm.h`:
    - Single-layer thin-film over a substrate: `evaluateThinFilm(NoV, ηFilm, ηSubstrate,
      thickness, wavelengths_v) -> JonesMatrix` — exact analytical solution for one
      layer, including phase via `2π · 2 · ηFilm · thickness · cos θ_t / λ`.
    - Multi-layer transfer-matrix method: `evaluateLayerStack(layers[], NoV, λ_v) ->
      JonesMatrix` — for multi-layer interference filters (anti-reflective coatings,
      dielectric mirrors). Implemented as 2×2 complex transfer-matrix products per layer
      per λ. `JonesMatrix` = 4 complex entries per λ = 8 × `v_f32_8`.
- These return Jones matrices (not Mueller matrices) because thin-film interference requires
  coherent (phase-preserving) propagation. Applying a `JonesMatrix` to a `WaveSpectrum`
  is a 2×2 complex matrix-vector multiply on `(Ex, Ey)` per λ. When needed in the
  incoherent (Stokes/Mueller) path, convert via the standard Jones-to-Mueller mapping.

### 4. Diffraction (lens + aperture only)

- Out of scope for this enabler beyond a stub: declare a
  `applyApertureDiffraction(WaveSpectrum, apertureShape, λ) -> WaveSpectrum` function
  signature, but implement only as an identity passthrough. Real diffraction integrals
  are an open research area; ship the API surface so the implementation item can plug in
  Fraunhofer / Fresnel approximations later.

### 5. Tests

- Two-slit interference: create two `WaveSpectrum`s representing contributions from each
  slit to a sensor point; advance each by different path lengths (different phase
  rotations); add coherently; assert that `extractIncoherent()` exhibits the expected
  cosine fringe pattern as the path difference varies. Pure-API test, no integrator needed.
- Soap-film iridescence: `evaluateThinFilm` for ηFilm=1.33, thickness=300 nm, swept over
  the visible — produces the expected color sequence (yellow → magenta → cyan as
  thickness changes).
- Coherence loss: a `WaveSpectrum` advanced past a 1 μm threshold returns the same
  intensities under `extractIncoherent()` regardless of the phase content (within
  averaging tolerance).
- Round-trip `fromIncoherent(unpolarized(S0)) → extractIncoherent()` preserves `S0`
  intensity. For fully polarized input, round-trip preserves all 4 Stokes parameters
  to within `1e-5f`. For partially polarized input, document that the unpolarized
  component is approximated (Jones calculus limitation).

## Out of scope (defer to the implementation item)

- Switching ray throughput to `WaveSpectrum`.
- Per-segment `advance()` on the active path.
- Coherence-loss detection and switchback to polarized.
- Wiring thin-film BRDFs into materials.
- Lens-side diffraction beyond the API stub.

## Non-degradation guarantee

- Zero changes to the active integrator path.
- New types `#include`d only by tests until the implementation item lands.
- Regression baseline bit-identical.

## Key files (new)

- `sources/raytracing/include/raytracing/shading/spectral/wave.h`
- `sources/raytracing/include/raytracing/shading/spectral/thinfilm.h`
- `sources/raytracing/include/raytracing/shading/spectral/coherence.h`
- `sources/raytracing/src/raytracing/shading/spectral/*.cpp` (wave, thinfilm, coherence)
- `tests/raytracing.native-test/src/raytracing/shading/spectral/wave_*_test.cpp`
- `tests/raytracing.native-test/src/raytracing/shading/spectral/thinfilm_*_test.cpp`
