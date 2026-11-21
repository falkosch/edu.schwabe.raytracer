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
7. `Raytracer::applyBRDF()` composites: spectral BRDF, Fresnel reflectance, Beer-Lambert absorption
8. Reflection/transmission rays are traced recursively up to `maxTraceDepth`; the `SpectralContext` propagates
9. Output: `spectrumToXYZ` (with the context's XYZ context) `→ xyzToDisplayRGB` per pixel → HDRImage
   (sRGB-linear) + depth map + timing map

## Subsystems

### Scene (`scene/`)

- `Scene` -- owns SceneObjects, builds scene-level KD-tree via `buildSceneGraph()`
- `SceneObject` -- Form + ObjectShader + transform + ID string
- `Camera` -- perspective projection, view/model matrices, frustum plane computation
- `Resources` -- loads OFF meshes and HDR/PPM textures

### Geometry (`geometry/`)

- `Form` (base) -> `Sphere`, `Box`, `Plane`, `Mesh`
- `Mesh` loads `.off` files, builds per-mesh KD-tree, stores vertices/facets/normals/texcoords. Its leaf loops read
  the plane normals in `leafGeometry` order, component-wise in blocks of 8 (`FacetLanes`, `FacetNormalsBlock`), and
  test a block at once with `nearestIntersectionHavel`'s steps lane-wise. Its dot products are explicit FMA chains, so
  results can differ from the single-facet test in the last bits
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
`wideNodes`: each `KDTreeWideNode` holds the children of an inner node's children (2 to 4 lanes, boxes component-wise),
so one SIMD test (`overlappedLanes`) covers two levels; it shares the binary tree's leaves.

Traversers (template on intersection type):

- `NaiveKDTreeTraverser<T>` -- depth-first with an explicit stack (nearest queries: the child the ray enters first,
  the other deferred; any-hit: child A first), skips a subtree when the ray misses both child boxes, early exit for
  any-hit queries
- `WideKDTreeTraverser<T>` -- the same queries over `wideNodes`, about half the node visits; meshes use it (nearest:
  the hit child entered first, the others deferred with their entry distance and dropped on pop when beyond a hit;
  any-hit: lane order, which visits the leaves in the naive traverser's order)
- `VoxelizationKDTreeTraverser<T>` -- debug view: intersects the leaf boundings instead of their geometry

### Shading (`shading/`)

- `SceneShader` -- scene-level: background shader, ambient light, light collection, shadow computation
- `ObjectShader` -- per-object: 7 `MaterialShader` slots (diffusion, reflectance, specular, roughness,
  transmittance, refractionEta, emittance) + texture transform
- `SpectralContext` -- per hero wavelength: wavelengths, Smits basis, XYZ context, ambient and per-light spectra;
  built once per sub-sample and frame (`SceneShader::createSpectralContext()`)
- `Shader<C, I, O>` -- template base for all shaders (Containment, Intersection -> Output)
- Concrete shaders: `ConstShader`, `HDRImageShader`, `EnvironmentShader`, `IntersectionNormalShader`,
  `MultiplyByConstShader`, `ConcatenatingShader`, `NoiseGeneratorMap<Octaves>`

### BRDF (`shading/brdf/`)

- `ggxD()` -- Trowbridge-Reitz normal distribution
- `ggxG_smith()` -- Smith height-correlated geometry term
- `ggxV_smith()` -- Smith height-correlated visibility term (G / 4NoVNoL, with NoV * NoL cancelled)
- `evaluateGGX()` -- full microfacet BRDF (D * G * F / 4NoVNoL)
- `evaluateGGX_DG()` -- BRDF without Fresnel (D * G / 4NoVNoL), from precomputed NoV, NoL, NoH
- `schlickFresnelSpectral()` -- per-wavelength Schlick approximation on Spectrum
- `sampleGGX()` -- VNDF importance sampling (Heitz 2018)

### Color (`shading/color/`)

- `primaries.h` -- sRGB/Rec.2020 <-> XYZ matrices, Bradford chromatic adaptation
- `view_transform.h` -- `agx()` (Sobotka AgX), `acesFilmic()` (Narkowicz 2015), `srgbEncode()`/`srgbDecode()`

### Common (`common/`)

- `RGBS` -- RGB color wrapper over Float4 with arithmetic operators
- `HDRImage` -- Float4-per-pixel image with bilinear sampling
- `Bitmap` -- LDR image (8-bit per channel)
- `PNGWriter` -- PNG file output (uses libpng)
- `PerlinNoiseGenerator`, `SimplexNoiseGenerator` -- procedural noise
- `RefractionIndices` -- physical IOR constants (Vacuum, Air, Water, Glass, Diamond, etc.)
- `StatisticsCookie` -- ray counts, KD-tree visits, cycle timings
