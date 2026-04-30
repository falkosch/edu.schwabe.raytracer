# Implementation: switch RGB pipeline to ACEScg-linear with GGX and a view transform

## Sequencing

- **Type:** implementation
- **Level:** 1.5 (linear-RGB cleanup)
- **Order:** 1
- **Depends on:** `illumination-rgbs-type.md`, `illumination-rgb-linear-pipeline-enabler.md`
- **Blocks:** *(none — independent branch from the spectral track)*

## Motivation

`illumination-rgb-linear-pipeline-enabler.md` ships ACEScg primaries, AgX/ACES view transforms,
and energy-conserving GGX as new utilities. This item flips them on: declare ACEScg-linear as
the working space, replace Phong with GGX in the default material, and apply a view transform
at framebuffer write.

This is a defensible end-state on its own — no spectral migration required — and is the cheapest
way to remove the "CG look" from the current renderer.

## Scope

### 1. Working color space declaration

- `RGBS` is reinterpreted as ACEScg-linear (no struct/layout change, only semantics).
- HDR environment maps are sampled assuming Rec.709-linear input and converted on load to
  ACEScg via the matrix from the enabler. Note: no `.hdr` / Radiance RGBE loader exists;
  the current pipeline loads PPM via `Bitmap::loadPPM` and constructs `HDRImage` from it.
  Adding an `.hdr` loader is a prerequisite or must be scoped into this item.
- 8-bit textures (PPM — the only input format with a loader) are decoded from sRGB via
  `srgbDecode`, then converted to ACEScg. Procedural shaders (e.g. `NoiseGeneratorMap`)
  output values directly and must not receive sRGB decode. This is the largest behavior
  change and **will shift colors of every existing test scene**.

### 2. Default BRDF: GGX replaces Phong

- `Raytracer::applyBRDF` now calls `evaluateGGX` for the specular lobe.
- Material gets a roughness parameter `α` (0.04 default — close to current Phong shininess,
  to be tuned per scene).
- Schlick-Fresnel `F0` already present; reuse it.
- The existing Phong BRDF function remains available (not deleted) so the regression baseline
  scene can still produce the documented `secondaryRays`/`shadowRays` counters. Existing
  Shaders referencing Phong via `SceneShader::phongSpecularIntensityPerReflectedIncident`
  keep working; new scenes default to GGX via a new GGX shader or by updating existing
  specular shaders.

### 3. Framebuffer view transform

- The `Bitmap(const Image<ImageVectorType>&)` template constructor in `Bitmap.h` currently
  does a linear clamp-and-scale to 8-bit. Insert the view transform (AgX → `srgbEncode`)
  before quantization, either by specializing for `Float4` or extracting a non-template
  helper. The constructor is called from `RaytracerUI::notifyUpdate`.
- HDRImage output stays linear ACEScg (no view transform), to preserve full information.
- **OpenGL display path:** `OpenGLWindowsRaytracerUI::onRenderComplete` uploads the HDRImage
  directly as `GL_RGBA32F`. Either apply the view transform in a GLSL shader or accept that
  the on-screen preview differs from saved Bitmaps. Document the chosen approach.

### 4. Drop the shared `S` scale field

- `RGBS` collapses to 12 B (R, G, B). With `float32` throughout, dynamic range is
  ~10³⁸ — `S` is solving a problem that no longer exists.
- All existing arithmetic on `RGBS` simplifies: drop the per-op `S` propagation.

### 5. Test-scene recalibration

Every predefined scene needs light-intensity tuning under the new tonemap. Scene definitions
live in separate files under `sources/raytracerui/src/tests/`:
`CornellBoxScene.cpp`, `DragonScene.cpp`, `TestScene1.cpp`, `TestScene2.cpp`,
`TestLightScene.cpp` (shared lighting preamble used by TestScene1/2 and DragonScene — not
standalone), and `ProceduralScene.h` (header-only template). `main.cpp` only selects which
scene to invoke. Document the old vs. new values in scene source comments so the change is
auditable.

## Acceptance criteria

- All `vectorization.native-test` and `raytracing.native-test` cases pass.
- Regression baseline runs without crash; ray counters remain in the same order of magnitude
  (exact match not required because GGX changes light-transport magnitudes).
- Visual check: CornellBox shows recognizable color bleed; specular highlights look like
  microfacet (long anisotropic-ish tails on grazing angles), not the current Phong cosine
  lobe.
- If the `S` scale field is dropped: `RGBS` carries 3× float (12 B payload) but
  `sizeof(RGBS)` remains 16 B due to SSE alignment padding (W lane unused). Test asserts
  `sizeof(RGBS) == 16` and that the W component is always zero after construction.
  If `S` is retained (e.g. as a luminance-preserving factor under tonemapping), document why.

## Out of scope

- Anything spectral. Wavelength-dependent IOR, real metals, dispersion — handled later.
- Path tracing / multi-bounce indirect — Whitted topology is preserved.
- ACES vs. AgX choice as runtime config; pick one (recommend AgX) and ship it.

## Risk / fallback

- If recalibrating every scene proves too disruptive, ship behind a compile-time toggle
  (`RAYTRACER_LINEAR_PIPELINE`, default off) for one release, then flip and remove the
  toggle in the next.

## Key files (modified)

- `sources/raytracing/include/raytracing/common/RGBS.h` — drop `S` (or document retention)
- `sources/raytracing/src/raytracing/Raytracer.cpp` — `applyBRDF`
- `sources/raytracing/include/raytracing/common/Bitmap.h` — view transform in templated
  Bitmap constructor (HDR → 8-bit conversion); note this is a header template, not `.cpp`
- `sources/raytracing/src/raytracing/common/HDRImage.cpp` — sRGB decode in
  `HDRImage(const Bitmap&)` constructor (8-bit → float conversion)
- `sources/raytracing/src/raytracing/common/Bitmap.cpp` — PPM load path
- `sources/raytracerui/src/tests/CornellBoxScene.cpp` — scene recalibration
- `sources/raytracerui/src/tests/DragonScene.cpp` — scene recalibration
- `sources/raytracerui/src/tests/TestScene1.cpp` — scene recalibration
- `sources/raytracerui/src/tests/TestScene2.cpp` — scene recalibration
- `sources/raytracerui/src/tests/TestLightScene.cpp` — shared lighting recalibration
- `sources/raytracerui/include/tests/ProceduralScene.h` — scene recalibration
- `sources/raytracing/include/raytracing/shading/SurfaceShading.h` — add roughness field
- `sources/raytracing/include/raytracing/shading/ObjectShader.h` — add roughness shader slot
- `sources/raytracing/src/raytracing/shading/ObjectShader.cpp` — roughness shader wiring
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` — swap Phong specular → GGX
- `sources/raytracerui/src/RaytracerUI.cpp` — OpenGL display path (needs view transform
  parity with Bitmap path, or document the discrepancy)
