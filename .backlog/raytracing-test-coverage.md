# Test coverage for raytracing library

## Motivation

The vectorization library has excellent test coverage (120 files, edge cases for NaN/denormals/infinity).
However, the raytracing library has zero tests for:

- Ray-geometry intersection (Sphere, Box, Plane, Mesh, Facet)
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

## Notes

- Needs a test project added to CMake (similar to `tests/vectorization.native-test/`)
- Consider whether to use MS CppUnitTest (consistent with vectorization tests) or Catch2 (CMake module
  already in `cmake/Catch2Tests.cmake`)
- Intersection tests should cover edge cases: tangent rays, rays parallel to surfaces, rays originating
  inside geometry, degenerate triangles

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/` (Sphere, Box, Plane)
- `sources/raytracing/include/raytracing/geometry/forms/meshes/` (Mesh, Facet)
- `sources/raytracing/include/raytracing/geometry/partitioning/` (KD-tree)
- `sources/raytracing/include/raytracing/shading/` (BRDF, shaders)
- `sources/raytracing/include/raytracing/scene/` (Camera, Scene)
