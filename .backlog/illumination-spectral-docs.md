# Spectral pipeline documentation polish

## Sequencing

- **Type:** documentation
- **Level:** 2 (unpolarized spectral)
- **Depends on:** none (purely additive doc/comment edits)
- **Blocks:** nothing

## Motivation

Two acceptance items from the original unpolarized-spectral implementation are
documentation notes that never got written:

1. **`superSamplingFactor` variance hint.** Hero-wavelength sampling (Wilkie 2014) trades
   spectral coverage for sample variance: each primary ray covers all 8 wavelengths but
   "samples" only one stratum of the spectral domain. To match the perceptual smoothness
   of the pre-spectral RGB pipeline, users need to bump `superSamplingFactor` (typically
   one step up — e.g. 0→1 doubles per-pixel samples). Today nothing in `main.cpp`,
   `raytracerui/CLAUDE.md`, or the panel UI explains this.

2. **HDR environment map approximation note.** `EnvironmentShader::sample()` returns RGB
   sampled from an HDR image; that RGB gets Smits-upsampled to a Spectrum at the
   `ObjectShader::shade()` boundary. For authored materials this is the canonical path,
   but for HDR environment captures the underlying *true* spectrum of the lighting
   environment is unknown — the upsampling fabricates a plausible spectrum that matches
   the integrated tristimulus. Worth a one-line code comment noting the approximation so
   readers don't assume spectral fidelity for env-map illumination.

## Scope

### 1. `superSamplingFactor` variance note

Add a short paragraph to `sources/raytracerui/CLAUDE.md` (in the Configuration table or
just below it) explaining:

- Spectral rendering samples one hero λ per primary ray and varies it across pixel
  sub-samples (stratified across `superSamplingFactor` and the 8 wavelengths).
- For smoother output, raise `SUPER_SAMPLING` (or use `J`/`K` keys at runtime) by one
  step compared to the equivalent RGB scene.

### 2. Env-map upsampling note

Add a brief comment (1–3 lines) in `sources/raytracing/src/raytracing/shading/shaders/EnvironmentShader.cpp`
or the matching header explaining that the returned RGB radiance is Smits-upsampled at
the shading boundary, that this is an approximation of the captured environment's true
spectrum, and that the integrated tristimulus matches by construction.

## Out of scope

- API or behavior changes — purely doc edits.
- Per-wavelength env-map support (would require multi-channel HDR formats; see
  `illumination-spectral-output-formats.md` for the related output-side concern).

## Key files

- `sources/raytracerui/CLAUDE.md` — supersampling-variance note
- `sources/raytracing/src/raytracing/shading/shaders/EnvironmentShader.cpp` — env-map
  upsampling comment
