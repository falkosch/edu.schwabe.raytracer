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

- Sphere lights close by, Light mode leaving the emitter behind and the emitter box glowing on all faces moved to
  `rendering-physical-material-model.md` (points 6 to 8, with the measurements).
- Re-measure the Cornell rough mirror's noise (`rendering-distribution-noise.md`, current noise).

## Key files

- `sources/raytracing/include/raytracing/shading/LightInfo.h`
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (`sampleLighting`, `computeLitSphereFraction`)
- `sources/raytracing/include/raytracing/shading/RaytraceBaseParameters.h` (sample caps)
- `sources/raytracerui/src/tests/CornellBoxScene.cpp`, `SceneObjects.cpp` (`addSphereLight`)
- `CLAUDE.md` (regression block)
