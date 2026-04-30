# Enabler: AVX-wide spectral storage container, upsampling tables, and CIE conversion utilities

## Sequencing

- **Type:** enabler
- **Level:** 2 (unpolarized spectral)
- **Order:** 1
- **Depends on:** `implement-planned-vector-types.md` (specifically the `v_f32_8` rollout —
  defined in `vectorization/v_f32_8/` following the existing `v_*_N` directory convention;
  on AVX2 it lowers to one `__m256`),
  `illumination-rgbs-type.md` (section 5's `xyzToDisplayRGB` returns `RGBS`)
- **Blocks:** `illumination-unpolarized-spectral.md`,
  `illumination-polarized-spectral-enabler.md`
- **Related:** `raytracing-testability.md` §1 (BRDF helpers in headers — needed for per-λ
  rewrites in the implementation item, not for this enabler), `fma-intrinsics.md` (FMA
  wrappers will be heavily used by per-λ Beer-Lambert / Schlick-Fresnel)

## Motivation

To bring the renderer from RGB to wavelength-resolved illumination, we need a single storage
container that carries N spectral samples plus a hero wavelength scalar, alongside the
RGB-side conversion tables (Jakob-Hanika reflectance upsampling, Meng emissive upsampling,
CIE color-matching functions, metal IORs).

This enabler ships all of the above as **new types and tables, not yet wired into any ray or
material**. It must not change the runtime behavior or performance of the existing RGB path.

## Storage container design

```cpp
// sources/raytracing/include/raytracing/shading/spectral/spectrum.h
//
// Channels = 1 → unpolarized (this level)
// Channels = 4 → polarized Stokes (next level)
template <int Channels>
struct SpectralSample {
    v_f32_8 values[Channels];  // 8 spectral samples per channel
    float   heroLambda;        // λ_h ∈ [380, 380+Δλ); Δλ = 50 nm (= 400/8)
    // companions: λ_k = wrapToVisible(heroLambda + k·Δλ),  k = 0..7
};

using Spectrum = SpectralSample<1>;
```

Notes:

- **Width is parameterized once**. The enabler delivers a single `v_f32_8`-based carrier; on
  AVX2 builds it occupies one 256-bit register. SSE-only fallback is not currently planned
  in `implement-planned-vector-types.md`; AVX2 is the minimum target (`/arch:AVX2`).
  No `#if` at the spectral-call-site level.
- **Hero λ included in the container** as a scalar, per the storage policy. Total: 8 floats
    + 1 float = 36 B before alignment padding, 64 B aligned.
- **Wavelength range:** [380, 780] nm visible; Δλ = 50 nm → 8 samples cover the visible band
  uniformly when `λ_h = 380`; for arbitrary `λ_h ∈ [380, 430)` the 8 companions wrap
  cyclically around the band (Wilkie 2014 hero-wavelength sampling).
- **AoS / SoA layout for ray packets:** the `Raytracer` already runs ray packets of size
  `RAY_PACKET_SIZE`. Spectrum-per-ray inside a packet uses the existing SoA pattern: one
  `Spectrum` per ray, packets store `Spectrum[RAY_PACKET_SIZE]` contiguous. Transposed
  per-λ-across-rays variants are explicitly out of scope — defer until benchmarks justify.

## Scope of the enabler

### 1. Container type and basic ops (additive)

- `Spectrum` type as above with:
    - `zero()`, `constant(float)`, `monochromatic(λ, intensity)`
    - `operator+`, `operator-`, `operator*`, `operator*=` between two `Spectrum`s
    - `operator*` by scalar
    - `wavelengthsAt(heroLambda) -> v_f32_8` (computes the 8 λ values, wrapped)
    - No `Spectrum`-to-`RGBS` conversion yet — comes later in this enabler under §5.

### 2. Jakob-Hanika reflectance upsampling

- `raytracing/shading/spectral/jakob_hanika.h`:
    - 3D LUT generator: scans the RGB cube, fits 3 sigmoid-polynomial coefficients per
      RGB triple. Either bake offline once and ship as a binary blob in `data/`, or build
      at startup (~ms cost). Match Mitsuba 3's table dimensions (64³ resolution, three
      separate tables for the dominant-channel parametrization).
    - `evaluateReflectance(coeffs, wavelengthsV) -> v_f32_8` — evaluates
      `s(λ) = sigmoid(c₀ + c₁·λ + c₂·λ²)` across all 8 λ in one fused-multiply-add chain.
- Table assumes a D65 illuminant; document this and the implications for white balance.

### 3. Emissive upsampling and analytic light spectra

- `raytracing/shading/spectral/emissive.h`:
    - Meng et al. 2015 upsampling for unbounded RGB → smooth emission spectrum.
    - `blackbody(temperatureKelvin, wavelengthsV) -> v_f32_8` via Planck's law
      (this becomes the preferred way to specify lights post-migration).
    - Tabulated CIE illuminants D65, D50, A, F2 sampled at the 8 λ.

### 4. Metal IOR tables

- `raytracing/shading/spectral/metals.h`:
    - Tabulated complex refractive index `(n(λ), k(λ))` for Cu, Au, Al, Fe, Ag — sampled
      densely (e.g. 5 nm step) from refractiveindex.info or Polyanskiy's data.
    - `metalIOR(MetalId, wavelengthsV) -> { v_f32_8 n; v_f32_8 k; }`.
- Tests assert that `metalIOR(Au, [589 nm])` matches reference data within 1%.

### 5. CIE conversion utilities

- `raytracing/shading/spectral/cie.h`:
    - CIE 1931 2° observer color-matching functions x̄(λ), ȳ(λ), z̄(λ) tabulated at 1 nm.
    - `cmfAt(wavelengthsV) -> { v_f32_8 x; v_f32_8 y; v_f32_8 z; }`.
    - `spectrumToXYZ(Spectrum, heroLambda) -> Float3` — three dot products, accounting for
      the per-sample Δλ stride.
    - `xyzToDisplayRGB(xyz, displayPrimaries) -> RGBS` — reuses the matrices from
      `illumination-rgb-linear-pipeline-enabler.md` if it has landed; otherwise ships its
      own private copy of the sRGB-linear ↔ XYZ matrix and notes the duplication.

### 6. Tests

- `Spectrum` arithmetic correctness (commutative add/mul, distributivity within `eps`).
- Wavelength wrap correctness for hero λ at 380, 405, 429.999, 380+ε.
- Jakob-Hanika round-trip: pick reference RGB → upsample → integrate against CIE CMFs at
  D65 → compare back to the input RGB. Should match to within ~1% across the cube interior.
- `spectrumToXYZ(monochromatic(555 nm, 1.0))` should yield `(X, Y, Z)` close to
  `(x̄(555), ȳ(555), z̄(555))` within Δλ-quantization tolerance.

## Out of scope (defer to the implementation item)

- Replacing illumination types in the shader chain. The existing RGBS-based pipeline
  (SurfaceShading, LightShading, BRDFParameters, IlluminatedIntersection) stays untouched.
- Per-λ BRDF rewrites.
- Hero-λ sampling at primary-ray spawn.
- Dispersion handling on refractive surfaces.
- Texture pipeline conversion to Jakob-Hanika coefficients.

## Non-degradation guarantee

- Zero changes to existing call sites or data types on the hot path.
- No new transitive include from any file currently used by `Raytracer::trace`.
- Regression baseline (~0.07s on teapot, ray counters) bit-identical after this
  enabler lands.
- Binary size grows (LUTs, IOR tables, CMF tables — order of ~MB if all baked); no runtime
  cost unless the new APIs are called.

## Key files (new)

- `sources/raytracing/include/raytracing/shading/spectral/spectrum.h`
- `sources/raytracing/include/raytracing/shading/spectral/jakob_hanika.h`
- `sources/raytracing/include/raytracing/shading/spectral/emissive.h`
- `sources/raytracing/include/raytracing/shading/spectral/metals.h`
- `sources/raytracing/include/raytracing/shading/spectral/cie.h`
- `sources/raytracing/src/raytracing/shading/spectral/*.cpp`
- `data/spectral/jakob_hanika_lut.bin` (or generated at startup)
- `data/spectral/metal_iors.bin` (or compiled-in)
- `tests/raytracing.native-test/src/raytracing/shading/spectral/*_test.cpp`
