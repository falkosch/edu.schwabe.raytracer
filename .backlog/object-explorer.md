# Object explorer

## Motivation

The UI shows only counts of a scene's content (Metrics panel, "Scene Info": objects, lights, faces, KD-tree). Lights
are tuned by editing the scene files and rebuilding: the light and shading scenes' balance (key, softbox, sphere light,
fill, ambient, `LIGHT_SCALE`) took many such rounds. The only way to change a light at runtime is Light mode (key 3),
which moves the last light's position along z and leaves an area light's emitter behind
(`rendering-cornell-box-area-light.md`, open points).

Goal: an explorer panel that lists the scene's objects and lights and lets the user change the lights' properties, with
the image re-rendering as they change.

## Scope

### 1. The panel

- An ImGui window next to the others, e.g. "Scene (F5)", toggled like F1 to F4 (`OpenGLWindowsRaytracerUI.cpp`,
  `WindowsRaytracerUI.cpp` for the key).
- Objects: id (`SceneObject::getId`), form (sphere, box, plane, mesh with its face count), placement (scale, rotation,
  translation, from `TransformInfo`), bounding box, the light emitter flag (`ObjectShader::isLightEmitter`), the
  material channels' constant values where the shader is a `ConstMaterialShader`. Read-only to start with.
- Lights: index, shape (point, sphere, box), kind (RGB, blackbody, D65, illuminant A) and their properties. Lights have
  no names yet: show the emitter's id for area lights, or give `LightInfo` a name.
- The ambient light and the background as entries of their own.

### 2. Editing lights

- Position; brightness and colour: RGB emittance, or kind, temperature and brightness for the spectral kinds;
  attenuation factors (point and sphere lights); radius (sphere lights); half extents, rotation and emitting faces (box
  lights); the ambient light.
- Brightness of the spectral kinds: `LightInfo` keeps the normalized `intensity`, not the brightness the factories take
  (`LightInfo::blackbody` etc. divide by the photopic luminance). Changing the temperature at the same brightness needs
  that luminance (`photopicLuminance` in `LightInfo.cpp`, file-local today) or the brightness stored in `LightInfo`.
- Rotation of box lights: `LightInfo::orientation` is a matrix; the panel edits Euler angles in the order
  `ObjectGeometry` applies them (as `addBoxLight` builds it), so `LightInfo` or the panel keeps the angles.
- Area lights and their emitters stay together: the light's position, size, rotation and radiance change the emitter
  object too (its placement and its emittance shader). Nothing links a `LightInfo` to its emitter today: add the link
  (e.g. the emitter's pointer or id in `LightInfo`, or a light rig type owning both). The same link fixes Light mode.

### 3. Applying changes safely

- Scene changes go through `Raytracer::enqueueWork` (root `CLAUDE.md`, Threading Model). Rapid enqueues overwrite each
  other and only the last runs: a slider drag must enqueue the whole edited state (e.g. the light's full copy), not
  one property per call, or edits get lost.
- Light spectra are rebuilt per frame (`createSpectralContexts` in `Raytracer.cpp`), so emittance, kind and
  temperature apply on the next render. Moving or resizing an emitter object changes the scene's bounding volumes:
  rebuild the scene graph (`Scene::buildSceneGraph`) in the same work item.
- Adding and removing lights is out of scope for a first version: the per-light shadow caches
  (`PerLightShadowCache`) and the spectral contexts follow the light count.

### 4. Nice to have

- Copy a light as scene code (`ImGui::SetClipboardText`), to paste a tuned light into the scene file.
- Select in the explorer, and let Light and Object mode drag the selection instead of the last light or object
  (`object-picking.md` covers selecting by clicking into the image).
- Reset a light to the scene's values.

## Key files

- `sources/raytracerui/src/OpenGLWindowsRaytracerUI.cpp` (panels, "Scene Info"), `WindowsRaytracerUI.cpp` (keys, drag
  modes)
- `sources/raytracing/include/raytracing/shading/LightInfo.h`, `src/raytracing/shading/LightInfo.cpp`
- `sources/raytracing/include/raytracing/scene/SceneObject.h`, `geometry/ObjectGeometry.h`, `geometry/TransformInfo.h`
- `sources/raytracerui/src/tests/SceneObjects.cpp` (`addSphereLight`, `addBoxLight`: lights with their emitters)
- `sources/raytracing/src/raytracing/Raytracer.cpp` (`enqueueWork`, spectral contexts per frame)

## Related backlog items

- `object-picking.md`: selecting objects and lights by clicking into the image
- `rendering-cornell-box-area-light.md`: Light mode separates area lights from their emitters (open points)
