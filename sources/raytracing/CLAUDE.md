# Raytracing Library

Core rendering engine. Static library depending on `primitives` and `vectorization`.

## Rendering pipeline

1. `Raytracer::trigger()` starts a render on the worker thread
2. `RaytracerPackets` tiles the image into configurable ray packets with optional super-sampling
3. Primary rays are generated from `Camera` frustum parameters; each sub-sample gets a stratified
   `heroLambda` for hero-wavelength spectral sampling (Wilkie 2014). Per frame, the raytracer builds one
   `SpectralContext` per sub-sample (wavelengths, Smits basis, CMF/XYZ context, ambient and light spectra), and
   every `Raytrace` points to the context of its sub-sample
4. KD-tree traversal finds nearest geometry intersection
5. `ObjectShader::shade()` evaluates 7 material channels, upsamples RGB to `Spectrum` via the context's Smits basis
6. `SceneShader::sampleLighting()` computes per-light contributions in spectral domain (Lambert diffuse,
   GGX microfacet specular with spectral Schlick-Fresnel, distance attenuation, shadow rays with caching)
7. `brdf::applyBRDF()` composites: spectral BRDF, Fresnel reflectance, Beer-Lambert absorption
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
   glossy rays don't darken the image
9. Output: `spectrumToXYZ` (with the context's XYZ context) `→ xyzToDisplayRGB` per pixel → HDRImage
   (sRGB-linear) + depth map + timing map

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

- `SceneShader` -- scene-level: background shader, ambient light, light collection, shadow computation. Shadow rays
  pass transmissive objects (Fresnel transmission and absorption along the straight chord, no caustics). Area lights
  get up to `sphereLightSamples` shadow rays:
    - a sphere light (`LightInfo::radius`) to its visible cap, lighting as a point light at its center (exact while
      the sphere is above the horizon, too dark where the horizon cuts it); rough surfaces get its highlight with the
      lobe widened by its size (Karis 2013)
    - a box light (`LightInfo::halfExtents`, `orientation`, `emittingFaces`; a zero extent gives a rectangle) to the
      area of its emitting faces that the hit sees, clipped at its horizon (`BoxLightFaces`). Their radiance lights
      by the exact form factor (Lambert's polygon formula) times the lit fraction of the rays, each weighed by its
      irradiance (ratio estimator, Heitz et al. 2018); rough surfaces get the highlight from the same rays (light
      sampling), or from a representative point per face without samples (Karis 2013)

  The light's emissive object (`setLightEmitter`) shows in mirrors, while the glossy rays of rough surfaces skip it.
  Only these `LightInfo`s light the scene: the emittance of any other object (meshes, emissive textures, boxes without
  a box light) makes it glow where it is seen, but it casts no light
- `ObjectShader` -- per-object: 7 `MaterialShader` slots (diffusion, reflectance, specular, roughness,
  transmittance, refractionEta, emittance) + texture transform
- `SpectralContext` -- per hero wavelength: wavelengths, Smits basis, XYZ context, ambient and per-light spectra;
  built once per sub-sample and frame (`SceneShader::createSpectralContext()`)
- `Shader<C, I, O>` -- template base for all shaders (Containment, Intersection -> Output)
- `beerLambertTransmittance()` (`beer_lambert_transmittance.h`) -- the absorption along a path through a
  transmissive material, for transmitted rays (`traceTransmission`) and shadow rays (`transmitShadowRay`)
- Concrete shaders: `ConstShader`, `HDRImageShader`, `EnvironmentShader`, `IntersectionNormalShader`,
  `MultiplyByConstShader`, `ConcatenatingShader`, `NoiseGeneratorMap<Octaves>`
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
- `fresnelReflectance()` (`fresnel_reflectance.h`) -- Schlick reflectance at a dielectric interface, from the eta
  layout of `RefractionIndices::refractionEta`; 1 for a total internal reflection
- `applyBRDF()` (`apply_brdf.h`) -- the radiance of a hit from its `BRDFParameters`
- `sampleGGX()` -- VNDF importance sampling (Heitz 2018), for the glossy reflections
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
