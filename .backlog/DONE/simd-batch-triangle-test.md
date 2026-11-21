# SIMD batch triangle intersection test

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

## Explicit FMA dot products (2026-11-21)

A search for hand-written dot products that mirror `dot3v` / `dotv` found that the block test's lane dot products
matched the single-facet test only as written. The compilers rewrite them (checked in a two-file LTCG / ThinLTO model
with the project flags):

- MSVC `/fp:fast` drops the `+ 0` that stood for `_mm_dp_ps`'s masked w term, so a zero determinant could come out as
  -0; without `/d2SSAOptimizer-` it also fuses the products into FMAs.
- clang-cl (`-ffp-contract=fast -fassociative-math -fno-signed-zeros`) fuses and reorders them.

So `dotLanes` / `dot3Lanes` are now explicit `multiplyAdd` chains (`Float8` has them), which no compiler setting
changes. The block path is no longer bit-identical to `nearestIntersectionHavel` (`_mm_dp_ps`, no FMA):

- Mesh benchmark: dragon2 coherent has one more hit (69838; facet 63095 at distance 7.874, a ray grazing an edge);
  everything else is unchanged. Timing -1.7% to 1.9% against the previous build, five alternating runs.
- Cornell box: same ray counts; 2 of 262144 pixels differ by 1 level in one channel.
- `MeshTest::leafBlocksMatchNodeLoop` now compares distances with a tolerance (1e-5) and still requires the same hit
  and facet.

## Block test in `primitives`

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

### Resolution

No other form needs it yet; it moved anyway, with one type per file instead of into `Facet.h`: `FacetLanes` (the lane
alias, `Float8`), `FacetNormalsBlock` with `intersectionsHavel` (formerly `testBlock`), `FacetBlockRay` (formerly
`RayLanes`) and `FacetBlockTest` with `texCoordsAndDistance`. The leaf-entry logic (candidate lanes, the nearest and
any-hit loops over a leaf's blocks, the packing) stays in `Mesh.cpp`.

- Results unchanged: hits, facet index sums and distance sums of all eight benchmark queries are identical, and
  `MeshTest::leafBlocksMatchNodeLoop` passes.
- New primitives tests (`FacetNormalsBlock_test.cpp`, `FacetBlockRay_test.cpp`, `FacetBlockTest_test.cpp`) check each
  lane against `nearestIntersectionHavel` plus the mesh's culling, with rays that cross the facets clear of their
  edges, and cover the candidate mask, `maxDistance`, facets behind the origin and the per-lane `{ u, v, distance }`.
- Mesh benchmark against the previous build (`cc5f1ca`, its "8 lanes" code re-measured in the same session), 10
  alternating runs each, median "best" in core cycles per ray. The machine ran faster than for the baseline above, so
  compare within this table only:

| Mesh    | Query   | Rays       | Mesh.cpp | primitives | Change | primitives faster |
|---------|---------|------------|---------:|-----------:|-------:|------------------:|
| teapot  | nearest | coherent   |    238.2 |      237.1 |  -0.5% |              6/10 |
| teapot  | any     | coherent   |    208.3 |      205.7 |  -1.3% |              6/10 |
| teapot  | nearest | incoherent |    600.5 |      601.2 |  +0.1% |              5/10 |
| teapot  | any     | incoherent |    435.4 |      432.8 |  -0.6% |              8/10 |
| dragon2 | nearest | coherent   |    257.2 |      256.4 |  -0.3% |              7/10 |
| dragon2 | any     | coherent   |    215.7 |      214.6 |  -0.5% |              6/10 |
| dragon2 | nearest | incoherent |    530.8 |      535.8 |  +1.0% |              4/10 |
| dragon2 | any     | incoherent |    419.4 |      419.7 |  +0.1% |              4/10 |

- Neutral (-1.3% to +1.0%, wins split): the call across the library boundary costs nothing measurable under LTCG.
- Not built with clang-cl (`clang-release`), and no Cornell box image comparison was run.

## Key files

- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` (leaf intersection loops)
- `sources/raytracing/include/raytracing/geometry/Facet.h` (intersection functions)
- `sources/primitives/include/primitives/Facet.h` (Facet data type)

## Commits

- `5c8a256` perf(raytracing): test mesh leaves in 4-wide facet blocks
- `9e23c9b` perf(raytracing): test mesh leaves in 8-wide facet blocks
- `32cf1df` fix(raytracing): use explicit broadcasts and FMA in the mesh block test
- `a7d2c29` refactor(primitives): move the mesh block test into primitives
