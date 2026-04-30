# Test coverage for raytracing library

## Motivation

The vectorization library has excellent test coverage (120 files, edge cases for NaN/denormals/infinity).
The raytracing library now has 226 tests in `tests/raytracing.native-test/` covering closed-form
intersection (Sphere, Box, Plane, Mesh/Facet), KD-tree balancer + traversal, the shading pipeline
(Fresnel, Beer-Lambert, Phong, BRDF, IntersectionNormalShader), Camera ray generation, and
Bitmap/HDRImage I/O round-trips. Remaining gaps and testability improvements surfaced during the
work are tracked in `.backlog/raytracing-testability.md`.

Any refactoring of the raytracing layer previously had no safety net beyond a visual comparison of
`ray-traced.bmp` output and stdout ray counts. The new tests catch numerical regressions adjacent
to (and partially through) the production code paths.

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
- Mesh/Facet intersection: 25 tests via `Mesh::buildTriangleMesh()` / `buildCubeMesh()` /
  `buildPlaneMesh()` (hit/miss for triangle and cube, recentered/scaled bounding verification, normal
  and vertex output, texCoords, frontface/backface culling, maxDistance boundary, self-occlusion,
  smoothed normal, model-space matches world-space, ray parallel to plane). Uses the brute-force
  fallback path; the KD-tree traverser path is exercised by the Traverser tests below.
- KD-tree balancer correctness: 17 tests (empty input → null tree, single primitive → leaf, below
  maxNodesSize stays leaf, above cost threshold splits, root bounding union, all inputs reach leaves,
  coincident primitives terminate cleanly, ArithmeticMean/MaxAxis balancers split, max-axis split
  selection, depth grows with primitive count, KDTreeBounding::split correctness for midpoint and
  edge t=0, findMinimumBoundingOfGeometry union and single-primitive)
- KD-tree traversal: 15 tests via custom `BoxNodesTraverser` over Box geometry (nearest hit picks
  closest, picks-not-first, miss, ray missing tree bounds early, inside-tree-bounds origin, max
  distance pruning and just-within boundary, findAny hit/miss/maxDistance, diagonal ray, self-
  occlusion advances to next, hit from behind, intersection.vertex set, 32 boxes across multiple
  splits all reachable through traversal)
- Schlick-Fresnel + refraction/reflection: 13 tests (R0 at normal incidence for water/glass, grazing-
  angle approach to 1, monotonicity, unit-range, dense->less-dense Snell branch, refract air->glass
  direction, total internal reflection returns zero, normal-incidence pass-through, equal-eta
  identity, reflect at horizontal surface and at normal incidence)
- Beer-Lambert: 7 tests (zero distance -> 1, large distance -> 0, unit-d/unit-T -> 1/e, per-channel
  independence, monotonic decay, large transmittance coefficient, range stays in [0,1])
- Phong specular: 8 tests (aligned -> 1, perpendicular -> 0, opposite clamped, narrowing lobe with
  shininess, non-negative, range in [0,1], reflect-then-Phong consistency, shininess=1 returns dot)
- BRDF combine (formula from `Raytracer::applyBRDF`): 7 tests (white-light surface, emittance pass-
  through, diffusion*ambient, specular*lighting, mix-coefficient endpoints select reflection vs
  transmission, all-zero produces zero)
- IntersectionNormalShader: 6 tests (normal-to-color mapping, negative -> 0, +1 -> 1, zero -> 0.5,
  operator() matches sample(), arbitrary linear mapping)
- Camera ray generation: 18 tests (default state, frustum population, square/wide/tall aspect, FOV
  scaling, translate/rotate/resetView, center & corner pixel rays via the RaytracerPackets formula,
  symmetric corners, far-plane span vs near-plane span)
- Bitmap I/O: 15 tests (default 1x1, stride padding to 4 bytes, operator[], loadPPM round-trip from
  hand-written P6, missing-file/non-P6 rejection, Y-axis flip on load, saveAsBMP/saveAsPNG magic
  bytes, 1x1/wide/tall edge cases, all-black/all-white round-trips, bad path)
- HDRImage I/O round-trip: 15 tests (default, resolution, operator[], Bitmap → HDRImage normalised
  values, full Bitmap → HDR → Bitmap round-trip, HDR-range storage of values >1 / <0, clamping in
  HDR → Bitmap, NaN and infinity preservation, normalizeEachChannel, sampleBilinear at texel,
  resolution preservation, 32x32 round-trip)

## Findings (locked-in current behavior)

- `Mesh::buildXxxMesh` static helpers default-construct without a traverser. To use the tree-driven
  traversal path, callers must either go through a Mesh constructed via `Mesh(traverser, balancer)`
  + `loadFromOffFile` (the only public path) or a separate `KDTreeRoot` + `GeometryNodesTraverser`
  pair. The Traverser tests use the latter via Box leaves.
- `Mesh::setupMesh` calls `computeStandardMesh` which recenters and uniformly rescales the input
  vertices. `buildTriangleMesh` produces vertices in z=0 with bounds `[-0.5, -0.5, 0]..[0.5, 0.5, 0]`
  (apex at +y, base at -y). `buildCubeMesh` produces `[-0.5, 0.5]^3`. Tests assert the rescaled
  geometry, not the input.
- Mesh intersection requires `ray.origin.w == 1` for Havel's algorithm to compute the correct plane-
  ray distance via the homogeneous dot product. Sphere/Box/Plane tolerate `w=0`.
- Havel's barycentric check (`isNegative(detU ^ checkU)`) rejects hits exactly on a triangle edge.
  Cube and plane mesh tests use off-axis ray xy to avoid the diagonal between paired triangles.
- Triangle vertex winding `(-0.5,-0.5,0), (0,0.5,0), (0.5,-0.5,0)` produces a flat normal of
  `(0,0,-1)` (cross of edge1, edge2 in this order). This means the triangle's "front" face is the -z
  side; tests assert the actual normal direction.
- KDTreeBalancer's default `maxNodesSize=16` plus the SAH-style cost post-check means even 20
  primitives can stay as a single leaf; tests use 64 primitives where splits are guaranteed.
- Production shading helpers `schlickFresnel`, `fresnelReflectance`, `phongSpecularIntensity`, and
  `applyBRDF` are file-local statics / private members. The shading tests reproduce the formulas via
  public primitives (`refract`, `reflect`, `mix`, `pow3`, `exp`) to lock in mathematical behavior;
  refactors of the production helpers must update the tests in lockstep.

## Remaining Notes

- HDRImageShader and EnvironmentShader still need targeted tests (require an HDRImage fixture)
- Bitmap I/O is asymmetric (only `loadPPM`, no `savePPM`; `saveAsBMP` / `saveAsPNG` have no matching
  loaders), so the Bitmap suite uses hand-written P6 files for the load round-trip and asserts magic
  bytes for the save side. HDRImage has no file I/O of its own, so the HDR round-trip exercises the
  `Bitmap ↔ HDRImage` conversion constructors.
- Test runner quirk: `vstest.console.exe` aborts the test host process at tear-down whenever
  OpenMP-parallelized paths (`loadPPM`, `Bitmap ↔ HDRImage` ctors, `normalizeEachChannel`, raytrace
  loops) have run. All assertions still pass, but the last 1–2 results may be truncated. Per-class
  runs are clean. See `.backlog/raytracing-testability.md` for the planned investigation.
- The Mesh KD-tree-driven path (with traverser) is not directly testable through `Mesh::buildXxxMesh`
  helpers — see `.backlog/raytracing-testability.md` for the proposed API change

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/` (Sphere, Box, Plane)
- `sources/raytracing/include/raytracing/geometry/forms/meshes/` (Mesh, Facet)
- `sources/raytracing/include/raytracing/geometry/partitioning/` (KD-tree)
- `sources/raytracing/include/raytracing/shading/` (BRDF, shaders)
- `sources/raytracing/include/raytracing/scene/` (Camera, Scene)
