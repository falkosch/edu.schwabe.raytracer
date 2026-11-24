# Physically based material and light model (done parts)

Carved out of `.backlog/rendering-physical-material-model.md`. The specular lobe, the Fresnel equations, the energy of
layers and microfacets, the light falloff, sphere lights at the horizon, the emitters and the firefly clamp stay open
there.

## 1. Lambert's 1/pi

- `brdf::applyBRDF` multiplies the diffuse albedo by the irradiance (`lighting.diffuse`: the light's emittance times
  the cosine and the falloff, a box light's radiance times `BoxLightFaces::projectedSolidAngle`). The Lambert BRDF
  is albedo / pi, so diffuse surfaces come out pi times too bright relative to the highlights (`F D G / (4 NoV NoL)`
  times the irradiance, correct), the reflections and the emitters' own radiance. A white wall facing a box light
  that fills its hemisphere (projected solid angle pi) shows pi times the light's radiance: more light than arrives.
- Fix: divide the diffuse term by pi (`applyBRDF`, also the ambient part once it is defined as a radiance), then
  re-tune the scenes' lights (see Scenes). `rendering-diffuse-interreflection.md` raised the same units question for
  its diffuse rays.

### Resolution

- `brdf::applyBRDF` reflects diffusion / pi of the direct irradiance. The ambient light is a radiance from all
  directions (irradiance pi times its value), so its part stays the diffusion times the ambient light.
- The box lights' cutoffs (`sampleBoxLight`, `lightPointDensity`) compare the reflected light, the irradiance / pi,
  so that they sample and skip the lights as before.
- `raytracerui`: every scene's lights and their emitters are times pi (`* Pi<Float>()`, or `lightScale` where a scene
  scales all its lights), the ambient lights not. Diffuse surfaces look as before; highlights, visible emitters and
  their reflections get their physical ratio, about 3 times brighter. The Cornell box's light: luminance 3 pi.
- Renders against `1e7933f` (all test scenes, Whitted and default, preview and full): the Dragon and Meshes scenes
  (diffuse only) identical but for rounding (at most 1 to 3 levels in Whitted); mean brightness +0.2 % (Meshes) to
  +7.5 % (Test Scene 1), Test Scene 2 +14 % (its plane's highlight of F0 0.5, a material of point 1 there) and the
  procedural scene +23 % (its spheres' highlights). The Cornell box's preview ray counters stay those of the
  regression block. The noise numbers of `DONE/rendering-distribution-noise.md` predate the brighter highlights.
- Tests: `ApplyBRDFTest::diffuseLightIsScaledByDiffusionOverPi`, `whiteSurfaceReflectsTheRadianceAroundIt` (radiance
  from the whole hemisphere, direct or ambient, reflects as itself), and
  `SceneShaderTest::whiteSurfaceUnderALightFillingItsHemisphereShowsTheLightsRadiance` (a box light of 2000 x 2000:
  irradiance pi, reflected radiance 1).

## Commits

- `982437d` fix(raytracing): divide the diffuse reflection by pi (Lambert)
