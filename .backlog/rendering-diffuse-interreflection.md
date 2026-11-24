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
  caching (Ward et al. 1988), denoising (`rendering-distribution-noise.md`, candidates).
- Tests: the irradiance of a uniform emitter estimated by diffuse rays, colour bleeding (a white floor point next to
  a red wall gets red), 0 rays is the image without interreflection.

## Key files

- `sources/raytracing/src/raytracing/Raytracer.cpp` (`trace`, `traceReflection`; `traceGlossyReflection` as the model)
- `sources/raytracing/include/raytracing/shading/RaytraceBaseParameters.h`, `shading/sampling.h`
- `sources/raytracerui/src/main.cpp`, `OpenGLWindowsRaytracerUI.cpp` (option, slider)
- `sources/raytracerui/src/tests/CornellBoxScene.cpp`, `CLAUDE.md` (regression block)

## Related backlog items

- `DONE/rendering-cornell-box-area-light.md`: the box light and the ambient stand-in
- `rendering-distribution-noise.md`: measuring the noise, denoising
