# SIMD batch triangle intersection test (done parts)

Carved out of `.backlog/simd-batch-triangle-test.md`. 8-wide AVX blocks and moving the block test into
`primitives` stay open there.

## Motivation

The leaf intersection loop in `Mesh.cpp:66-92` tests triangles one at a time via
`nearestIntersectionHavel()`. With KD-tree leaf size of 16 primitives, each leaf visit loops up to 16
iterations, each testing a single triangle. With SoA triangle data (see `soa-mesh-layout.md`), 4 triangles
could be tested simultaneously using 128-bit SIMD (or 8 with AVX): load N plane normals, compute N dot
products in parallel, compare N distances at once.

## Prerequisites

- SoA mesh data layout (`DONE/soa-mesh-layout.md`, done) — AoS `std::vector<Facet>` prevents SIMD-width loads.

## Proposed fix

1. Restructure the leaf loop to process triangles in batches of 4 (SSE) or 8 (AVX).
2. Load plane normals for the batch into SIMD registers (transposed from SoA arrays).
3. Compute `nearestIntersectionHavel` for 4/8 triangles in parallel.
4. Horizontal reduction to find the nearest hit across the batch.
5. Scalar tail loop for remaining triangles (count not divisible by batch size).

### Resolution

4-wide, in `Mesh.cpp` (`testBlock`, `nearestHitInBlocks`, `anyHitInBlocks`) on the `FacetNormalsBlock`s of
`DONE/soa-mesh-layout.md`, which also has the timings:

1. A batch is a block of 4 consecutive leaf entries. Blocks run across leaf boundaries; lanes outside the leaf and
   the origin's lane are masked, so there is no scalar tail loop.
2. The blocks are stored component-wise at build time, so the test loads them without transposing.
3. `testBlock` is `nearestIntersectionHavel` lane-wise, with the same operations in the same order (dot products
   summed like `_mm_dp_ps`), so every lane gets the single-facet result. Culling uses the sign of the determinant.
4. No horizontal minimum: hit lanes are taken in entry order against the nearest hit so far, which keeps the
   single-facet loop's tie-breaking. Results are bit-identical (checked with MSVC).

Mesh benchmark: 12-30% fewer core cycles per ray than the contiguous single-facet loop, 31-50% fewer than before
both changes.

## Commits

- `5c8a256` perf(raytracing): test mesh leaves in 4-wide facet blocks
