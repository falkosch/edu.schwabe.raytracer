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

| Constant              | Release       | Debug |
|-----------------------|---------------|-------|
| `FAST_PREVIEW_SIZE`   | 512x512       | 64x64 |
| `MAX_TRACE_DEPTH`     | 31            | 0     |
| `PERSPECTIVE_FOV`     | 45            | 45    |
| `RAY_PACKET_SIZE`     | 15            | 15    |
| `SUPER_SAMPLING`      | 0             | 0     |
| `CULLING_ORIENTATION` | -1 (backface) | -1    |

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
  falloff variants, ambient light, an emitter that doesn't light its surroundings
- `ShadingScene` (`shading`) -- the shader classes and texture transforms (environment map, sRGB vs. linear texture,
  normals as colors, tinted texture)
- `MaterialsScene` (`materials`) -- chart of metal roughness, every refraction index, plastics and metals, Perlin and
  Simplex noise by octaves
- `MeshesScene` (`meshes`) -- all meshes of `data/meshes` in a grid; loading takes about 10 s
- `ShowcaseScene` (`showcase`) -- studio demo: glass dragon, gold bunny, chrome sphere on marble, diamond

The new scenes are composed for the start camera (the dropdown doesn't move the camera) and build on
`SceneMaterial` (the seven material channels as constants, with factories) and `createSceneObject`.

## Command line

- `--scene=<id>` -- start with that scene instead of the Cornell box
- `--exit-after-render` -- close the window once the first image is rendered and saved as `ray-traced.png`, e.g.
  `raytracerui.exe --scene=showcase --exit-after-render` from `data/` to render a scene without interaction

## Controls

- **WASD/Space/C**: camera movement; **right-drag**: rotation
- **Q**: cycle culling; **E**: cycle display (image/timing/depth); **R**: reset camera
- **G/H**: ray packet size; **J/K**: super-sampling; **T**: toggle fast preview
- **1/2/3**: interaction mode (Camera/Object/Light); **Enter**: full-quality render
- **F1-F4**: toggle ImGui panels (Controls, Metrics, Config, Log)

## Image pipeline

HDR output (sRGB-linear from the spectral pipeline) → AgX tone map → sRGB encode → OpenGL texture.
Display modes: tone-mapped image (0), timing heatmap (1), depth map (2).

## Thread safety

Render results are marshaled from worker to UI thread via `pendingMutex` + `WM_RENDER_COMPLETE` message.
Scene mutations use `Raytracer::enqueueWork()` to serialize with rendering.
