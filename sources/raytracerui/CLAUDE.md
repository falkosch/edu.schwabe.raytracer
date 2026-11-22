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
| `CULLING_ORIENTATION`        | -1 (backface) | -1    |

## Scenes (`include/tests/`, `src/tests/`)

`SceneCatalog.cpp` lists every scene with its command-line id and dropdown label; the dropdown and `--scene=<id>`
both use it. Append new scenes there (and keep the order, the dropdown index follows it).

- `CornellBoxScene` (`cornell-box`) -- default; classic Cornell box with a glass sphere, noise-textured box, teapot
- `TestScene1` (`test-scene-1`) -- multi-object with various materials
- `TestScene2` (`test-scene-2`) -- plane and emissive sphere
- `DragonScene` (`dragon`) -- dragon mesh with marble texture
- `ProceduralScene<L,O>` (`procedural`) -- parametric grid of lights and objects
- `FormsScene` (`forms`) -- every form type with the checker texture: analytic and mesh sphere, ellipsoid, boxes,
  planes, low-poly mesh
- `LightingScene` (`lighting`) -- every emitter kind as light pools on a wall, RGB lights with colored shadows,
  falloff variants, ambient light, an orange sphere light with soft shadows
- `ShadingScene` (`shading`) -- the shader classes and texture transforms (St. Peter's light probe as angular map,
  sRGB vs. linear texture, normals as colors, tinted texture, checker times marble)
- `MaterialsScene` (`materials`) -- chart of metal roughness, every refraction index, plastics and metals, Perlin and
  Simplex noise by octaves
- `MeshesScene` (`meshes`) -- all meshes of `data/meshes` in a grid; loading takes about 10 s
- `ShowcaseScene` (`showcase`) -- studio demo: glass dragon, gold bunny, chrome sphere on marble, diamond, lit by a
  softbox (sphere light)

The new scenes are composed for the start camera (the dropdown doesn't move the camera) and build on
`SceneMaterial` (the seven material channels as constants, with factories), `createSceneObject` and `addSphereLight`
(an emissive sphere with its sphere light).

## Command line

- `--scene=<id>` -- start with that scene instead of the Cornell box
- `--exit-after-render` -- close the window once the first image is rendered and saved as `ray-traced.png`, e.g.
  `raytracerui.exe --scene=showcase --exit-after-render` from `data/` to render a scene without interaction. Failed
  asserts then print to stderr instead of opening a dialog
- `--full-render` -- the first image is a full render (like Enter) instead of a fast preview
- `--exposure=<stops>` -- scale the displayed image by 2^stops (also a slider in the Config panel)
- `--max-trace-depth=<n>` -- replace `MAX_TRACE_DEPTH`, e.g. to trace reflections in a debug build
- `--glossy-samples=<n>`, `--sphere-light-samples=<n>` -- replace `GLOSSY_SAMPLES` and `SPHERE_LIGHT_SAMPLES`, the most
  reflection rays per glossy hit and shadow rays per sphere light (distribution ray tracing; also sliders in the Config
  panel). 0 for both is Whitted ray tracing: mirror reflections and hard shadows

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
