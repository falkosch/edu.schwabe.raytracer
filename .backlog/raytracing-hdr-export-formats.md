# HDR image export: Radiance .hdr, OpenEXR and the UI

## Motivation

`HDRImage` reads and writes only Portable Float Maps (`saveAsPFM`, `loadPFM`; `DONE/raytracing-testability.md`, 3).
PFM is exact and needs no library, but few image tools open it. The formats other tools read are Radiance `.hdr`
(RGBE) and OpenEXR. The renderer doesn't export HDR at all yet: the UI saves PNG and BMP only, both 8-bit and after
the AgX view transform.

Goal: save the rendered image scene-linear in the common HDR formats, from the library and from the UI.

## Scope

### 1. Radiance `.hdr`

- `HDRImage::saveAsHDR` and `HDRImage::loadHDR`: the header (`#?RADIANCE`, `FORMAT=32-bit_rle_rgbe`, a blank line,
  `-Y <height> +X <width>`), then RGBE pixels: 8-bit mantissas sharing an 8-bit exponent, about 1% precision.
- The file stores the top row first, `HDRImage` the bottom row first: flip on both sides.
- Writing may store flat scanlines; reading must also decode the run-length encoded scanlines (widths 8 to 32767)
  that other tools write, and the old-style RLE if it's cheap.
- No library needed.

### 2. OpenEXR

- `HDRImage::saveAsEXR` and `HDRImage::loadEXR`, RGB as half or float channels (pick one, or a parameter), with a
  lossless compression (ZIP or PIZ).
- Needs a library: the `openexr` vcpkg port (it throws exceptions, so the target that wraps it opts in like mesh
  loading does, see the root `CLAUDE.md`) or `tinyexr` (single header, no exceptions, `miniz` or `zlib`). Add it to
  `vcpkg.json` for the `x64-windows-static` triplet and keep it in one source file, like WIC in `save_png.cpp`.

### 3. Export from the UI

- The save dialog (`OpenGLWindowsRaytracerUI.cpp`) offers PFM, `.hdr` and `.exr` next to PNG and BMP and picks the
  writer by extension.
- The HDR formats save `RaytraceConfiguration::image` (sRGB-linear), not `RaytracerUI::outputHDR`: for the image
  view that one is replaced by the AgX display image. Decide whether the exposure applies (probably not: the file
  stays scene-referred, a viewer applies its own).
- Maybe a command-line option next to `--exit-after-render` (e.g. `--save-hdr=<file>`), so test renders can be
  compared in float.

## Tests

- Round-trips through each format: exact for EXR float channels, within the format's precision for RGBE and half
  floats (values beyond [0, 1], tiny and huge ones, black).
- `.hdr`: a hand-written RLE scanline decodes; the row order is flipped; broken and truncated files are rejected.
- EXR: a file written by the library reads back with the expected channel order and row order.

## Key files

- `sources/raytracing/include/raytracing/common/HDRImage.h`, `sources/raytracing/src/raytracing/common/HDRImage.cpp`
- `sources/raytracing/CMakeLists.txt`, `vcpkg.json` (OpenEXR)
- `sources/raytracerui/src/OpenGLWindowsRaytracerUI.cpp` (save dialog), `sources/raytracerui/src/RaytracerUI.cpp`
- `tests/raytracing.native-test/src/raytracing/common/HDRImage_test.cpp`

## Related backlog items

- `illumination-spectral-output-formats.md`: writes the 8 spectral channels as PFM and leaves multi-channel OpenEXR
  as a follow-up; an EXR writer from here could take more channels later.
- `DONE/raytracing-testability.md` (3): the PFM I/O this item builds on.
