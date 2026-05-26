# Raytracing Library

Core rendering engine. Static library depending on `primitives` and `vectorization`.

## Rendering pipeline

1. `Raytracer::trigger()` starts a render on the worker thread
2. `RaytracerPackets` tiles the image into configurable ray packets with optional super-sampling
3. Primary rays are generated from `Camera` frustum parameters
4. KD-tree traversal finds nearest geometry intersection
5. `ObjectShader::sample()` evaluates 7 material channels at the intersection point
6. `SceneShader::sampleLighting()` computes per-light contributions (Lambert diffuse, Phong specular, distance
   attenuation, shadow rays with per-light caching)
7. `Raytracer::applyBRDF()` composites: GGX microfacet BRDF, Schlick-Fresnel reflectance, Beer-Lambert absorption
8. Reflection/transmission rays are traced recursively up to `maxTraceDepth`
9. Output: HDRImage (ACEScg-linear) + depth map + timing map + StatisticsCookie

## Subsystems

### Scene (`scene/`)

- `Scene` -- owns SceneObjects, builds scene-level KD-tree via `buildSceneGraph()`
- `SceneObject` -- Form + ObjectShader + transform + ID string
- `Camera` -- perspective projection, view/model matrices, frustum plane computation
- `Resources` -- loads OFF meshes and HDR/PPM textures

### Geometry (`geometry/`)

- `Form` (base) -> `Sphere`, `Box`, `Plane`, `Mesh`
- `Mesh` loads `.off` files, builds per-mesh KD-tree, stores vertices/facets/normals/texcoords
- `FacetIntersection` -- model-space intersection data (position, geometric normal, tex coords)
- `SceneIntersection` -- world-space data (smoothed normal, reflection direction, parent object ref)

### KD-Tree (`geometry/partitioning/`)

Balancers (all extend `KDTreeBalancer`):

- `BruteForceSAHKDTreeBalancer` -- exhaustive SAH, best quality (default)
- `FixedIterationsSAHKDTreeBalancer` -- bounded SAH iterations
- `MedianKDTreeBalancer`, `ArithmeticMeanKDTreeBalancer` -- simple split strategies
- `MaxAxisKDTreeBalancer`, `RotatingAxisKDTreeBalancer` -- axis selection strategies

Traversers (template on intersection type):

- `NaiveKDTreeTraverser<T>` -- recursive depth-first with early termination
- `VoxelizationKDTreeTraverser<T>` -- uses box entry/exit points

### Shading (`shading/`)

- `SceneShader` -- scene-level: background shader, ambient light, light collection, shadow computation
- `ObjectShader` -- per-object: 7 `MaterialShader` slots (diffusion, reflectance, specular, roughness,
  transmittance, refractionEta, emittance) + texture transform
- `Shader<C, I, O>` -- template base for all shaders (Containment, Intersection -> Output)
- Concrete shaders: `ConstShader`, `HDRImageShader`, `EnvironmentShader`, `IntersectionNormalShader`,
  `MultiplyByConstShader`, `ConcatenatingShader`, `NoiseGeneratorMap<Octaves>`

### BRDF (`shading/brdf/`)

- `ggxD()` -- Trowbridge-Reitz normal distribution
- `ggxG_smith()` -- Smith height-correlated geometry term
- `evaluateGGX()` -- full microfacet BRDF (D * G * F / 4NoVNoL)
- `sampleGGX()` -- VNDF importance sampling (Heitz 2018)

### Color (`shading/color/`)

- `primaries.h` -- sRGB/ACEScg/Rec.2020 <-> XYZ matrices, Bradford chromatic adaptation
- `view_transform.h` -- `agx()` (Sobotka AgX), `acesFilmic()` (Narkowicz 2015), `srgbEncode()`/`srgbDecode()`

### Common (`common/`)

- `RGBS` -- RGB color wrapper over Float4 with arithmetic operators
- `HDRImage` -- Float4-per-pixel image with bilinear sampling
- `Bitmap` -- LDR image (8-bit per channel)
- `PNGWriter` -- PNG file output (uses libpng)
- `PerlinNoiseGenerator`, `SimplexNoiseGenerator` -- procedural noise
- `RefractionIndices` -- physical IOR constants (Vacuum, Air, Water, Glass, Diamond, etc.)
- `StatisticsCookie` -- ray counts, KD-tree visits, cycle timings
