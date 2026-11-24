# Physically based material and light model

## Motivation

The user's standard since 2026-11-24: lighting must be physically based. Approximations are fine where they follow
what is considered near-physical behaviour; invented lighting behaviour isn't. MIS, rough transmission and adaptive
sampling were built to that standard (`DONE/rendering-distribution-noise.md`, section 6). Reviewing them turned up
gaps in the older material and light model. They predate that work and change Whitted renders, so they stayed out of
its scope and are collected here. Lambert's 1/pi is done: `DONE/rendering-physical-material-model.md`.

The user agreed on 2026-11-24 that this item's fixes may change Whitted renders
(`--glossy-samples=0 --sphere-light-samples=0`): where the model is wrong, the Whitted image is wrong too.

## Scope

Each point: what the code does, what physics says, the fix. The order is the suggested work order: the specular
lobe first, because the energy points build on it.

### 1. One specular BRDF for highlights and reflections

- Highlights (`SceneShader::sampleLighting`'s point light highlight, `sampleSphereLight`, `sampleBoxLight`,
  `weighedEmitterHighlight` in `Raytracer.cpp`) take Schlick's Fresnel of the specular channel (F0) times GGX.
  Reflections take `reflectanceCoefficient`: the larger of the indices' Fresnel and Schlick's of the specular channel
  for opaque surfaces, the indices' Fresnel alone for transmissive ones; `applyBRDF` multiplies it by the reflectance
  channel on top.
- So one surface reflects a light and any other object by two different laws:
    - Metals (`SceneMaterial::metal`: reflectance = specular = colour) reflect colour times F(colour), about colour
      squared (gold (1, 0.78, 0.34) reflects (1, 0.61, 0.12) at normal incidence), but their highlights show the
      colour.
    - The Cornell box's glass teapot (reflectance 0.1, specular 0.5, glass's indices) reflects 0.1 times glass's
      Fresnel (0.04 at normal incidence) but has highlights of F0 0.5.
    - The Cornell box's mirror sphere (reflectance = specular = 0.9) reflects 0.81.
  Defect 1 of the MIS work (a glossy ray's emitter part with specular 0) was a symptom.
- Physics: one microfacet BRDF; a light and a lit object in the same direction reflect alike.
- Fix: one Fresnel for the whole specular lobe, from the specular channel (opaque surfaces) or from the indices
  (dielectric interfaces). Decide what happens to the reflectance channel: drop it, or keep it as a weight on the
  whole lobe (at most 1, like OpenPBR's `specular_weight`), so highlights and reflections stay one BRDF. All scenes
  feed it constants (`ConstMaterialShader`).

### 2. Exact dielectric Fresnel

- `brdf::fresnelReflectance` (both overloads), the shadow rays' `fresnelTransmission` (`SceneShader.cpp`) and
  `SceneMaterial::dielectric`'s F0 use Schlick's approximation. Its (1 - cos)^5 term doesn't vanish for R0 = 0: at
  an interface of equal indices it rises to 1 at grazing angles, where the exact Fresnel equations give about 0.
  Test Scene 1's air sphere (eta 1.0003, reflectance 0.1) reflects at its rim and transmits less there; an opaque
  surface with vacuum-to-vacuum indices, a reflectance and specular 0 would reflect at grazing angles.
- Fix: the exact unpolarized Fresnel equations for dielectrics, Rs = ((n1 cos_i - n2 cos_t) / (n1 cos_i + n2 cos_t))^2,
  Rp = ((n1 cos_t - n2 cos_i) / (n1 cos_t + n2 cos_i))^2, F = (Rs + Rp) / 2. `refractEta` already yields cos_t, and
  total internal reflection stays 1. Schlick stays for conductors until `illumination-metal-ior-data.md`. Rs and Rp
  are also what `illumination-polarized-spectral-enabler.md` (polarized Fresnel) and
  `illumination-dispersion-refraction.md` (per-wavelength indices) build on.

### 3. Energy of the layers

- `applyBRDF` adds the diffuse term, the highlights and the reflection weighted by Fresnel without the diffuse
  giving way. A plastic (`SceneMaterial::plastic`: a diffuse colour, specular 0.04) reflects nearly all light
  specularly at grazing angles and still its full diffuse albedo: more than 1. A transmissive material with
  diffusion (the Cornell box's teapot, 0.02) adds it on top of reflection and transmission.
- Physics: light reflected by the dielectric coating doesn't reach the diffuse base, and light that leaves the base
  passes the coating again. Diffuse, specular and transmitted shares sum to at most 1.
- Fix: the diffuse base scaled by the light the specular lobe leaves, from its directional albedo at both
  directions (Kelemen and Szirmay-Kalos 2001; Kulla and Conty 2017, "Revisiting Physically Based Shading at
  Imageworks"); for a mirror coating 1 - F at both angles. Transmission and diffusion share what the coating
  transmits.

### 4. Multiple scattering in microfacets

- GGX with Smith masking scatters once: light that a microfacet reflects onto another is lost. The loss grows with
  roughness (white furnace test, Kulla and Conty 2017). Rough metals and the frosted glass of the Dragon scene come
  out darker than they should. This was listed as a remaining approximation of the rough transmission work.
- Fix: an energy compensation lobe (Kulla and Conty 2017; Turquin 2019) from a table of the directional albedo
  E(mu, alpha) (and eta for dielectrics), computed at startup or embedded. Highlights, glossy reflections and rough
  transmission alike.

### 5. Light falloff

- Point lights fall off as 1 / (c + l d + q d^2) (`SceneShader::attenuateDiffuseIntensity`,
  `LightInfo::attenuationFactors`, OpenGL's model). The Forms, Materials and Meshes scenes use (1, 0, 0), no falloff;
  `TestLightScene` uses (1, 0, 1), 1 / (1 + d^2). Sphere lights use (0, 0, 1).
- Physics: a point light's irradiance falls as 1 / d^2. A light that doesn't fall off is a distant light: a
  direction and an irradiance (soft shadows from its angular diameter, like the sun).
- Fix: inverse square for point and sphere lights; a directional light kind for distant lights. Decide whether
  `attenuationFactors` goes. The scenes' key and fill lights get re-tuned (see Scenes).

### 6. Sphere lights at the horizon

- A sphere light's diffuse light is the shadow rays' lit fraction times the Lambert cosine at the sphere's center
  times the falloff (`SceneShader::sampleSphereLight`, `computeLitSphereFraction`). That is exact for a sphere fully
  above the horizon. Where the horizon cuts it, the cosine at the center underestimates, and a center below the
  horizon gives no light at all (`isNegative(dotv(L, surfaceNormal))` returns) although part of the sphere is above.
  Measured (from `rendering-cornell-box-area-light.md`): radius 0.5 at a horizontal distance of 1, center height
  above the horizon: 0.3 gives 0.95 of the exact irradiance, 0.1 gives 0.59, 0 gives 0 (exact 0.091), -0.2 gives 0
  (exact 0.028); radius 0.5 at (0.6, 0.2) gives 0.79. `SceneShaderTest::sphereLightIrradianceIsExactAboveTheHorizon`
  covers the exact part. A horizon-clipped irradiance would also let sphere lights drop `attenuationFactors` (5).
- Fix: the form factor of a sphere cut by the horizon (Snyder 1996, "Area light sources for real-time graphics";
  Lagarde and de Rousiers 2014, "Moving Frostbite to PBR"); its cap sampling (`SphereLightCap`) then has to cover
  the part above the horizon. Also consider weighing each shadow ray by its own cosine per density (as the
  highlight does) instead of the lit fraction times the unshadowed irradiance, which treats all visible points of a
  partly shadowed light alike.

### 7. Box light emitters

- `addBoxLight` (`raytracerui`, `SceneObjects.cpp`) gives the emitter box `SceneMaterial::emissive(radiance)` on all
  six faces, while `LightInfo::emittingFaces` lets only some faces emit (the Cornell box's ceiling panel downwards,
  the Showcase's softbox to the front). A direct view or a mirror shows the other faces glowing (the softbox's back
  in the Showcase's chrome sphere, the ceiling light's 1 cm sides in the Cornell box's mirrors). A glossy ray that
  hits one of them returns black (`Raytracer::trace`'s emitter branch), not the face's material.
- Fix: an emitter emits only where its light does: the emittance per face (a shader masking the box's faces, or the
  emitter taking its emittance from its `LightInfo`), the other faces with a plain material.

### 8. Dragging a light

- The Light interaction mode (key 3; `WindowsRaytracerUI.cpp`, `DragTypes::Light`) moves the last `LightInfo` only:
  the Cornell box's box light, the Lighting scene's orange sphere light. Its emitter stays and glows where the light
  was; since the MIS work, `SceneShader::lightPointDensity` ignores an emitter off its light.
- Fix: move the light's emitter with it (the scene object whose `ObjectShader::getEmittedLight()` is the light),
  through `Raytracer::enqueueWork`.

### 9. The firefly clamp

- `raytracerui` limits each glossy ray's radiance to 4 (`GLOSSY_CLAMP`, `spectral::limitPeak` in
  `traceGlossyReflection`; the library's default 0 is off). That removes energy, a bias that more samples don't
  remove (darker glossy reflections of bright objects); the noise measurements' references use `--glossy-clamp=0`.
  Rough transmission doesn't clamp.
- Fix: measure the fireflies without the clamp now that MIS covers the area lights (and after 4); default off, or
  documented as the one biased option.

### Scenes

- After 1 and 5 the scenes' lights and hand-written materials need re-tuning: the Cornell box (its glass teapot
  as `SceneMaterial::dielectric`, its mirror sphere, the box light's radiance at exposure 0), Test Scenes 1 and 2,
  the Dragon scene, and the test scenes' key and fill lights. Keep each scene's look where it was physical.
- New Cornell box regression block in the root `CLAUDE.md`.

### Approximations that stay

Near-physical and documented, not part of this item:

- Whitted-style transport: shadow rays pass straight through transmissive objects (Fresnel and Beer-Lambert, no
  caustics), point lights are delta lights.
- The ambient light stands in for interreflection (`rendering-diffuse-interreflection.md`).
- Schlick for conductors until `illumination-metal-ior-data.md`.
- The near-mirror limits (`MirrorRoughness`), the smooth surface's total reflection for rough glass, the adaptive
  sampling's stopping rule (`DONE/rendering-distribution-noise.md`).

## Constraints

- Whitted renders may change where a fix changes the model (see Motivation). Images stay deterministic.
- One commit per point, each with renders of all test scenes before and after (Whitted and default), and the
  change explained.
- Tests per point:
    - a light's highlight equals the glossy reflection of an emitter of the same radiance and shape (1)
    - Fresnel: 0 at all angles for equal indices, ((n1 - n2) / (n1 + n2))^2 at normal incidence, 1 beyond the
      critical angle (2)
    - white furnace tests: a surface of albedo 1 reflects at most 1 at every angle (3), and about 1 with the
      compensation (4)
    - a point light's irradiance falls as 1 / d^2 (5)
    - the horizon-cut sphere's form factor against numeric integration, center below the horizon included (6)
    - a ray to a non-emitting face of a box emitter sees no emittance (7)
- New code follows `simd-scalar-hot-paths.md`: vector operations, no scalar fallbacks.

## Related backlog items

- `rendering-cornell-box-area-light.md`: 6 to 8 came from its open points
- `illumination-metal-ior-data.md`: conductor Fresnel from n and k; builds on 1 (which channel gives metals F0)
- `rendering-diffuse-interreflection.md`: diffuse rays instead of the ambient light, in the units of
  `DONE/rendering-physical-material-model.md`
- `illumination-dispersion-refraction.md`, `illumination-polarized-spectral-enabler.md`: build on 2
- `DONE/rendering-distribution-noise.md`, section 6: where these gaps were found

## Key files

- `sources/raytracing/src/raytracing/shading/brdf/apply_brdf.cpp`, `fresnel_reflectance.cpp`, `ggx.cpp`
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp`, `SphereLightCap.cpp`, `BoxLightFaces.cpp`
- `sources/raytracing/src/raytracing/Raytracer.cpp` (`trace`, `traceGlossyReflection`, `weighedEmitterHighlight`)
- `sources/raytracerui/src/tests/SceneMaterial.cpp`, `SceneObjects.cpp`, the scenes
- `sources/raytracerui/src/WindowsRaytracerUI.cpp`, `main.cpp`
