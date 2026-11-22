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

## Scope

### 1. Box lights

- `LightInfo` gets a box shape next to the point and the sphere: half extents around the position (axis-aligned is
  enough for the ceiling; an orientation for other uses). "Volumetric" here means a light with a body that emits from
  its surface, not a participating medium.
- Shadow rays to points spread over the faces visible from the intersection, area-uniform and stratified
  (`sequencePoint2D`); `sphereLightSamples` becomes a cap for all area lights. With 0 one ray to the nearest point
  (hard shadows, Whitted).
- Irradiance per sample instead of from the light's center: E = (A / N) * sum of L cos(theta_x) cos(theta_l) / d^2
  over the visible samples, with the emitter's own cosine. A light of 0.5 x 0.4 at one or two units from the
  surfaces is too large for the center approximation the sphere lights use (check how far that is off for sphere
  lights close by too). `attenuationFactors` then stays for point lights only.
- Highlights of rough surfaces: a representative point (Karis 2013: the point of the rectangle closest to the
  reflected ray), or Linearly Transformed Cosines (Heitz et al. 2016) for an analytic GGX highlight of a rectangle,
  shadowed by the ratio estimator (Heitz et al. 2018). Mirrors see the emissive box, as with sphere lights.
- `addBoxLight` in `raytracerui` next to `addSphereLight`: a thin emissive box marked by `setLightEmitter` plus its
  light.
- Tests: the irradiance under a square light against its analytic form factor, soft shadow fractions, 0 samples gives
  hard shadows.

### 2. The Cornell box's lighting

- Replace the point light by a box light set into the ceiling, e.g. half extents (0.25, 0.005, 0.2) just below y = 1,
  emitting from its bottom face. Tune radiance and colour against the reference: warm white (the original's measured
  light, or a blackbody of 3000 to 4000 K), at exposure 0 with AgX; prefer changing the light over the exposure.
- The reference's lit ceiling, light shadows and colour bleeding come from interreflection, which this ray tracer
  doesn't trace (Whitted plus glossy reflections and area lights). Options, cheapest first:
    - ambient light: lifts the shadows evenly, the ceiling stays flat;
    - fill lights by hand: dim lights near the walls in their colours as a fake colour bleeding, one below the
      ceiling light for the ceiling;
    - diffuse interreflection: cosine-weighted diffuse rays for one bounce (a `diffuseSamples` cap, 0 for off) with
      the sampling of `shading/sampling.h`. The step towards path tracing; costly and noisy, see
      `rendering-distribution-noise.md`.
- Decide on the camera fill light: it lights the front faces flatly.

### 3. Regression

- The Cornell box is the regression baseline (root `CLAUDE.md`, "Regression Tests"): new image and counters, update
  the block. The box light's shadow rays raise the render time.
- Without the point light 2 cm below the ceiling, the ceiling hotspot and the glitter it causes in the rough mirror
  sphere go away (`rendering-distribution-noise.md`, Motivation).

## Key files

- `sources/raytracing/include/raytracing/shading/LightInfo.h`
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (`sampleLighting`, `computeLitSphereFraction`)
- `sources/raytracing/include/raytracing/shading/RaytraceBaseParameters.h` (sample caps)
- `sources/raytracerui/src/tests/CornellBoxScene.cpp`, `SceneObjects.cpp` (`addSphereLight`)
- `CLAUDE.md` (regression block)
