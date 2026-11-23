# Cornell box: a box light in the ceiling and lighting closer to the reference

## Motivation

The classic Cornell box rendering (https://en.wikipedia.org/wiki/Cornell_box) has a rectangular light set into the
ceiling: soft shadows, a lit ceiling, and the red and green walls bleeding colour onto the white surfaces.

The `cornell-box` scene lights its room ([-1, 1]^3, open towards the camera) with a warm point light (blackbody
3000 K, brightness 0.1) only 0.02 below the ceiling, with the falloff 1/(0.13 d + d^2), plus a faint D65 fill light at
the camera. That gives hard shadows, a dark ceiling with a hotspot right above the light (the source of the rough
mirror sphere's glitter, see `rendering-distribution-noise.md`) and dark corners.

Goal: a box-shaped light in the ceiling and lighting that reads more like the reference. The objects and materials
stay as they are (mirror spheres, noise box, glass teapot, fully saturated walls); the scene may differ from the
original.

This item also carries the box part of "emissive objects don't light the scene"
(`DONE/rendering-test-scene-findings.md`, 5.3): so far only point and sphere lights light the scene.

Box lights, the Cornell box's new lighting and the regression are done: `DONE/rendering-cornell-box-area-light.md`.
The open points below came up while doing them.

## Open points

- Sphere lights close by (the check of section 1 in the done file): lighting from the center (intensity radiance * pi
  r^2, quadratic falloff) is exact while the sphere lies above the horizon
  (`SceneShaderTest::sphereLightIrradianceIsExactAboveTheHorizon`) and too dark where the horizon cuts it. Radius 0.5 at
  a horizontal distance of 1, center height above the horizon: 0.3 gives 0.95 of the exact irradiance, 0.1 gives 0.59, 0
  gives 0 (exact 0.091), -0.2 gives 0 (exact 0.028); radius 0.5 at (0.6, 0.2) gives 0.79. Sphere lights still use
  `attenuationFactors`; a horizon-clipped irradiance would let them drop it, as section 1 asked.
- Light mode (key 3) moves `getLights().back()` only: in the Cornell box that is now the box light, and its emitter
  box stays behind (the same for the Lighting scene's orange sphere light).
- The emitter box glows on all six faces in mirrors, also where only its bottom face emits light (the ceiling light's
  1 cm sides).
- Re-measure the Cornell rough mirror's noise (`rendering-distribution-noise.md`, current noise).

## Key files

- `sources/raytracing/include/raytracing/shading/LightInfo.h`
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (`sampleLighting`, `computeLitSphereFraction`)
- `sources/raytracing/include/raytracing/shading/RaytraceBaseParameters.h` (sample caps)
- `sources/raytracerui/src/tests/CornellBoxScene.cpp`, `SceneObjects.cpp` (`addSphereLight`)
- `CLAUDE.md` (regression block)
