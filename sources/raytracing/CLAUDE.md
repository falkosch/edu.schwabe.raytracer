# Raytracing Library

Core rendering engine. Static library depending on `primitives` and `vectorization`.

## Rendering pipeline

1. `Raytracer::trigger()` starts a render on the worker thread
2. `RaytracerPackets` tiles the image into configurable ray packets with optional super-sampling
3. Primary rays are generated from `Camera` frustum parameters; each sub-sample gets a stratified
   `heroLambda` for hero-wavelength spectral sampling (Wilkie 2014), the strata dividing one wavelength step
   (`DELTA_LAMBDA`: a context's 8 wavelengths lie a step apart, so strata of the whole range would repeat the same 8
   wavelengths for 1, 2, 4 or 8 sub-samples). With fewer sub-samples than 8 in a denoised image, each sub-sample's
   stratum splits into 8 / n, and the pixels take them in turn (`pixelContext`: Roberts' R2 sequence over the pixels,
   every 5 x 5 pixels take all), since the same 8 wavelengths in every pixel are a fixed quadrature that shifts
   saturated colours (the Cornell box's red wall was +6 % in previews). Undenoised images (Whitted ray tracing,
   `denoise` off) keep that quadrature: their pixels' dither would show on saturated colours. Per frame, the
   raytracer builds one `SpectralContext` per sub-sample and pixel context (wavelengths, Smits basis, CMF/XYZ context,
   ambient and light spectra), and every `Raytrace` points to the context of its sub-sample and pixel
4. KD-tree traversal finds nearest geometry intersection
5. `ObjectShader::shade()` evaluates 7 material channels, upsamples RGB to `Spectrum` via the context's Smits basis:
   the reflectances (diffusion, reflectance, specular) within [0, 1] at every wavelength, white at 1
   (`SpectralContext::reflectanceFromRGB`, `spectral::rgbToReflectance`: Smits' white is 1.06), the emittance and the
   absorption lengths as they are (`fromRGB`, as the lights and backgrounds)
6. `SceneShader::sampleLighting()` computes per-light contributions in spectral domain (the diffuse irradiance,
   GGX highlights with the specular lobe's Fresnel, inverse-square falloff, shadow rays with caching). At the hits
   of diffuse rays (`Raytrace::diffuseBounce`), `manyLightSamples` (0 in the library) bounds the shadow rays to point
   lights: with more of them, systematic sampling picks that many by their unshadowed diffuse light, each picked light
   weighed by its picks per their expected number (unbiased; exact for the diffuse light without shadows)
7. `brdf::applyBRDF()` composites: Lambert's diffusion / pi, the highlights, the reflected and transmitted light
   (weighed by the specular lobe's Fresnel reflectance, `brdf::SpecularFresnel`, and what it leaves), Beer-Lambert
   absorption
8. Reflection/transmission rays are traced recursively up to `maxTraceDepth`; the `SpectralContext` propagates. A
   surface rougher than `brdf::MirrorRoughness` reflects up to `glossySamples` VNDF-sampled rays
   (`traceGlossyReflection`, fewer the less the reflection adds to the pixel); `Raytrace::seed` (pixel and sub-sample,
   `shading/sampling.h`) makes the sampling deterministic. `RaytraceBaseParameters::glossySamples` and
   `sphereLightSamples` cap the rays of this distribution ray tracing (default 16; the latter for sphere and box
   lights); 0 is Whitted ray tracing (mirror reflections, hard shadows). The seeds and sequence shifts hash in SIMD
   lanes (`deriveSeeds`, 8 at a time; `sequenceOffset2D`, once per hit). The sub-samples of a pixel share one sample
   sequence (`Raytrace::strataSeed`, `stratum`, `stratifiedIndex`); `seedOffset` picks another sample pattern. Against
   fireflies, `glossyClamp` limits the radiance of one glossy ray (`spectral::limitPeak`), and `regularizePaths` keeps
   `Raytrace::glossy` along mirror and refraction chains (both biased, off by default in the library). The sample
   budgets shrink with `Raytrace::visibilityIndex` (divided among a glossy reflection's rays), the cutoffs of dim rays
   and lights read `Raytrace::importance` (not divided: each glossy ray estimates the whole reflection), so that more
   glossy rays don't darken the image. With shadow rays, the highlights of area lights on rough surfaces combine
   both strategies by multiple importance sampling (power heuristic, `power_heuristic.h`): the shadow rays' part is
   weighed in `SceneShader::sampleLighting` (it gets the hit's number of glossy rays), a glossy ray that hits an
   emitter (`Raytrace::weighsEmitters`) returns its light (`IlluminatedIntersection::emitterLight`,
   `ObjectShader::getEmittedLight`) and adds its weighed part to the highlight, unclamped. Both estimate the same
   light: `SceneShader::lightPointDensity` gives the glossy side the shadow rays' density, 0 where they don't sample
   the highlight (horizon, cutoff, seen from inside), and the light's radiance as they take it; a point off the
   light (an emitter moved away from it) is no light, and a lobe that doesn't reflect (F0 = 0, equal indices) has no
   highlight on either side. Highlights, mirror and glossy reflections take one Fresnel reflectance
   (`brdf::SpecularFresnel`, built per hit in `Raytracer::trace`): Schlick's of the specular channel on an opaque
   surface, the indices' at a transmissive one, times the reflectance channel as the lobe's weight (at most 1); each
   glossy ray takes its microfacet's, and the VNDF samples' exact weight G2 / G1(V) (height-correlated, as the
   highlights). Highlights, glossy reflections and rough transmission compensate the multiple scattering between the
   microfacets that the single-scattering lobe loses (`SpecularFresnel::multipleScattering`, Turquin 2019). A rough
   dielectric whose refracted lobe is wider than a mirror's (|1 - eta| / 2
   times the reflected lobe's width) reflects and refracts through VNDF-sampled microfacet normals, each with its own
   Fresnel reflectance (`traceGlossyReflection`, `traceRoughTransmission`, Walter et al. 2007): a microfacet that
   reflects totally refracts nothing and reflects all, a masked direction counts as black. The refraction takes fewer
   rays than a reflection of that roughness; a narrower refracted lobe refracts as a mirror, both lobes with the smooth
   surface's Fresnel, as at the smooth surface's total reflection. Diffuse interreflection (`diffuseSamples`, 0 in the
   library): a diffuse hit gathers the light of other surfaces with up to that many rays of Lambert's distribution
   (`traceDiffuseReflection`, `brdf::sampleLambert`, fewer the less the diffuse reflection adds to the pixel, on a
   sequence of its own); pi times their mean radiance adds to the lights' irradiance, in place of the ambient light. One
   bounce: their hits (`Raytrace::diffuseBounce`, also along mirror and glossy rays after them) trace none and keep the
   ambient light, as the stand-in for the further bounces (Ward's ambient value). Diffuse rays are `Raytrace::glossy`
   (an area light's emitter adds nothing, its light is the shadow rays'), `glossyClamp` limits each. Irradiance
   caching (`irradianceCaching`, off in the library; Ward et al. 1988): before the pixels, `Raytracer::trace` records
   the irradiance of 16 times `diffuseSamples` rays, at least 256, each limited to 16 times their mean against rare
   bright paths (`IrradianceRecord`, with the harmonic mean distance of the rays that hit as the radius, at least
   1.5 pixels, valid by at most twice the grid: `maxRadius`) at the primary hits of each first-round
   sub-sample, on grids of 32 down to 4 pixels where the coarser grids' records don't cover the hit fully
   (`RaytracerCache::recording`); a level's rays see only the coarser levels' records, so the records don't depend on
   the threads. Each level clamps the radii by the neighbours' (R_i <= R_j + |x_i - x_j|, Krivanek et al. 2006), so
   that a record in a seam, whose rays pass through the other surface, can't spread; the clamp takes the radii
   without their caps (a fine grid's cap would shrink the coarser records it was placed between, and open holes that
   no level fills). Each sub-sample has records of its own, each record at one of the sub-sample's contexts (by a
   hash of its pixel). Diffuse hits of the same sub-sample interpolate the records valid there, those of its other
   contexts at their own wavelengths (`IrradianceCache`, `spectral::shiftWavelengths`: linear between the record's
   wavelengths, 50 nm apart; accuracy 0.1, the normals' term at half Ward's weight, so valid up to 16 degrees from a
   record's normal; weights 1 / eps - 1 / a, one grid per level): where their coverage (the weight sum times a)
   reaches 0.5 instead of tracing diffuse rays, below it blended with the rays by the coverage, so that no seam shows
   between the two.
   Adaptive sampling (`adaptiveRounds`, `adaptiveThreshold`, off in the library and for Whitted ray
   tracing), in two passes: every pixel of at least 2 x 2 sub-samples takes its first round and keeps the spread of
   its display luminance (`SubSampleSpread`, the standard error of the mean); then each takes the further rounds that
   its neighbours' spread asks for (`AdaptiveRounds`: the root mean square of their errors, never its own, which would
   bias it dark where rare bright rays dominate), on the grid shifted by Roberts' R2 sequence, at hero wavelengths of
   their own (one set of `SpectralContext`s per round), continuing the pixel's sample sequence
9. Output: `spectrumToXYZ` (with the context's XYZ context) `→ xyzToDisplayRGB` per pixel → HDRImage
   (sRGB-linear) + depth map + timing map
10. Denoising (`denoise`, off in the library and for Whitted ray tracing): the primary rays leave their guides
    (`PixelFeatures` via `Raytrace::features`: the normal, albedo and path length of the first hit that isn't a
    smooth mirror or glass; the albedo is the diffusion's RGB, `ObjectShader::sampleDiffusion`, the same at every hero
    wavelength), summed per pixel; `ATrousDenoiser` filters the image with them (edge-avoiding a-trous
    wavelets, Dammertz et al. 2010, with SVGF's edge-stopping weights, Schied et al. 2017). The luminance weight
    compares against the variance of each pixel's mean from its sub-samples, or with one sub-sample against the
    variance around it among pixels of the same surface

## Subsystems

### Scene (`scene/`)

- `Scene` -- owns SceneObjects, builds scene-level KD-tree via `buildSceneGraph()`
- `SceneObject` -- Form + ObjectShader + transform + ID string
- `Camera` -- perspective projection, view/model matrices, frustum plane computation. Its header documents the
  conventions: the frustum's "top left" is image row 0, the bottom of the view; `translate(v)` moves the camera by
  -v; `rotate` takes yaw, pitch and roll
- `Resources` -- loads OFF meshes and HDR/PPM textures

### Geometry (`geometry/`)

- `Form` (base) -> `Sphere`, `Box`, `Plane`, `Mesh`
- `Mesh` loads `.off` files, builds per-mesh KD-tree, stores vertices/facets/normals/texcoords. Its leaf loops read
  the plane normals in `leafGeometry` order, component-wise in blocks of 8 (primitives' `FacetLanes`,
  `FacetNormalsBlock`), and test a block at once with `intersectionsHavel` (`nearestIntersectionHavel`'s steps
  lane-wise). Its dot products are explicit FMA chains, so results can differ from the single-facet test in the last
  bits. `buildCubeMesh`, `buildPlaneMesh` and `buildTriangleMesh` take a traverser and a balancer like
  `loadFromOffFile`; without them the mesh has no KD-tree and tests every facet (also after `setTreeBalancer`)
- `FacetIntersection` -- model-space intersection data (position, geometric normal, tex coords)
- `SceneIntersection` -- world-space data (smoothed normal, reflection direction, parent object ref)

### KD-Tree (`geometry/partitioning/`)

Balancers (all extend `KDTreeBalancer`):

- `BruteForceSAHKDTreeBalancer` -- exhaustive SAH, best quality (default)
- `FixedIterationsSAHKDTreeBalancer` -- bounded SAH iterations
- `MedianKDTreeBalancer`, `ArithmeticMeanKDTreeBalancer` -- simple split strategies
- `MaxAxisKDTreeBalancer`, `RotatingAxisKDTreeBalancer` -- axis selection strategies

Balancers build a temporary pointer tree (`KDTreeNode`, `KDTreeNodeChildren`) and flatten it into `KDTreeRoot`:
inner nodes depth-first in parallel arrays (`childBoundings`, one cache line per node, and `childLinks`), leaves as
offset/count ranges into `leafGeometry`. A `KDTreeLink` is an inner-node index or a leaf index with `LeafFlag` set.
`GeometryNodesTraverser` gets each leaf's geometry as a `GeometryNodeSpan`. The build also collapses the tree into
`wideNodes` (`KDTreeRoot::buildWideNodes()`): each `KDTreeWideNode` holds an inner node's descendants three levels
down (2 to 8 lanes of `KDTreeWideLanes` = `Float8`, boxes component-wise, depth-first with child A first), so one SIMD
test (`overlappedLanes`) covers three levels; it shares the binary tree's leaves.

Traversers (template on intersection type):

- `NaiveKDTreeTraverser<T>` -- depth-first with an explicit stack (nearest queries: the child the ray enters first,
  the other deferred; any-hit: child A first), skips a subtree when the ray misses both child boxes, early exit for
  any-hit queries
- `WideKDTreeTraverser<T>` -- the same queries over `wideNodes`, under half the node visits; meshes and the scene
  (`raytracerui`'s `main.cpp`) use it (nearest: the hit child entered first, the others deferred with their entry
  distance and dropped on pop when beyond a hit; any-hit: lane order, which visits the leaves in the naive traverser's
  order)
- `VoxelizationKDTreeTraverser<T>` -- debug view: intersects the leaf boundings instead of their geometry

### Shading (`shading/`)

- `SceneShader` -- scene-level: background shader, ambient light (a radiance from all directions, which a diffuse
  surface reflects times its diffusion; a hit that traces diffuse rays takes what they gather instead), light
  collection, shadow computation. Shadow rays pass transmissive objects (what their specular lobe and diffusion leave,
  and the absorption along the straight chord, no caustics). Area lights get up to `sphereLightSamples` shadow rays:
    - a sphere light (`LightInfo::radius`) to its visible cap (`SphereLightCap`: points of the disk the cap projects
      to, lifted onto it, and their density per solid angle), lighting by its exact form factor, also where the horizon
      cuts it (Snyder 1996), times the lit fraction of the rays, each weighed by its cosine per density (ratio
      estimator); without samples one ray to the cap's center, or to the middle of the part above the horizon; rough
      surfaces get its highlight from the same rays with the sphere's radiance (the intensity over its cross
      section), or without samples from its center with the lobe widened by its size (Karis 2013)
    - a box light (`LightInfo::halfExtents`, `orientation`, `emittingFaces`; a zero extent gives a rectangle) to the
      area of its emitting faces that the hit sees, clipped at its horizon (`BoxLightFaces`). Their radiance lights
      by the exact form factor (Lambert's polygon formula) times the lit fraction of the rays, each weighed by its
      irradiance (ratio estimator, Heitz et al. 2018); rough surfaces get the highlight from the same rays (light
      sampling), or from a representative point per face without samples (Karis 2013)
    - a directional light (`LightInfo::direction`, a distant light like the sun: its emittance is its irradiance,
      without falloff) into its cone (`angularRadius`, uniform in solid angle), lighting from its direction; its
      highlight is the lobe widened by the cone (Karis 2013), and it has no emitter

  Point and sphere lights fall off as the inverse square of the distance (their emittance is an intensity). The light's
  emissive object (`setLightEmitter`, which takes the light; only where it emits, `ObjectShader::emitsAt`: a box light's
  emitter emits from the light's emitting faces, `BoxFacesShader`) shows in mirrors; the glossy rays of rough surfaces
  see it weighed against the shadow rays (multiple importance sampling), and skip it without shadow rays. Only these
  `LightInfo`s light the scene by light sampling: the emittance of any other object (meshes, emissive textures, boxes
  without a box light) makes it glow where it is seen, and it lights other surfaces only through their diffuse rays
  (`diffuseSamples`) and reflections
- `ObjectShader` -- per-object: 7 `MaterialShader` slots (diffusion, reflectance, specular, roughness,
  transmittance, refractionEta, emittance) + texture transform
- `SpectralContext` -- per hero wavelength: wavelengths, Smits basis, XYZ context, ambient and per-light spectra;
  built once per sub-sample (and pixel context, with fewer than 8 sub-samples) and frame
  (`SceneShader::createSpectralContext()`)
- `Shader<C, I, O>` -- template base for all shaders (Containment, Intersection -> Output)
- `beerLambertTransmittance()` (`beer_lambert_transmittance.h`) -- the absorption along a path through a
  transmissive material, for transmitted rays (`traceTransmission`) and shadow rays (`transmitShadowRay`)
- Concrete shaders: `ConstShader`, `HDRImageShader`, `EnvironmentShader`, `IntersectionNormalShader`,
  `MultiplyByConstShader`, `ConcatenatingShader`, `NoiseGeneratorMap<Octaves>`, `BoxFacesShader` (one value on a
  box's faces of a mask, another elsewhere)
- `EnvironmentShader` maps the ray direction into its image by an `EnvironmentProjection`
  (`environment_coordinates.h`; the image center faces the start camera's view direction -z; latitude-longitude maps
  sample with `TextureWrap::ClampV`, the disc projections with `Clamp`). The projections of `data/textures`:
    - `FisheyeDome` (default): `distorted_environment`
    - `AllSkyFisheye` (equidistant, the horizon at a radius the shader takes): `env_eso_bright_night_at_paranal`
      (all-sky photo, horizon at 0.43; the walkway around the camera fills the ring beyond)
    - `MirrorBall`: `env_spherical_cameraman`, `env_spherical_forest` (photos of a chrome ball), and the radial
      `env_spherical_supernova`, `env_spherical_gradient`, `env_spherical_phong_gradient`
    - `AngularMap`: `env_spherical_stpeters`, `env_spherical_uffizium` (Debevec's light probes)
    - `LatitudeLongitude`: `env_latitude_checker`

### BRDF (`shading/brdf/`)

- `ggxD()` -- Trowbridge-Reitz normal distribution
- `ggxG_smith()` -- Smith height-correlated geometry term
- `ggxV_smith()` -- Smith height-correlated visibility term (G / 4NoVNoL, with NoV * NoL cancelled)
- `evaluateGGX()` -- full microfacet BRDF (D * G * F / 4NoVNoL)
- `evaluateGGX_DG()` -- BRDF without Fresnel (D * G / 4NoVNoL), from precomputed NoV, NoL, NoH
- `schlickFresnelSpectral()` -- per-wavelength Schlick approximation on Spectrum
- `fresnelReflectance()` (`fresnel_reflectance.h`) -- the reflectance at a dielectric interface by the exact Fresnel
  equations for unpolarized light (the mean of Rs and Rp; 0 for equal indices), from the eta layout of
  `RefractionIndices::refractionEta`; 1 for a total internal reflection. The overloads of an incident direction and a
  (microfacet) normal, or of the cosine alone, decide on the total reflection themselves
- `SpecularFresnel` -- the Fresnel reflectance of a hit's specular lobe, one law for its highlights, mirror and glossy
  reflections and the emitters its glossy rays find: Schlick's of the specular channel (opaque surfaces, conductors; an
  F0 of 0 is no lobe) or the indices' (transmissive surfaces, dielectric interfaces), times the reflectance channel as
  the lobe's weight (clamped to [0, 1], as OpenPBR's `specular_weight`); a dielectric interface transmits what the lobe
  leaves. `multipleScattering()` compensates what the single-scattering lobe loses between its microfacets (Turquin
  2019, a factor at the view direction: 1 + F0 (1 - E_1) / E_1, or 1 / (E_R + E_T) for both lobes of a dielectric
  interface; a white furnace keeps all the light), `directionalAlbedo()` and `averageAlbedo()` include it
- `SpecularAlbedo` -- tables of the GGX lobe's directional albedo E(mu, alpha) and its cosine-weighted mean (single
  scattering; the means of the compensated lobes): for Schlick's Fresnel (E_1 of F = 1 and E_5 of the (1 - cos)^5
  term) and for dielectric interfaces from either side (reflected and transmitted), computed once in parallel by VNDF
  samples (`Raytracer::trace` builds them before its parallel regions), 32 cosines by 32 roughnesses by 16 index
  ratios
- `applyBRDF()` (`apply_brdf.h`) -- the radiance of a hit from its `BRDFParameters`; the diffuse part is Lambert's
  BRDF, the diffusion / pi times the direct irradiance (a white surface under light from all directions reflects
  that light's radiance); the reflected and transmitted light come weighed by the specular lobe's Fresnel. Under a
  specular lobe, the diffuse base is coupled to it (Kelemen and Szirmay-Kalos 2001): it gets what the lobe leaves
  of each light at the light's direction (`SceneShader::sampleLighting`; the ambient light and the diffuse rays'
  irradiance what it leaves of light from all directions) and reflects what it leaves at the view direction per what
  it leaves of light from all directions (`BRDFParameters::diffuseCoupling`): together at most the light. A
  transmissive surface's diffusion takes its share of what the lobe transmits, the transmission the rest
- `sampleLambert()` (`sample_lambert.h`) -- cosine-weighted directions (Malley's method, Shirley and Chiu's concentric
  mapping), for the diffuse rays
- `sampleGGX()` -- VNDF importance sampling (Heitz 2018), for the glossy reflections; `sampleGGXNormal()` its
  microfacet normal, for rough refraction; `pdfGGX()` the density of its directions, `smithG1()` Smith's masking
- The roughness channel is the GGX alpha itself (highlights and glossy reflections), not its square root

### Color (`shading/color/`)

- `primaries.h` -- sRGB/Rec.2020 <-> XYZ matrices, Bradford chromatic adaptation
- `view_transform.h` -- `agx()` (Sobotka AgX, output already display-encoded), `acesFilmic()` (Narkowicz 2015),
  `srgbEncode()`/`srgbDecode()`

### Common (`common/`)

- `RGBS` -- RGB color wrapper over Float4 with arithmetic operators
- `HDRImage` -- Float4-per-pixel image with bilinear sampling (texel i's center at (i + 0.5) / resolution, row 0 at
  v = 0). `TextureWrap::Repeat` tiles (default), `ClampV` keeps the rows of images that span a sphere once apart at
  the poles, `Clamp` keeps both axes at the edges; `HDRImageShader` takes the mode. No mipmaps: minified textures
  alias
- `HDRImage::saveAsPFM()`, `loadPFM()` -- Portable Float Map file I/O (PF/Pf, either byte order on load)
- `Bitmap` -- LDR image (8-bit per channel), laid out as a Windows DIB: b,g,r per pixel, the bottom row first.
  `saveAsBMP`/`loadBMP` (uncompressed 24-bit), `saveAsPNG`/`loadPNG`, `loadPPM`; `HDRImage(Bitmap)` and
  `Bitmap(Image)` convert between the channel orders
- `savePNG()` (`save_png.h`), `loadPNG()` (`load_png.h`) -- PNG file output and input (WIC)
- `PerlinNoiseGenerator`, `SimplexNoiseGenerator` -- procedural noise (Simplex follows Ken Perlin's reference, but
  with a seeded hash table; `NoiseGeneratorMap` sums octaves of `noise3`, octave i at frequency 2^i weighing 2^-i)
- `RefractionIndices` -- physical IOR constants (Vacuum, Air, Water, Glass, Diamond, etc.)
- `StatisticsCookie` -- ray counts, KD-tree visits, cycle timings
