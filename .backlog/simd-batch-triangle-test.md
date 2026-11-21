# SIMD batch triangle intersection test

## Motivation

The leaf intersection loop in `Mesh.cpp:66-92` tests triangles one at a time via
`nearestIntersectionHavel()`. With KD-tree leaf size of 16 primitives, each leaf visit loops up to 16
iterations, each testing a single triangle. With SoA triangle data (see `DONE/soa-mesh-layout.md`), 4 triangles
could be tested simultaneously using 128-bit SIMD (or 8 with AVX): load N plane normals, compute N dot
products in parallel, compare N distances at once.

The 4-wide batch test and 8-wide AVX blocks are done, see `DONE/simd-batch-triangle-test.md`. The open point
below remains.

## Open points

- The block test lives in `Mesh.cpp`; the key files below name `primitives/Facet.h` as its home. Move it there if
  another form needs it.

### Baseline: 8-wide blocks (`9e23c9b`)

Mesh benchmark, 10 alternating runs each in one session, median "best" in core cycles per ray; a move must not
regress the "8 lanes" column (re-measure it in the same session):

| Mesh    | Query   | Rays       | 4 lanes | 8 lanes | Change | 8 faster |
|---------|---------|------------|--------:|--------:|-------:|---------:|
| teapot  | nearest | coherent   |   366.8 |   320.5 | -12.6% |    10/10 |
| teapot  | any     | coherent   |   305.2 |   275.8 |  -9.6% |    10/10 |
| teapot  | nearest | incoherent |   995.1 |   931.4 |  -6.4% |    10/10 |
| teapot  | any     | incoherent |   676.2 |   659.3 |  -2.5% |     8/10 |
| dragon2 | nearest | coherent   |   403.8 |   373.0 |  -7.6% |    10/10 |
| dragon2 | any     | coherent   |   337.9 |   321.4 |  -4.9% |    10/10 |
| dragon2 | nearest | incoherent |   988.7 |   981.0 |  -0.8% |     5/10 |
| dragon2 | any     | incoherent |   686.1 |   681.0 |  -0.8% |     3/10 |

## Key files

- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` (leaf intersection loops)
- `sources/raytracing/include/raytracing/geometry/Facet.h` (intersection functions)
- `sources/primitives/include/primitives/Facet.h` (Facet data type)
