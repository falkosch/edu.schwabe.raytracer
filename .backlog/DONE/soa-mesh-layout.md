# Convert mesh data from AoS to SoA layout (done parts)

Carved out of `.backlog/soa-mesh-layout.md`. Replacing `MeshGeometryNode`'s facet copy by an index stays open there.

## Motivation

`Mesh` stores `std::vector<Facet>` as Array-of-Structures — each `Facet` packs 3 vertices (v0, v1, v2)
contiguously. This layout prevents SIMD batch intersection (`simd-batch-triangle-test.md`), because loading
the same component (e.g. all v0.x values) across 4 triangles requires gathering from non-contiguous memory.

The same concern applies to `planeNormals`, `flatNormals`, `smoothNormals`, `facetEdges`, and `texCoords` —
all currently stored as AoS vectors in `Mesh`.

## Layout options (must be benchmarked before committing)

The right layout is not obvious without measurement. Single-ray traversal currently visits facets in
KD-tree leaf order (effectively random), so a pure component-wise SoA layout will *gather* on every
facet access and likely regress the single-ray path until the batch-intersection follow-up lands.
Different layouts trade single-ray cost against batch-intersection feasibility.

### Path A — Full component-wise SoA (max SIMD-batch friendly)

Replace each `std::vector<Facet>` with 12 separate `std::vector<float>` (`v0x`, `v0y`, `v0z`, `v0w`,
`v1x`, ..., `v2w`). Same expansion for `planeNormals` (12 floats per facet), `flatNormals` (4),
`smoothNormals` (12), `facetEdges` (8), `texCoords` (12).

- Pros: contiguous `_mm_load_ps` of 4 consecutive facets' v0.x — directly unlocks batch intersection
- Cons: every single-facet access becomes a 12-component gather; more cache lines touched per facet
  in the SISD path; intersection helper signatures balloon (or need a wrapper struct)
- Open question: is the regression on the SISD path tolerable until the batch path is in place?
  Need a benchmark before/after with the existing `nearestIntersectionHavel` loop.

### Path B — Slot-wise SoA (`v0[]`, `v1[]`, `v2[]` arrays of `Float4`)

Keep `Float4` storage but split each `Facet` field into three separate arrays. Cheap to implement,
nearly zero churn to call sites that take `Float4` by value.

- Pros: minimal code change; no per-facet gather cost; still aligned 16-byte loads
- Cons: does NOT allow contiguous `_mm_load_ps(&v0x[base])` of 4 facets' v0.x — i.e. it does not
  actually unlock the batch-intersection use case the parent task names
- This path has no real benefit unless paired with a batched re-layout step; document and probably
  reject after measurement.

### Path C — Block-SoA (a.k.a. AoSoA, SIMD-width tiles)

Group facets into blocks of 4 (SSE) or 8 (AVX). Within a block the data is component-wise SoA
(`v0x[0..3]` packed as one `Float4`, etc.); across blocks it's an array. Equivalent to storing the
mesh as `std::vector<FacetBlock4>` where `FacetBlock4` holds 12× `Float4` (one lane per facet).

- Pros: contiguous SIMD load *within* a block (batch-intersection friendly); only one cache line
  per block in the SISD path (better locality than Path A); easier to retrofit alignment
- Cons: KD-tree leaves reference facets by global index; inside a block we'd need partial-mask
  intersection if the leaf only references a subset; reordering facets by KD-tree leaf
  improves locality but complicates `MeshGeometryNode::index`
- Probably the most interesting option to benchmark; matches what `embree` and similar use.

### Path D — Hybrid: keep AoS `Facet`, add SoA mirror only where batch loads need it

Leave `std::vector<Facet>` etc. in place for the SISD path. Add separate SoA mirror arrays
(component-wise or block-SoA) populated alongside, used only by the batch traverser when it lands.

- Pros: zero regression on the SISD path; the SoA layout is purely additive; can be implemented
  per-field as needed (e.g. only `planeNormals` SoA initially)
- Cons: roughly 2× memory for the mirrored fields; bookkeeping to keep mirror in sync on
  scene mutation; "two ways to do it" cognitive load
- The conservative choice if benchmarks show Path A regresses too much.

## Cross-cutting design choices (independent of layout path)

### `Facet` / `FacetNormals` / `FacetEdges` struct lifetime

These structs are used outside `Mesh`: `Plane.h::baseVectors`, `MeshGeometryNode::facet`,
`computeNodesAndGraph` constructs `MeshGeometryNode` from a `Facet`, and single-facet intersection
helpers (`nearestIntersectionHavel`, `nearestIntersectionMoeller`) take them by const ref. They
should stay; SoA paths add `gather*(soa, i)` helpers that reconstruct an AoS struct on demand for
SISD callers.

### Encapsulation of SoA fields

12+ parallel `std::vector<float>` members on `Mesh` is unwieldy. Wrap them in a small
`MeshFacetSoA` (and `FacetNormalsSoA`, `FacetEdgesSoA`, etc.) struct so `computeNormals` and
`computeFacetIntersection` parameter lists don't explode. Independent of which path is chosen.

### KD-tree leaf access pattern is the hot constraint

The SISD intersection loop in `Mesh.cpp::findNearestIntersection` accesses facets via
`MeshGeometryNode::index` — the order is determined by KD-tree traversal, not by index. Any layout
that assumes "consecutive indices touched together" (Path C in particular) needs the KD-tree
balancer to preserve or restore that locality, e.g. by reordering facets so that leaf-adjacent
facets are also block-adjacent.

## Recommended sequencing

1. **Microbenchmark first** — extract a minimal harness around the current `nearestIntersectionHavel`
   loop on `meshes/teapot.off` (already in the regression baseline: 0.067s for 512×512 first frame).
   Establish a stable baseline.
2. **Prototype Path A on `planeNormals` only** (the hottest field — 3 `Float4` per facet, used in
   every Havel test). Measure SISD-path regression in isolation.
3. **If regression < ~5%**, expand Path A to remaining fields; proceed to `simd-batch-triangle-test.md`.
4. **If regression too large**, prototype Path C (block-SoA, blocks of 4) on `planeNormals`. Same
   measurement.
5. **If both regress unacceptably**, fall back to Path D and document that SoA is mirror-only.
6. Each benchmark result should land as a comment in this file before the next path is attempted.

## Measurements

### Leaf loop share (2026-11-21, on top of `ef841e4`)

Probe in `benchmarks --only=meshes`, not committed:

- An untimed pass records every leaf call of the production traversal: the ray, the `maxDistance` handed to the leaf,
  and the span.
- The timed passes replay only those calls into `Mesh::findNearestIntersection(span, ...)` /
  `findAnyIntersection(span, ...)`. They use the same chunked best-of-15 timing as the traced passes.
- "Empty" replays the same calls with empty spans, which leaves the per-call cost.
- The recording pass also repeats the leaf loop to count what it executes. Its results matched the production leaf
  results in all runs (0 mismatches).

Two alternating runs per build. The table gives the lower "best" of the two in core cycles per ray. `static_cast` is
the same code with `static_cast` instead of the `dynamic_cast` in both leaf loops.

| Mesh    | Query   | Rays       | Traced |    Leaf replay | Empty | Havel tests |  Traced, static |  Leaf, static |
|---------|---------|------------|-------:|---------------:|------:|------------:|----------------:|--------------:|
| teapot  | nearest | coherent   |  690.5 |  526.4 (76.2%) |  23.6 |        8.70 |  484.2 (-29.9%) | 320.2 (66.1%) |
| teapot  | any     | coherent   |  507.0 |  340.6 (67.2%) |  18.0 |        5.38 |  359.5 (-29.1%) | 200.4 (55.7%) |
| teapot  | nearest | incoherent | 1871.0 | 1229.2 (65.7%) |  42.3 |       17.22 | 1520.9 (-18.7%) | 844.7 (55.5%) |
| teapot  | any     | incoherent | 1055.6 |  591.7 (56.1%) |  31.2 |        7.68 |  897.0 (-15.0%) | 393.7 (43.9%) |
| dragon2 | nearest | coherent   |  759.2 |  499.8 (65.8%) |  12.6 |        4.96 |  594.2 (-21.7%) | 342.4 (57.6%) |
| dragon2 | any     | coherent   |  504.6 |  264.8 (52.5%) |   7.7 |        2.62 |  403.2 (-20.1%) | 178.0 (44.1%) |
| dragon2 | nearest | incoherent | 1766.0 | 1002.9 (56.8%) |  21.9 |        8.61 | 1433.4 (-18.8%) | 718.8 (50.1%) |
| dragon2 | any     | incoherent |  927.6 |  371.8 (40.1%) |  11.7 |        2.89 |  816.8 (-11.9%) | 273.5 (33.5%) |

"Havel tests" counts the tests executed per ray, after culling and the any-hit early exit. Coherent rays cull 22-28%
of the entries.

- The leaf loop is 40-76% of a ray, so it isn't too small to optimize.
- The `dynamic_cast` per entry alone costs 12-30% of the whole ray. Spread over the Havel tests, that is 22-26 cycles
  per test on teapot and 32-36 on dragon2.
- With `static_cast`, (leaf - empty) per Havel test is 34-48 cycles on teapot, whose data fits in L2, and 65-89
  cycles on dragon2. Each entry makes three scattered reads: the heap-allocated `MeshGeometryNode` (for `index`),
  `flatNormals[index]` and `planeNormals[index]`. The teapot/dragon2 gap suggested that on dragon2 these misses cost
  more than the arithmetic.
- The expectation before the steps below: contiguous per-leaf hit data pays off even before SIMD (dragon2 above all),
  and the 4-wide batch only cuts the arithmetic, less than 34 cycles per test. The outcome after the total corrects
  this.

### Step 1: per-entry copies in leaf order

`Mesh` keeps a copy of `planeNormals` and the facet index for every element of `KDTreeRoot::leafGeometry`, in the same
order. A leaf is addressed by its span's offset in `leafGeometry`, so neither the traversers nor the leaf callback
change. Spans outside `leafGeometry` (the node list of a mesh without KD-tree) keep the single-facet loop over the
nodes.

- Culling uses the sign of `dot3(planeNormals.v0, direction)` instead of `flatNormals`: `flatNormals` is
  `normalize3(planeNormals.v0)`. The probe counted 0 sign differences on both meshes.
- Same scalar Havel loop otherwise; 0 mismatches, same hits and checksums.

Two alternating runs each against the `static_cast` build, lower "best" in core cycles per ray; "per test" is
(leaf replay - empty) per executed Havel test:

| Mesh    | Query   | Rays       | Traced, static | Traced, packed | Change | Per test, static | Per test, packed |
|---------|---------|------------|---------------:|---------------:|-------:|-----------------:|-----------------:|
| teapot  | nearest | coherent   |          490.2 |          442.4 |  -9.8% |             32.8 |             28.9 |
| teapot  | any     | coherent   |          359.1 |          329.5 |  -8.2% |             29.5 |             28.7 |
| teapot  | nearest | incoherent |         1506.3 |         1355.9 | -10.0% |             45.5 |             39.3 |
| teapot  | any     | incoherent |          886.8 |          828.9 |  -6.5% |             48.2 |             41.2 |
| dragon2 | nearest | coherent   |          578.9 |          541.1 |  -6.5% |             65.6 |             57.4 |
| dragon2 | any     | coherent   |          413.1 |          381.7 |  -7.6% |             61.7 |             59.4 |
| dragon2 | nearest | incoherent |         1466.1 |         1199.3 | -18.2% |             82.5 |             62.5 |
| dragon2 | any     | incoherent |          825.6 |          707.2 | -14.3% |             91.8 |             68.6 |

### Step 2: blocks of 4 with a batch Havel test (Path C in leaf order)

The copies become component-wise blocks of 4 consecutive entries (`FacetNormalsBlock`: 12 `Float4`, 3 cache lines),
still in `leafGeometry` order. Blocks run across leaf boundaries: a leaf is the entry range [offset, offset + size),
and the lanes of its first and last block outside that range are masked. So there is no per-leaf padding, no leaf →
block table and no duplication beyond the tree's own (`kdtree-leaf-packed-facets.md`).

- `testBlock` is `nearestIntersectionHavel` lane-wise, with the same operations in the same order. The dot products
  are summed like `_mm_dp_ps` in `dotv` / `dot3v`. Hits are taken in entry order against the nearest hit so far (a lane
  behind a nearer hit in the same block is tested again), so the results are bit-identical.
- Verified with MSVC (clang-cl not checked): 0 mismatches, same hits and checksums, the Cornell box image
  byte-identical. `MeshTest::leafBlocksMatchNodeLoop` compares the block path with the single-facet loop for every
  sub-range of a leaf (ranges starting and ending mid-block), each culling mode and each facet of the range as the
  origin. Removing the origin lane mask makes it fail.

Two alternating runs each against step 1:

| Mesh    | Query   | Rays       | Traced, packed | Traced, block | Change | Per test, packed | Per test, block |
|---------|---------|------------|---------------:|--------------:|-------:|-----------------:|----------------:|
| teapot  | nearest | coherent   |          447.6 |         352.4 | -21.3% |             29.4 |            17.5 |
| teapot  | any     | coherent   |          331.7 |         267.6 | -19.3% |             29.4 |            16.5 |
| teapot  | nearest | incoherent |         1383.3 |         988.8 | -28.5% |             40.5 |            15.1 |
| teapot  | any     | incoherent |          850.4 |         624.9 | -26.5% |             37.2 |            11.8 |
| dragon2 | nearest | coherent   |          548.9 |         383.4 | -30.2% |             57.5 |            23.0 |
| dragon2 | any     | coherent   |          373.7 |         312.5 | -16.4% |             58.2 |            24.5 |
| dragon2 | nearest | incoherent |         1199.6 |         953.1 | -20.5% |             60.8 |            29.0 |
| dragon2 | any     | incoherent |          711.2 |         623.0 | -12.4% |             64.3 |            33.7 |

"Per test" now counts executed tests of the single-facet loop; the block test runs all lanes of a block at once.

### Total

Original code (`ef841e4`, `dynamic_cast`, scattered reads) against step 2, two alternating runs each in one session:

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

Outcome per test: contiguous data (step 1) saved 1-7 cycles on teapot and 2-23 on dragon2, most on incoherent rays.
The batch test (step 2) saved 12-25 on teapot and 31-35 on dragon2, at or above the bound expected for arithmetic
alone. So the scattered reads were not the main per-test cost. What the batch removes besides arithmetic isn't
measured; likely candidates are the data-dependent early-exit branches of the single-facet test and the per-entry loop
overhead.

Cornell box, first frame, five alternating runs each. "Old" is the leaf-size experiment build, the same code as
`ef841e4`. Median 36.6 → 32.2 ms (-12%), best 33.6 → 31.6 ms (-6%). Single frames are noisy (old 33.6-45.5 ms, new
31.6-38.6 ms). The image was byte-identical in all ten runs.

Memory: the blocks and indices add 52 B per leaf entry (dragon2: 351507 entries, about 18.3 MB). `planeNormals` is
now only read by the single-facet loop for meshes without KD-tree.

### Resolution

Path C (block-SoA) for the hit-test data only, in KD-tree leaf order: `FacetNormalsBlock`s hold the plane normals of
`KDTreeRoot::leafGeometry`'s entries, 4 per block, plus a facet index per entry (steps 1 and 2 above). The other
per-facet arrays (`facets`, `flatNormals`, `smoothNormals`, `texCoords`, `facetEdges`) stay AoS: they are read once
per hit, for the winning facet only. So the `Facet` / `FacetNormals` / `FacetEdges` structs stay, and no SoA wrapper
structs or gather helpers were needed. The 4-wide test is in `DONE/simd-batch-triangle-test.md`; 8-wide AVX blocks
and moving the block test into `primitives` stay open in `simd-batch-triangle-test.md`.

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/meshes/Mesh.h` — member vector layout
- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` — SISD intersection loops, `clear`, `setupMesh`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/facets.cpp` — `computeFacets`, `computeNormals`,
  `computeNodesAndGraph`, `computeFacetIntersection`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/textureCoordinates.cpp` — `computeTexCoordsOrtho`,
  `computeTexCoordsSpherical`
- `sources/raytracing/include/raytracing/geometry/forms/meshes/MeshGeometryNode.h` — owns a `Facet` copy; consider
  replacing with index-only reference into the SoA arrays
- `sources/primitives/include/primitives/Facet.h` — keep struct, possibly add gather constructor
- `sources/primitives/include/primitives/FacetEdges.h` — keep struct

## Commits

- `5c8a256` perf(raytracing): test mesh leaves in 4-wide facet blocks
