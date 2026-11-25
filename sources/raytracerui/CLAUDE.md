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
| `DIFFUSE_SAMPLES`            | 16            | 16    |
| `MANY_LIGHT_SAMPLES`         | 16            | 16    |
| `GLOSSY_CLAMP`               | 4             | 4     |
| `ADAPTIVE_ROUNDS`            | 3             | 3     |
| `ADAPTIVE_THRESHOLD`         | 2/255         | 2/255 |
| `IRRADIANCE_CACHING`         | on            | on    |
| `DENOISE`                    | on            | on    |
| `FULL_RENDER_DENOISE`        | off           | off   |
| `CULLING_ORIENTATION`        | -1 (backface) | -1    |

## Scenes (`include/tests/`, `src/tests/`)

`SceneCatalog.cpp` lists every scene with its command-line id and dropdown label; the dropdown and `--scene=<id>`
both use it. Append new scenes there (and keep the order, the dropdown index follows it).

- `CornellBoxScene` (`cornell-box`) -- default; Cornell box lit by a warm white box light set into the ceiling (bottom
  face only, luminance 3 pi) and its interreflection (diffuse rays; without them the shadows and the ceiling are
  black), with a mirror and a rough mirror sphere, a noise-textured box and a glass teapot
- `TestScene1` (`test-scene-1`) -- multi-object with various materials
- `TestScene2` (`test-scene-2`) -- plane and emissive sphere
- `DragonScene` (`dragon`) -- dragon mesh with marble texture
- `ProceduralScene<L,O>` (`procedural`) -- parametric grid of lights and objects
- `FormsScene` (`forms`) -- every form type with the checker texture: analytic and mesh sphere, ellipsoid, boxes,
  planes, low-poly mesh
- `LightingScene` (`lighting`) -- every emitter kind as light pools on a wall, RGB lights with colored shadows, an
  orange sphere light with soft shadows, a cyan light bar (a box light glowing on all faces)
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
faces). Their lights' intensities and radiances carry a factor pi: a white surface facing a light reflects its
irradiance / pi (Lambert), so a light of pi lights it to 1. Point and sphere lights fall off as the inverse square of
the distance; the Forms and Meshes scenes' key and fill are directional lights (`LightInfo::direction`, no falloff). No
scene sets an ambient light (all 0 since 2026-11-24, until the object explorer sets it per scene,
`.backlog/object-explorer.md`): the library's stand-in for the interreflection where no diffuse rays gather it
(everywhere in Whitted ray tracing, at the hits of diffuse rays the further bounces), so without diffuse rays, what no
light reaches stays black.

## Command line

- `--scene=<id>` -- start with that scene instead of the Cornell box
- `--exit-after-render` -- render without showing the window (it stays hidden) and exit once the first image is
  saved as `ray-traced.png`, e.g. `raytracerui.exe --scene=showcase --exit-after-render` from `data/` to render a
  scene without interaction. Failed asserts then print to stderr instead of opening a dialog
- `--full-render` -- the first image is a full render (like Enter) instead of a fast preview
- `--exposure=<stops>` -- scale the displayed image by 2^stops (also a slider in the Config panel)
- `--max-trace-depth=<n>` -- replace `MAX_TRACE_DEPTH`, e.g. to trace reflections in a debug build
- `--glossy-samples=<n>`, `--sphere-light-samples=<n>` -- replace `GLOSSY_SAMPLES` and `SPHERE_LIGHT_SAMPLES`, the most
  reflection rays per glossy hit and shadow rays per area light, sphere or box (distribution ray tracing; also sliders
  in the Config panel). 0 for both and `--diffuse-samples=0` is Whitted ray tracing: mirror reflections and hard
  shadows
- `--diffuse-samples=<n>` -- replace `DIFFUSE_SAMPLES`, the most rays a diffuse hit gathers the light of other surfaces
  with (one bounce of interreflection, in place of the ambient light; 0 for none; a Config slider)
- `--many-light-samples=<n>` -- replace `MANY_LIGHT_SAMPLES`, the most point lights a diffuse ray's hit traces shadow
  rays to: with more, it picks that many by their light (the Procedural scene's 343; 0 for all; a Config slider)
- `--irradiance-caching=<0|1>` -- replace `IRRADIANCE_CACHING`: diffuse hits interpolate records of the irradiance
  instead of tracing diffuse rays (faster where diffuse rays dominate and smoother; a Config checkbox)
- `--denoise=<0|1>`, `--full-render-denoise=<0|1>` -- replace `DENOISE` and `FULL_RENDER_DENOISE`: denoise fast
  previews and full renders, guided by the normals, depths and albedos of the pixels' surfaces (full renders would
  lose glossy detail; Config checkboxes). Whitted ray tracing isn't denoised
- `--glossy-clamp=<x>` -- replace `GLOSSY_CLAMP`, the most radiance one glossy reflection ray or diffuse ray adds
  (against fireflies; 0 for no limit; a Config slider)
- `--regularize-paths` -- after a glossy reflection or a diffuse ray, mirrors and glass don't show area lights' emissive
  objects either (less glitter and fewer caustic speckles, but that light goes missing; a Config checkbox)
- `--super-sampling=<n>` -- replace `SUPER_SAMPLING_FACTOR`, (n+1)x(n+1) samples per pixel (a full render takes at
  least `FULL_RENDER_SUPER_SAMPLING`), e.g. `--full-render --super-sampling=7 --irradiance-caching=0` for a reference
  image
- `--seed=<n>` -- another sample pattern of the distribution ray tracing (0 is the default): renders with two seeds
  measure the noise
- `--visibility-cutoff=<x>` -- replace `VISIBILITY_CUTOFF` (1/512), the least share of the pixel for which rays and
  lights are still traced; smaller values measure what the cutoff loses
- `--adaptive-rounds=<n>`, `--adaptive-threshold=<x>` -- replace `ADAPTIVE_ROUNDS` and `ADAPTIVE_THRESHOLD`: a pixel
  of at least 2 x 2 sub-samples (a full render) takes up to n further rounds of sub-samples where the standard error
  of its neighbours' means exceeds x in display values (1/255 is about one 8-bit step), as many as bring it below x;
  0 rounds is off (also sliders in the Config panel, the threshold in 8-bit steps). Whitted ray tracing doesn't
  sample adaptively

## Controls

- **WASD/Space/C**: camera movement; **right-drag**: rotation
- **Q**: cycle culling; **E**: cycle display (image/timing/depth); **R**: reset camera
- **G/H**: ray packet size; **J/K**: super-sampling; **T**: toggle fast preview
- **1/2/3**: interaction mode (Camera/Object/Light); **Enter**: full-quality render (at least
  `FULL_RENDER_SUPER_SAMPLING`)
- **F1-F4**: toggle ImGui panels (Controls, Metrics, Config, Log)

## Image pipeline

HDR output (sRGB-linear from the spectral pipeline, denoised by the raytracer where `DENOISE` asks) → exposure → AgX
tone map (its output is already encoded for the display) → OpenGL texture.
Display modes: tone-mapped image (0), timing heatmap (1), depth map (2).

## Thread safety

Render results are marshaled from worker to UI thread via `pendingMutex` + `WM_RENDER_COMPLETE` message.
Scene mutations use `Raytracer::enqueueWork()` to serialize with rendering.
