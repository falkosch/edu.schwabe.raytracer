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

- `CornellBoxScene` -- default; classic Cornell box with a glass sphere, noise-textured box, teapot mesh
- `DragonScene` -- dragon mesh with marble texture
- `TestScene1` -- multi-object with various materials
- `TestScene2` -- plane and emissive sphere
- `ProceduralScene<L,O>` -- parametric grid of lights and objects

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
