# SIMD batch triangle intersection test (done parts)

Carved out of `.backlog/simd-batch-triangle-test.md`. Moving the block test into `primitives` stays open there.

## Motivation

The leaf intersection loop in `Mesh.cpp:66-92` tests triangles one at a time via
`nearestIntersectionHavel()`. With KD-tree leaf size of 16 primitives, each leaf visit loops up to 16
iterations, each testing a single triangle. With SoA triangle data (see `DONE/soa-mesh-layout.md`), 4 triangles
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

## 8-wide blocks with AVX

- 8-wide blocks with AVX (`Float8`): the mean leaf holds 12 facets, so more lanes would be masked. Measure against
  the 4-wide blocks.

### Resolution

The block code takes its lane type from one alias, `FacetLanes = Float8` in `Mesh.h`. A block is then 8 entries,
12 `Float8` (384 B, 6 cache lines). Under `VECTORIZATION_HIGH_PRECISION` this needs a double `Float8` (`v_f64_8`, see
`implement-planned-vector-types.md`), like the spectral code that already uses `Float8`.

- Results unchanged at 8 lanes: same hits and checksums, `MeshTest::leafBlocksMatchNodeLoop` passes, and the Cornell
  box image is byte-identical. The 8-wide multiply-adds use the same fused instructions as the 4-wide ones.
- Mesh benchmark, 10 alternating runs each in one session, median "best" in core cycles per ray; "8 faster" counts
  the run pairs where 8 lanes won:

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

- 4 lanes measured 1-14% slower per query than in the 4-wide timings of `DONE/soa-mesh-layout.md`: a median here,
  the lower of two runs there, and a noisier machine. Compare within this table only.
- Dragon2 incoherent is neutral: those rays reach random leaves, and 8-lane blocks read more cache lines per visit (a
  12-entry leaf at a random offset covers about 2.4 blocks of 6 lines instead of 3.75 blocks of 3).
- Cornell box, first frame, five alternating runs each: median 46.1 → 44.7 ms, inconclusive (single frames ranged
  36-56 ms in this session).

## Commits

- `5c8a256` perf(raytracing): test mesh leaves in 4-wide facet blocks
- `9e23c9b` perf(raytracing): test mesh leaves in 8-wide facet blocks
