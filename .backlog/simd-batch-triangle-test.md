# SIMD batch triangle intersection test

## Motivation

The leaf intersection loop in `Mesh.cpp:66-92` tests triangles one at a time via
`nearestIntersectionHavel()`. With KD-tree leaf size of 16 primitives, each leaf visit loops up to 16
iterations, each testing a single triangle. With SoA triangle data (see `soa-mesh-layout.md`), 4 triangles
could be tested simultaneously using 128-bit SIMD (or 8 with AVX): load N plane normals, compute N dot
products in parallel, compare N distances at once.

## Prerequisites

- SoA mesh data layout (`soa-mesh-layout.md`) — AoS `std::vector<Facet>` prevents SIMD-width loads.

## Proposed fix

1. Restructure the leaf loop to process triangles in batches of 4 (SSE) or 8 (AVX).
2. Load plane normals for the batch into SIMD registers (transposed from SoA arrays).
3. Compute `nearestIntersectionHavel` for 4/8 triangles in parallel.
4. Horizontal reduction to find the nearest hit across the batch.
5. Scalar tail loop for remaining triangles (count not divisible by batch size).

## Key files

- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` (leaf intersection loops)
- `sources/raytracing/include/raytracing/geometry/Facet.h` (intersection functions)
- `sources/primitives/include/primitives/Facet.h` (Facet data type)
