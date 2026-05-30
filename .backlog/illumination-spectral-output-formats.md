# 8-channel spectral output formats (.pfm / .exr)

## Sequencing

- **Type:** extension
- **Level:** 2 (unpolarized spectral)
- **Depends on:** Level 2 unpolarized spectral pipeline (shipped)
- **Blocks:** nothing

## Motivation

Today the renderer projects every pixel's accumulated `Spectrum` to sRGB-linear via
`spectrumToXYZ → xyzToDisplayRGB`, then runs the AgX view transform, then writes 8-bit
sRGB to disk (PNG / BMP). All spectral information is collapsed to three channels at
output time.

For uses where preserving full spectral fidelity matters — re-grading under different
illuminants, spectral comparison against reference renderers, scientific visualization,
post-process color science experiments — the current path is lossy in a way that can't
be recovered. A wide-channel output format (8 channels for unpolarized, or 32 for
polarized Stokes at level 3) keeps the spectral information available downstream.

The original unpolarized-spectral implementation flagged this as
*"explicit non-goal in this item, log a follow-up if requested."* This is that follow-up.

## Scope

Add an alternative output path that writes the raw `Spectrum` per pixel before the
spectrum → XYZ → sRGB collapse, plus a corresponding file-format writer.

### Format choice

- **`.pfm` (Portable Float Map):** simple ASCII header + raw little-endian float32
  payload. The PFM spec supports 1-channel (grayscale) and 3-channel (RGB) variants;
  there is no standard for arbitrary N channels, but the format generalizes naturally
  by reading the header as N-channel float32 raster. Most viewers won't display
  N-channel PFM, but tools like Python/numpy can load it trivially. Recommended for
  the renderer-internal debugging use case.
- **`.exr` (OpenEXR):** the industry standard for multi-channel HDR. Supports
  arbitrary channel names, per-channel float32/half data, and is read by every DCC
  tool. Requires a `libopenexr` dependency (vcpkg has it). Recommended if interop
  with external tools matters.

Pick PFM first (zero external dependency, drop-in writer). EXR if/when an interop need
arises — file a separate item then.

### Implementation outline

1. Add `sources/raytracing/include/raytracing/common/SpectralPFMWriter.h` mirroring
   `PNGWriter.h`'s shape — `saveSpectralPFM(const HDRImage& spectralImage, const path&)`.
2. Extend the render loop to capture the per-pixel `Spectrum` (8-channel) into a
   parallel buffer, alongside the existing 3-channel sRGB-linear `HDRImage`.
3. Surface a UI toggle / hotkey (or CLI flag in `main.cpp`) to save the spectral PFM
   on demand instead of always paying the storage cost.

### Storage

`Spectrum.data` is a `Float8` (32 bytes). For a 512×512 image: 512·512·32 = 8 MB per
spectral frame. Negligible compared to render time; no need for compression in the
PFM path.

## Acceptance criteria

- `saveSpectralPFM` writes a file readable by Python/numpy as a `[height, width, 8]`
  float32 array.
- Loaded array's per-pixel 8-channel values, integrated against the same CMF tables the
  renderer uses, match the displayed sRGB output to within numerical tolerance.

## Out of scope

- OpenEXR support (file a follow-up if needed).
- Stokes-vector (polarized) output — that's level 3, file under
  `illumination-polarized-spectral.md` follow-ups.
- A spectral viewer / falsecolor visualization tool. Standard image viewers handle the
  sRGB PNG output already.

## Key files

- `sources/raytracing/include/raytracing/common/SpectralPFMWriter.h` (new)
- `sources/raytracing/src/raytracing/common/SpectralPFMWriter.cpp` (new)
- `sources/raytracing/include/raytracing/RaytraceConfiguration.h` — optional spectral
  output buffer pointer alongside the existing image/depthMap/timingMap
- `sources/raytracing/src/raytracing/Raytracer.cpp` — capture the per-pixel Spectrum
  into the spectral buffer (gated by a configuration flag) before projecting to RGB
- `sources/raytracerui/src/main.cpp` or UI input layer — toggle/hotkey to save the
  spectral PFM
