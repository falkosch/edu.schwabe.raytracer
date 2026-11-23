# Cornell box: a box light in the ceiling and lighting closer to the reference (done parts)

Carved out of `.backlog/rendering-cornell-box-area-light.md`. Sphere lights near the horizon, Light mode leaving area
lights' emitters behind, the emitter box's non-emitting faces and the Cornell noise numbers stay open there.

## 1. Box lights

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

### Resolution

`LightInfo::halfExtents`, `orientation` and `emittingFaces` (a zero extent gives a rectangle); `BoxLightFaces` clips
the emitting faces a point sees at its horizon (convex polygons, up to three faces), `SceneShader::sampleBoxLight`
lights from them, `sphereLightSamples` caps the shadow rays of all area lights (the name and `--sphere-light-samples`
stayed; the UI slider reads "Area Light Rays"). `addBoxLight` in `SceneObjects.cpp`. Tests (`BoxLightFacesTest`,
`SceneShaderTest`): the form factor against Howell's closed form (centered and under a corner, its limits A / h^2 and
pi), horizon clipping and a rotated box against numerical integration, area-uniform samples, soft shadows, a half
hidden light (64 and 1024 rays), hard shadows without samples, the far field against a point light, the highlight
against the integrated GGX BRDF.

Implemented differently than the scope says:

- Irradiance: the exact form factor of the visible faces, clipped at the horizon (Lambert's polygon formula), times
  the lit fraction of the shadow rays, each weighed by its point's irradiance (ratio estimator, Heitz et al. 2018),
  instead of the sampled sum. Unshadowed regions are free of noise, 0 samples keep the exact irradiance.
- Highlights: light sampling over the same shadow rays (the GGX BRDF towards each lit point), unbiased and within 5 %
  of the integrated BRDF for 64 rays. The representative point (Karis 2013) came out 0.26 to 1.8 times the integral
  for large lights (0.5 x 0.4 to 2 x 2, roughness 0.05 to 0.7) and only serves 0 samples. LTC wasn't done.
- The box light's cutoff compares the unshadowed irradiance times the light's radiance with the visibility cutoff;
  point and sphere lights compare their geometric factor alone.

The check of sphere lights close by: lighting from the center is exact while the sphere lies above the horizon, too
dark where the horizon cuts it; the numbers stay with the open point in `.backlog/rendering-cornell-box-area-light.md`.
Sphere lights keep `attenuationFactors`.

## 2. The Cornell box's lighting

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

### Resolution

A box light of half extents (0.25, 0.005, 0.2) at y = 0.994, emitting from its bottom face, warm white of about 4000 K
in linear sRGB (1.406, 0.927, 0.532) at luminance 3: the light the camera sees is nearly white, the floor below gets an
irradiance of about 0.15. Light and emitter share the RGB radiance, so the mirror spheres show its colour.

Interreflection, tried on the box light: no fill (black ceiling and shadows), ambient 0.03 (closest to the reference),
ambient 0.06 (flat), and dim point lights near the walls in their colours plus one below the ceiling light (their hard
shadows drew coloured shapes on the back wall and the ceiling). The user chose ambient 0.03 in the light's colour on
2026-11-22, diffuse interreflection became `rendering-diffuse-interreflection.md`. The camera fill light is gone.

## 3. Regression

- The Cornell box is the regression baseline (root `CLAUDE.md`, "Regression Tests"): new image and counters, update
  the block. The box light's shadow rays raise the render time.
- Without the point light 2 cm below the ceiling, the ceiling hotspot and the glitter it causes in the rough mirror
  sphere go away (`rendering-distribution-noise.md`, Motivation).

### Resolution

New regression block in `CLAUDE.md`: 10 finite objects, shadow rays 3026473 instead of 736213, the fast preview about
0.155 s instead of 0.088 s. The ceiling hotspot and the rough mirror sphere's glitter are gone. Point and sphere light
scenes rendered byte-identical through the change.

## The other light and shading scenes

The other light and shading scenes use box lights as well (the user's request, 2026-11-23): Lighting adds a cyan
light bar glowing on all faces, Shading adds a rectangular softbox of 2.4 x 1.6 opposite its point key light, and
Showcase a rectangular fill softbox of 1.2 x 0.8 opposite its sphere light softbox (the chrome sphere reflects both,
round and rectangular). Shading also got a sphere light on the floor, and the lights of the three scenes are 15 %
dimmer than first composed (`LIGHT_SCALE` in each scene file), and Shading's key and fill fall off quadratically
(without falloff they lit the floor up to the horizon like an ambient light). An emitter box is an opaque object:
placed between another light and the scene, it shades the scene from that light (the light bar first stood behind the
Lighting scene's red light).

## Commits

- `c07d001` feat(raytracing): add box lights and light the Cornell box with one
