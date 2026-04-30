# Test coverage for raytracing library

## Motivation

The vectorization library has excellent test coverage (120 files, edge cases for NaN/denormals/infinity).
The raytracing library now has intersection tests for closed-form geometry (Sphere, Box, Plane — 50 tests
in `tests/raytracing.native-test/`), but still lacks tests for:

- Mesh/Facet intersection (requires KD-tree traverser + balancer plumbing)
- KD-tree building and traversal
- Shading/BRDF calculations (Schlick-Fresnel, Beer-Lambert, Phong)
- Scene management, camera, image processing

Any refactoring of the raytracing layer has no safety net. The current regression test is visual
(comparing `ray-traced.bmp` output and stdout ray counts), which catches gross errors but not subtle
numerical regressions.

## Scope

- Ray-geometry intersection tests for each form (Sphere, Box, Plane, Mesh/Facet)
- KD-tree balancer correctness (split plane selection, geometry partitioning)
- KD-tree traversal (nearest intersection, any intersection, edge cases)
- Shading pipeline (BRDF, Fresnel, refraction)
- Camera ray generation
- Bitmap/HDRImage I/O round-trips

## Done

- Test project `tests/raytracing.native-test/` added (MS CppUnitTest, mirroring existing pattern)
- Sphere intersection: 28 tests (hit, miss, tangent, inside, frontface/backface culling, maxDistance
  boundary, self-occlusion, exact texCoords, smoothed/model-space normals, NaN direction, infinity origin)
- Box intersection: 27 tests (all 6 face normals, miss, edge hit with output verification, inside,
  frontface/backface culling, maxDistance boundary, self-occlusion, texCoords, smoothed normals)
- Plane intersection: 25 tests (hit, parallel miss, both directions, frontface/backface culling, maxDistance
  boundary, ray on plane, self-occlusion, model-space/smoothed normals, NaN on-plane parallel)

## Remaining Notes

- Mesh tests need KD-tree traverser + balancer wired up — consider using `Mesh::buildCubeMesh()` or
  `Mesh::buildTriangleMesh()` static helpers
- Intersection tests should cover degenerate triangles
- Shading tests may need Scene/SceneObject scaffolding

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/` (Sphere, Box, Plane)
- `sources/raytracing/include/raytracing/geometry/forms/meshes/` (Mesh, Facet)
- `sources/raytracing/include/raytracing/geometry/partitioning/` (KD-tree)
- `sources/raytracing/include/raytracing/shading/` (BRDF, shaders)
- `sources/raytracing/include/raytracing/scene/` (Camera, Scene)
