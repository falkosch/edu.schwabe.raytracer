# Non-thermal emitter SPDs

## Sequencing

- **Type:** extension
- **Level:** 2 (unpolarized spectral)
- **Depends on:** Level 2 unpolarized spectral pipeline (shipped — `LightInfo` already dispatches over `EmitterKind`)
- **Blocks:** nothing

## Motivation

`EmitterKind` currently covers thermal emitters (`Blackbody`, `D65`, `IlluminantA`) plus the
generic `RGB` Smits fallback. Thermal emitters can only produce chromaticities on the
Planckian locus — they cannot represent saturated green, magenta, cyan, or purple light.
Real-world non-thermal sources cover those gaps:

- **Gas discharge** (sodium ~589nm orange, mercury 405/436/546/578nm violet-green mix,
  neon multiple red-orange lines) — line spectra
- **LEDs** — narrow-band gaussian emission around a peak wavelength (e.g. green LED ≈ 530nm)
- **Lasers** — single-wavelength monochromatic emission
- **Fluorescent** — broad phosphor SPD with characteristic peaks
- **Filtered illuminants** — broad source × absorption spectrum (D65 through a colored gel)

Today the only way to get a saturated green or magenta light is `EmitterKind::RGB`, which is
art-directed: a tint without spectral meaning. Adding physical non-thermal SPDs unlocks
spectrally-correct rendering of LEDs, sodium-vapor street lights, neon signs, laser pointers,
and stage lighting.

## Scope

Add new `EmitterKind` values and the corresponding SPD evaluators in
`sources/raytracing/include/raytracing/shading/spectral/`:

1. **`NarrowBand`** — gaussian SPD `exp(-((λ - peakλ)/σ)²)` parametrized by peak wavelength
   and bandwidth σ. Covers LEDs and (with very small σ) lasers. Single dispatch case.

2. **`LineSpectrum`** — sum of up to N delta-like gaussians at fixed wavelengths with
   per-line intensity weights. Covers sodium (one strong line), mercury (3-4 lines), neon
   (many lines, may need to cap N or split into multiple light entries). Data tables for
   common gas discharges (Hg, Na, Ne) compiled in.

3. **`Fluorescent`** — tabulated SPD for common phosphor mixes (CIE F2 cool-white,
   F7 daylight, F11 narrow-band tri-phosphor). Same lookup pattern as `D65`/`IlluminantA`.

4. **Filtered emission** — extend `LightInfo` so any spectral emitter can carry an optional
   RGB tint that is Smits-upsampled and multiplied in. Cheaper than a new `EmitterKind`;
   composes orthogonally. Example: filtered D65 = D65 × Smits(RGB tint).

`LightInfo` factory functions:

```cpp
static LightInfo led(Float peakWavelengthNm, Float bandwidthNm, Float brightness);
static LightInfo laser(Float wavelengthNm, Float brightness);
static LightInfo sodiumVapor(Float brightness);
static LightInfo mercuryVapor(Float brightness);
static LightInfo neonTube(Float brightness);
static LightInfo fluorescent(FluorescentKind kind, Float brightness);
```

All photopic-luminance-normalized like the existing factories: `brightness=X` produces
spectrum Y = X regardless of color.

## Tests

- LED at 530nm + bandwidth 20nm → spectrumToXYZ → xyzToDisplayRGB ≈ saturated green
- Sodium at 589nm with σ → 0 → near-monochromatic orange (matches the sodium-lamp
  acceptance criterion from the original unpolarized-spectral implementation)
- Mercury 4-line → green-tinged cool white, characteristic mercury chromaticity
- Photopic normalization: two emitters at different peak wavelengths but same `brightness`
  produce the same XYZ Y component

## Out of scope

- Light source geometry (area lights). All emitters here are point sources via
  `LightInfo::position`.
- Time-varying SPDs (flicker, lightning, candle).
- Wavelength-dependent angular emission patterns.

## Key files

- `sources/raytracing/include/raytracing/shading/spectral/line_spectrum.h` (new)
- `sources/raytracing/include/raytracing/shading/spectral/narrow_band.h` (new)
- `sources/raytracing/include/raytracing/shading/spectral/fluorescent.h` (new)
- `sources/raytracing/include/raytracing/shading/LightInfo.h` — new `EmitterKind` values,
  optional tint field, new factory functions
- `sources/raytracing/src/raytracing/shading/LightInfo.cpp` — factory implementations
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` — `sampleEmitter` dispatch
  extended with new cases
- `tests/raytracing.native-test/src/raytracing/shading/spectral/` — corresponding tests
