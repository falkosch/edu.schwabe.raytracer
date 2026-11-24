# Diffuse interreflection (one bounce)

## Motivation

The lit ceiling, the light shadows and the colour bleeding of the Cornell box reference
(https://en.wikipedia.org/wiki/Cornell_box) come from light that bounces off the walls. This ray tracer traces the
direct light of point, sphere and box lights, mirror and glossy reflections and refraction, but no diffuse bounces: a
diffuse surface sees only the lights and the ambient light.

The `cornell-box` scene therefore lights its room with a box light in the ceiling plus an ambient light in the
light's colour (warm white times 0.03, `CornellBoxScene.cpp`) as a stand-in. It lifts the shadows and the ceiling
evenly, without colour bleeding and without the ceiling's falloff away from the walls. The user decided this on
2026-11-22 (`DONE/rendering-cornell-box-area-light.md`, section 2): ambient for now, interreflection as this item.

Tried there with the box light (luminance 3): no fill (black ceiling, black shadows), ambient 0.03 (closest to the
reference), ambient 0.06 (flat), and dim point lights near the walls in their colours plus one below the ceiling
light as fake colour bleeding (their hard shadows draw coloured shapes on the back wall and the ceiling: wrong).

## Scope

- Cosine-weighted diffuse rays at hits with a diffuse albedo: stratified points of the pixel's sequence
  (`shading/sampling.h`, `sequencePoint2D`, `stratifiedIndex`) mapped to the hemisphere around the shading normal
  (Malley's method), up to a `diffuseSamples` cap (`RaytraceBaseParameters`, `--diffuse-samples=<n>`, a Config
  slider), fewer the less the hit adds to the pixel (the `visibilityIndex` budget of the glossy rays). 0 is off and
  keeps today's images byte-identical.
- One bounce: the hits of diffuse rays get direct light (their area light shadow rays shrink by the same budget) but
  no diffuse rays of their own. Mirror and glossy rays after a diffuse bounce follow the trace depth.
- A diffuse ray that hits the emissive object of an area light adds nothing: light sampling counts that light, as
  glossy rays skip it (`Raytrace::glossy`, `ObjectShader::isLightEmitter`).
- Units: `brdf::applyBRDF` reflects diffusion / pi of the irradiance (Lambert,
  `DONE/rendering-physical-material-model.md`; `addSphereLight`'s intensity radiance * pi r^2, the box lights' form
  factors). With cosine-weighted rays the irradiance is pi times the mean radiance they bring back; check it against
  the box lights' exact form factor (a diffuse ray estimate of a box light's emitter must match
  `BoxLightFaces::projectedSolidAngle` times its radiance).
- The Cornell box: drop the ambient light, re-tune the box light's radiance at exposure 0 with AgX, new regression
  block.
- Noise and cost: the fast preview has one sample per pixel. Measure with the method of
  `DONE/rendering-distribution-noise.md`. Follow-ups if needed: more diffuse rays at primary hits only, irradiance
  caching (Ward et al. 1988), denoising (`DONE/rendering-distribution-noise.md`, section 8).
- Tests: the irradiance of a uniform emitter estimated by diffuse rays, colour bleeding (a white floor point next to
  a red wall gets red), 0 rays is the image without interreflection.

## Resolution

- `RaytraceBaseParameters::diffuseSamples` (0 in the library; `DIFFUSE_SAMPLES` 16 in `raytracerui`,
  `--diffuse-samples=<n>`, the Config slider "Diffuse Rays"). `Raytracer::traceDiffuseReflection` traces up to that
  many rays per diffuse hit, ceil(diffuseSamples * visibility index * the diffusion's maximum), none below the
  visibility cutoff, in directions of `brdf::sampleLambert` (Malley's method on Shirley and Chiu's concentric mapping,
  vector operations) at points of the pixel's joint sequence (`DiffuseSequenceSeed`; the rays' seeds from
  `DiffuseSeeds`). Pi times their mean radiance adds to `LightShading::diffuse`, the irradiance. `glossyClamp` limits
  each ray as it limits a glossy ray.
- One bounce: `Raytrace::diffuseBounce` marks the diffuse rays and the mirror, glossy and refracted rays after them;
  their hits trace no diffuse rays. Their area light rays shrink with the visibility index (one shadow ray per hit at
  16 diffuse rays), their cutoffs read the importance of the whole diffuse reflection.
- Diffuse rays are `Raytrace::glossy`: an area light's emitter adds nothing (its light is the shadow rays'), and with
  `regularizePaths` the mirrors and glass behind them don't show it either. Emissive objects without a light now
  light their surroundings through the diffuse rays.
- The ambient light (decided on the way, since the other scenes keep theirs): a hit that traces diffuse rays takes
  their light in its place, and a ray that leaves the scene brings the background (a sky, an environment image). The
  hits of diffuse rays keep it, as the stand-in for the further bounces (Ward's ambient value, Radiance's `-av` with
  `-ab`). Without diffuse rays it stands in for all of the interreflection, as before.
- Cornell box: the ambient light dropped. The box light stays at luminance 3 pi: the full render's mean display value
  is 71.0 against 70.3 with the ambient light, the walls and the floor look as before. The ceiling is lit from the
  floor and the walls (darker than the reference's, which has every bounce), the tall box's side is red from the wall,
  the shadows are coloured. New regression block in the root `CLAUDE.md`. Whitted ray tracing (now with
  `--diffuse-samples=0`) leaves its shadows and the ceiling black (mean -32 %).
- Restored on 2026-11-24 (the user found the black ceiling and shadows of `--diffuse-samples=0` a regression), then
  set to 0 in every scene the same day (the user's decision): the object explorer is to set the ambient light per
  scene (`object-explorer.md`). With the Cornell box's 0.03, it stood in for the interreflection where no diffuse rays
  are traced, and for the further bounces at their hits: the default preview's mean display value 70.3 -> 77.0, the
  ceiling's 72 -> 81.
- Renders of all test scenes against `922c489`: before the Cornell box dropped its ambient light,
  `--diffuse-samples=0` was byte-identical in all 44 (Whitted and default, preview and full); Whitted stays
  byte-identical but for the Cornell box. Default renders, mean brightness: Cornell box, Dragon, Showcase, Materials,
  Test Scenes 1 and 2 +0.4 to +1.6 %, Forms +2.3 %, Shading +6.5 % (its environment image lights the scene), Lighting
  +9.4 %, Procedural +16 %, Meshes -1.6 % (its ambient light gave more than the bounce does).
- Cost (release, 512 x 512, preview / full render): Cornell box 0.17 / 1.2 s before, 0.49 / 4.5 s now; Dragon 0.66 /
  2.8 -> 1.9 / 8.7 s; Test Scene 2 0.40 / 2.5 -> 1.35 / 10.0 s; Showcase 1.08 / 8.7 -> 1.20 / 9.1 s; Procedural (343
  point lights, each with a shadow ray at every hit of a diffuse ray) 5.5 / 41 -> 36 / 316 s.
- Noise of the Cornell box (method of `DONE/rendering-distribution-noise.md`, 4 seeds, full render / preview, with
  the adaptive sampling of the time): rough
  mirror 2.51 / 8.73 (1.41 / 5.63 before), teapot 1.70 / 5.56 (0.87 / 3.52), and new crops: ceiling (120, 5, 400, 45)
  2.90 / 12.46, back wall (285, 120, 395, 300) 1.61 / 5.05, the tall box's side (128, 230, 142, 430) 1.81 / 6.36
  (0.00 to 0.69 / 0.01 to 3.21 before, under the ambient light). The preview's speckles are paths diffuse -> mirror
  or glass -> emitter (caustics, 0.4 % of the image's mean display value), which `--regularize-paths` removes. The
  shifts against the references and the follow-ups (more rays at primary hits, irradiance caching, many-light
  sampling, denoising) are in `DONE/rendering-distribution-noise.md` ("Current noise", section 8); the bias of
  adaptive sampling that this noise showed is fixed there in section 7.
- Tests: `SampleLambertTest` (unit directions above the surface, the normal at the square's center, the moments of
  cos / pi and its 45 degree cone, even azimuths) and `RaytracerTest`, which renders through `Raytracer`: diffuse rays
  estimate an emissive square's irradiance as a box light of its shape does by its exact form factor, skip an area
  light's emitter, show a uniform sky on a white wall in place of the ambient light, find the ambient light at their
  hits (one bounce: a white plate's form factor), and a red plate bleeds onto a white wall.
- Found on the way: upsampled white is 1.06 at every wavelength, a reflectance above 1
  (`rendering-physical-material-model.md`, point 10); the plate test allows for it.

## Key files

- `sources/raytracing/src/raytracing/Raytracer.cpp` (`trace`, `traceReflection`; `traceGlossyReflection` as the model)
- `sources/raytracing/include/raytracing/shading/RaytraceBaseParameters.h`, `shading/sampling.h`
- `sources/raytracing/src/raytracing/shading/brdf/sample_lambert.cpp`, `shading/Raytrace.h` (`diffuseBounce`)
- `tests/raytracing.native-test/src/raytracing/Raytracer_test.cpp`, `shading/brdf/sample_lambert_test.cpp`
- `sources/raytracerui/src/main.cpp`, `OpenGLWindowsRaytracerUI.cpp` (option, slider)
- `sources/raytracerui/src/tests/CornellBoxScene.cpp`, `CLAUDE.md` (regression block)

## Related backlog items

- `DONE/rendering-cornell-box-area-light.md`: the box light and the ambient stand-in
- `DONE/rendering-distribution-noise.md`: measuring the noise, its follow-ups (section 8)

## Commits

- `1d485c1` feat(raytracing): bounce diffuse light, cache irradiance, denoise previews
