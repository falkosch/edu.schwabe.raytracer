# Raytracer UI

Win32+OpenGL+ImGui executable. Depends on `raytracing` and `logging`.

## Class hierarchy

```
MessageLoopBasedUI (abstract, Windows message loop)
    └── RaytracerUI (abstract, rendering parameters + observer pattern)
            └── WindowsRaytracerUI (Win32 window, keyboard/mouse input)
                    └── OpenGLWindowsRaytracerUI (OpenGL texture display, ImGui overlay)
```

## Configuration (main.cpp)

| Constant                     | Release       | Debug |
|------------------------------|---------------|-------|
| `FAST_PREVIEW_SIZE`          | 512x512       | 64x64 |
| `MAX_TRACE_DEPTH`            | 31            | 0     |
| `PERSPECTIVE_FOV`            | 45            | 45    |
| `RAY_PACKET_SIZE`            | 15            | 15    |
| `SUPER_SAMPLING`             | 0             | 0     |
| `FULL_RENDER_SUPER_SAMPLING` | 2 (3x3)       | 0     |
| `GLOSSY_SAMPLES`             | 16            | 16    |
| `SPHERE_LIGHT_SAMPLES`       | 16            | 16    |
| `GLOSSY_CLAMP`               | 4             | 4     |
| `ADAPTIVE_ROUNDS`            | 3             | 3     |
| `ADAPTIVE_THRESHOLD`         | 4/255         | 4/255 |
| `CULLING_ORIENTATION`        | -1 (backface) | -1    |

## Scenes (`include/tests/`, `src/tests/`)

`SceneCatalog.cpp` lists every scene with its command-line id and dropdown label; the dropdown and `--scene=<id>`
both use it. Append new scenes there (and keep the order, the dropdown index follows it).

- `CornellBoxScene` (`cornell-box`) -- default; Cornell box lit by a warm white box light set into the ceiling (bottom
  face only, luminance 3) and an ambient light standing in for the interreflection, with a mirror and a rough mirror
  sphere, a noise-textured box and a glass teapot
- `TestScene1` (`test-scene-1`) -- multi-object with various materials
- `TestScene2` (`test-scene-2`) -- plane and emissive sphere
- `DragonScene` (`dragon`) -- dragon mesh with marble texture
- `ProceduralScene<L,O>` (`procedural`) -- parametric grid of lights and objects
- `FormsScene` (`forms`) -- every form type with the checker texture: analytic and mesh sphere, ellipsoid, boxes,
  planes, low-poly mesh
- `LightingScene` (`lighting`) -- every emitter kind as light pools on a wall, RGB lights with colored shadows,
  falloff variants, ambient light, an orange sphere light with soft shadows, a cyan light bar (a box light glowing on
  all faces)
- `ShadingScene` (`shading`) -- the shader classes and texture transforms (ESO's Paranal night sky as all-sky fisheye,
  sRGB vs. linear texture, normals as colors, tinted texture, checker times marble), lit by a point key light (hard
  shadows), a rectangular softbox opposite it (a box light: soft shadows), a sphere light on the floor and a fill light
- `MaterialsScene` (`materials`) -- chart of metal roughness, every refraction index, plastics and metals, Perlin and
  Simplex noise by octaves
- `MeshesScene` (`meshes`) -- all meshes of `data/meshes` in a grid; loading takes about 10 s
- `ShowcaseScene` (`showcase`) -- studio demo: glass dragon, gold bunny, chrome sphere on marble, diamond, lit by a
  round softbox (a sphere light) and a rectangular fill softbox (a box light emitting from its front face)

The new scenes are composed for the start camera (the dropdown doesn't move the camera) and build on
`SceneMaterial` (the seven material channels as constants, with factories), `createSceneObject`, `addSphereLight`
(an emissive sphere with its sphere light) and `addBoxLight` (an emissive box with its box light and its emitting
faces).

## Command line

- `--scene=<id>` -- start with that scene instead of the Cornell box
- `--exit-after-render` -- close the window once the first image is rendered and saved as `ray-traced.png`, e.g.
  `raytracerui.exe --scene=showcase --exit-after-render` from `data/` to render a scene without interaction. Failed
  asserts then print to stderr instead of opening a dialog
- `--full-render` -- the first image is a full render (like Enter) instead of a fast preview
- `--exposure=<stops>` -- scale the displayed image by 2^stops (also a slider in the Config panel)
- `--max-trace-depth=<n>` -- replace `MAX_TRACE_DEPTH`, e.g. to trace reflections in a debug build
- `--glossy-samples=<n>`, `--sphere-light-samples=<n>` -- replace `GLOSSY_SAMPLES` and `SPHERE_LIGHT_SAMPLES`, the most
  reflection rays per glossy hit and shadow rays per area light, sphere or box (distribution ray tracing; also sliders
  in the Config panel). 0 for both is Whitted ray tracing: mirror reflections and hard shadows
- `--glossy-clamp=<x>` -- replace `GLOSSY_CLAMP`, the most radiance one glossy reflection ray adds (against fireflies;
  0 for no limit; a Config slider)
- `--regularize-paths` -- after a glossy reflection, mirrors and glass don't show area lights' emissive objects either
  (less glitter, but that light goes missing; a Config checkbox)
- `--super-sampling=<n>` -- replace `SUPER_SAMPLING_FACTOR`, (n+1)x(n+1) samples per pixel (a full render takes at
  least `FULL_RENDER_SUPER_SAMPLING`), e.g. `--full-render --super-sampling=7` for a reference image
- `--seed=<n>` -- another sample pattern of the distribution ray tracing (0 is the default): renders with two seeds
  measure the noise
- `--visibility-cutoff=<x>` -- replace `VISIBILITY_CUTOFF` (1/512), the least share of the pixel for which rays and
  lights are still traced; smaller values measure what the cutoff loses
- `--adaptive-rounds=<n>`, `--adaptive-threshold=<x>` -- replace `ADAPTIVE_ROUNDS` and `ADAPTIVE_THRESHOLD`: a pixel
  of at least 2 x 2 sub-samples (a full render) takes up to n further rounds of sub-samples while the standard error
  of their mean exceeds x in display values (1/255 is about one 8-bit step); 0 rounds is off (also sliders in the
  Config panel, the threshold in 8-bit steps). Whitted ray tracing doesn't sample adaptively

## Controls

- **WASD/Space/C**: camera movement; **right-drag**: rotation
- **Q**: cycle culling; **E**: cycle display (image/timing/depth); **R**: reset camera
- **G/H**: ray packet size; **J/K**: super-sampling; **T**: toggle fast preview
- **1/2/3**: interaction mode (Camera/Object/Light); **Enter**: full-quality render (at least
  `FULL_RENDER_SUPER_SAMPLING`)
- **F1-F4**: toggle ImGui panels (Controls, Metrics, Config, Log)

## Image pipeline

HDR output (sRGB-linear from the spectral pipeline) → exposure → AgX tone map (its output is already encoded for the
display) → OpenGL texture.
Display modes: tone-mapped image (0), timing heatmap (1), depth map (2).

## Thread safety

Render results are marshaled from worker to UI thread via `pendingMutex` + `WM_RENDER_COMPLETE` message.
Scene mutations use `Raytracer::enqueueWork()` to serialize with rendering.
