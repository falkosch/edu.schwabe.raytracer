# SIMD batch triangle intersection test

## Motivation

The leaf intersection loop in `Mesh.cpp:66-92` tests triangles one at a time via
`nearestIntersectionHavel()`. With KD-tree leaf size of 16 primitives, each leaf visit loops up to 16
iterations, each testing a single triangle. With SoA triangle data (see `soa-mesh-layout.md`), 4 triangles
could be tested simultaneously using 128-bit SIMD (or 8 with AVX): load N plane normals, compute N dot
products in parallel, compare N distances at once.

The 4-wide batch test is done, see `DONE/simd-batch-triangle-test.md`. The open points below remain.

## Open points

- 8-wide blocks with AVX (`Float8`): the mean leaf holds 12 facets, so more lanes would be masked. Measure against
  the 4-wide baseline below.
- The block test lives in `Mesh.cpp`; the key files below name `primitives/Facet.h` as its home. Move it there if
  another form needs it.

### Baseline: 4-wide blocks (`5c8a256`)

Mesh benchmark, the lower "best" of two alternating runs in core cycles per ray, against the code before the block
layout (from `DONE/soa-mesh-layout.md`):

| Mesh    | Query   | Rays       | Traced, original | Traced, block | Change | Per test, original | Per test, block |
|---------|---------|------------|-----------------:|--------------:|-------:|-------------------:|----------------:|
| teapot  | nearest | coherent   |            701.1 |         354.0 | -49.5% |               51.0 |            16.9 |
| teapot  | any     | coherent   |            510.3 |         267.6 | -47.6% |               59.3 |            14.5 |
| teapot  | nearest | incoherent |           1887.4 |         972.4 | -48.5% |               67.4 |            15.0 |
| teapot  | any     | incoherent |           1090.7 |         627.8 | -42.4% |               69.7 |            12.1 |
| dragon2 | nearest | coherent   |            768.6 |         398.7 | -48.1% |               97.0 |            23.3 |
| dragon2 | any     | coherent   |            507.5 |         316.8 | -37.6% |              103.8 |            21.8 |
| dragon2 | nearest | incoherent |           1762.9 |         977.8 | -44.5% |              115.9 |            27.9 |
| dragon2 | any     | incoherent |            932.1 |         641.7 | -31.2% |              131.3 |            34.6 |

## Key files

- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` (leaf intersection loops)
- `sources/raytracing/include/raytracing/geometry/Facet.h` (intersection functions)
- `sources/primitives/include/primitives/Facet.h` (Facet data type)
