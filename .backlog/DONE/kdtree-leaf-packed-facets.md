# Per-leaf packed facet data for batch intersection

## Motivation

`DONE/simd-batch-triangle-test.md` wants to test 4 (or 8) facets of a KD-tree leaf in one SIMD op. That needs the
hit-test data of those facets in contiguous, lane-wise memory. `DONE/soa-mesh-layout.md` covers the global layout of the
mesh arrays, but a global layout alone doesn't give that: a leaf references arbitrary facet indices, and the reordering
idea in `DONE/soa-mesh-layout.md` ("reordering facets so that leaf-adjacent facets are also block-adjacent") can't be a
permutation, because leaves share facets.

Facets that straddle a split plane go into both children:

- `SAHKDTreeBalancer.cpp:22-38`: left gets every facet with `minimum[axis] <= split`, right every facet with
  `maximum[axis] >= split`.
- `KDTreeBalancer.cpp:202-232`: same through `overlaps` against both child boundings.
- `Balancer_test.cpp:98` documents it ("may be duplicated when straddling a split").

So the sum of leaf sizes exceeds the facet count, and no single order of the facet arrays makes every leaf
contiguous. None of the three items covers this step; it sits between them.

## Current leaf representation

- Since the flat-array refactor (`DONE/flat-array-kdtree-nodes.md`), a leaf is an offset/count range (`KDTreeLeaf`) into
  `KDTreeRoot::leafGeometry`, one flat array of `GeometryNode*`. `GeometryNodesTraverser` gets it as a
  `GeometryNodeSpan`; the span's offset from `leafGeometry.data()` identifies the leaf.
- `Mesh::findNearestIntersection(GeometryNodeSpan, ...)` (`Mesh.cpp:72`) and `findAnyIntersection` (`Mesh.cpp:123`)
  still do a `dynamic_cast<const MeshGeometryNode*>` per entry to read `index`, then index into the AoS arrays.
- The KD-tree types are generic: `SceneGeometry` uses the same tree for scene objects through the same
  `GeometryNodesTraverser` interface (`SceneGeometry.cpp:74`, `:96`). Packing is mesh-specific and must not leak into
  that path.

## What has to be packed

Only the data the per-facet test reads. Everything used for the winning facet stays AoS, indexed by facet index
(`computeFacetIntersection` reads `texCoords`, `flatNormals`, `smoothNormals` and `nodes` once per leaf visit):

- `planeNormals`: 3 `Float4` = 12 floats per facet, the whole Havel test input.
- The original facet index per lane, for the self-occlusion check and for `computeFacetIntersection`.

Two per-facet checks in the leaf loop run before the Havel test and become lane masks:

- Culling doesn't need `flatNormals`: `flatNormals[i] = normalize3(planeNormal0)` (`facets.cpp:65`), so
  `dot3(flatNormals[i], dir)` has the same sign as Havel's own `determinant = dot3v(planeNormals.v0, dir)`. The
  batch test can reuse the determinant. Check degenerate facets (`normalize3` of a zero normal).
- Self-occlusion compares `originIntersection->node` against the `GeometryNode*`, which is unique across meshes. A
  facet index isn't: `SceneObject::findNearestIntersection` / `findAnyIntersection` (`SceneObject.cpp:131-160`) pass
  `originIntersection` to the form even when it belongs to another object. Resolve the origin to a facet index of
  this mesh once per leaf call (none if `originIntersection->node` isn't one of this mesh's `nodes`), then compare
  the lane indices against it.

## Options

### A — Per-leaf packed copies (block-SoA per leaf)

After `balancer->build()`, walk the leaves and write each leaf's facets as blocks of 4/8 lanes (component-wise inside
a block) into one mesh-owned array; the leaf references a block range. Pad the last block of a leaf with inactive
lanes (data that can't hit, or a lane mask).

- Pros: contiguous aligned loads, no gathers; one leaf visit reads a few consecutive blocks.
- Cons: duplication; memory is (sum of leaf sizes, rounded up to the block width per leaf) × (12 floats + 1 index);
  needs a leaf → block range mapping (see coupling below).
- A per-leaf form of `DONE/soa-mesh-layout.md` Path C. Like its Path D, it can sit next to the existing AoS arrays, so
  the SISD path doesn't regress.

### B — Global component-wise SoA + gather

Keep one copy in component-wise SoA (`DONE/soa-mesh-layout.md` Path A), keep index lists in the leaves, and load lanes
with `_mm256_i32gather_ps` (AVX2) or scalar inserts.

- Pros: no duplication; the leaf keeps an index list.
- Cons: 12 gathers per block of facets; AVX2 gathers are slow on many cores, which may eat the batch win. Measure.

### C — Global order chosen for locality + index lists

Order the global arrays so that most leaves are mostly contiguous (e.g. by leaf of first occurrence), and fall back
to gathers for the rest. Only worth it if B's gathers are the bottleneck and A's memory overhead isn't acceptable.

## Coupling with `DONE/flat-array-kdtree-nodes.md`

With A, the leaf has to address its packed data: a block range. The flat-array refactor already made the leaf an
offset/count range into `KDTreeRoot::leafGeometry` and passes it to `GeometryNodesTraverser` as a `GeometryNodeSpan`,
so the offset (`span.data() - leafGeometry.data()`) identifies the leaf without another change to the traversers.

Open question: how `Mesh` maps that offset to its block range. Per-leaf padding means the mesh's block offsets differ
from the tree's offsets, so the mesh needs its own offset per leaf: a table indexed by the tree offset (one entry per
`leafGeometry` element), or the leaf index passed along with the span (a signature change after all).

## Measure first

- Duplication factor on teapot and dragon: sum of leaf sizes / facet count, plus the padding overhead at block width
  4 and 8. Cheap to compute from the built `KDTreeRoot`, like `countLeafGeometry` in `Balancer_test.cpp`.
- Leaf size distribution. Since the leaf-size fix, `maxNodesSize` (16, `KDTreeBalancer.cpp:99`) is the maximum leaf
  size; only the cost check and the depth limit leave larger leaves. Measured below.
- A vs. B with `DONE/mesh-traversal-benchmark.md`.

### Measured leaf statistics (before the leaf-size fix)

`benchmarks --only=meshes` (`BruteForceSAHKDTreeBalancer`, as in `Resources::getMesh`). The build is deterministic,
so these numbers don't vary between runs:

| Mesh    | Facets | Leaves | Leaf facet refs | Duplication | Leaf size min/mean/max | Padded to 4 | Padded to 8 |
|---------|-------:|-------:|----------------:|------------:|-----------------------:|------------:|------------:|
| teapot  |   1452 |    130 |            3193 |       2.20x |         17 / 24.6 / 82 |      1.064x |      1.160x |
| dragon2 | 100400 |  10679 |          255059 |       2.54x |        17 / 23.9 / 390 |      1.070x |      1.175x |

Leaf size histogram (17-32 / 33-64 / >64 facets): teapot 117 / 12 / 1, dragon2 9640 / 987 / 52. No leaf is smaller
than 17 facets.

- `maxNodesSize` = 16 works as a floor, not a cap: the SAH split search skips every candidate with a child of
  ≤ 16 facets (`SAHKDTreeBalancer.cpp:124`), so splitting stops before leaves get that small. A leaf visit is 5-8
  blocks of 4 or 3-4 blocks of 8, so padding costs little (7% at width 4, 16-18% at width 8).
- Option A stores (sum of padded leaf sizes) × (12 floats + 1 index): teapot 3396 lanes ≈ 177 KB, dragon2
  272940 lanes ≈ 14.2 MB at width 4. The AoS `planeNormals` take 1452 × 48 B ≈ 70 KB and 100400 × 48 B ≈ 4.8 MB.

### Leaf size: 16 as a real maximum (adopted in `bed1485`)

The SAH sweep no longer skips candidates with a child of ≤ 16 facets, and `KDTreeBalancer::isTerminalPostCheck` no
longer rejects a split because a child would be a leaf; only the cost check remains. Measured on top of the traversal
fix, `BruteForceSAHKDTreeBalancer`:

| Mesh    | Leaves (empty)           | Leaf facet refs        | Leaf size min/mean/max  | Build cycles (median) |
|---------|--------------------------|------------------------|-------------------------|-----------------------|
| teapot  | 130 → 386 (0 → 4)        | 3193 → 4617 (+45%)     | 17/24.6/82 → 1/12.1/31  | 30.0M → 34.4M (+15%)  |
| dragon2 | 10679 → 30799 (0 → 1589) | 255059 → 351507 (+38%) | 17/23.9/390 → 1/12.0/84 | 3.22G → 3.56G (+11%)  |

Five alternating benchmark runs each, median of "best" core cycles per ray (facets tested per ray in brackets):

| Mesh    | Query   | Rays       | ≥ 17 per leaf  | 16 as maximum  | Change |
|---------|---------|------------|---------------:|---------------:|-------:|
| teapot  | nearest | coherent   | 1361.7 (29.06) |  702.3 (11.33) | -48.4% |
| teapot  | any     | coherent   |  857.1 (22.46) |  501.7 (9.81)  | -41.5% |
| teapot  | nearest | incoherent | 2876.6 (39.67) | 1861.3 (17.22) | -35.3% |
| teapot  | any     | incoherent | 1634.6 (25.44) | 1063.3 (10.30) | -35.0% |
| dragon2 | nearest | coherent   | 1081.7 (13.25) |  762.4 (6.38)  | -29.5% |
| dragon2 | any     | coherent   |  664.1 (10.66) |  501.9 (5.06)  | -24.4% |
| dragon2 | nearest | incoherent | 2184.2 (17.13) | 1642.0 (8.61)  | -24.8% |
| dragon2 | any     | incoherent | 1056.7 (8.63)  |  872.0 (3.90)  | -17.5% |

Hits and checksums identical in all runs, the Cornell box image byte-identical, all tests pass. For packing, smaller
leaves raise the padding share (teapot 1.105x at width 4, 1.254x at width 8; dragon2 1.116x and 1.258x), and option A
grows to teapot 5100 lanes ≈ 265 KB and dragon2 392148 lanes ≈ 20.4 MB at width 4.

Current counters per ray (starting point for this item; hits and sums as in `DONE/mesh-traversal-benchmark.md`):

| Mesh    | Query   | Rays       | Nodes | Leaves | Facets |
|---------|---------|------------|------:|-------:|-------:|
| teapot  | nearest | coherent   |  8.78 |   0.90 |  11.33 |
| teapot  | any     | coherent   |  7.99 |   0.78 |   9.81 |
| teapot  | nearest | incoherent | 11.03 |   1.40 |  17.22 |
| teapot  | any     | incoherent |  8.75 |   0.83 |  10.30 |
| dragon2 | nearest | coherent   | 10.08 |   0.51 |   6.38 |
| dragon2 | any     | coherent   |  9.24 |   0.40 |   5.06 |
| dragon2 | nearest | incoherent | 10.05 |   0.69 |   8.61 |
| dragon2 | any     | incoherent |  8.10 |   0.31 |   3.90 |

With 4-17 facets tested per ray, the batch test gains less than the leaf statistics above suggested; measure the
leaf loop's share before investing in packing.

## Prerequisites

- `DONE/mesh-traversal-benchmark.md` (done) — to choose between the options.
- Layout decision in `DONE/soa-mesh-layout.md`, decided together with this item: options A and B here are that decision
  for the hit-test data.
- Leaf payload: done with `DONE/flat-array-kdtree-nodes.md` (offset/count range passed as `GeometryNodeSpan`).

## Required by

- `DONE/simd-batch-triangle-test.md`

## Key files

- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeRoot.h` — flat tree, leaf ranges
- `sources/raytracing/include/raytracing/geometry/partitioning/GeometryNodesTraverser.h` — leaf callback signature
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp`,
  `sources/raytracing/src/raytracing/geometry/partitioning/treebalancer/SAHKDTreeBalancer.cpp` — facet duplication
- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` — leaf loops, `setupMesh`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/facets.cpp` — `computeNormals`, `computeNodesAndGraph`,
  `computeFacetIntersection`
- `sources/raytracing/src/raytracing/geometry/SceneGeometry.cpp` — second user of the generic tree

## Resolution

A variant of option A without per-leaf padding (`5c8a256`). The blocks hold `leafGeometry`'s entries in the
same order, so the tree's own duplication is the only one, and a leaf's facets are the entry range of its span
(`span.data() - leafGeometry.data()`). Blocks run across leaf boundaries; the lanes outside the leaf's range are
masked. That settles the open question above: no leaf → block table and no signature change.

- Culling uses the sign of the Havel determinant: 0 differences against `flatNormals` on teapot and dragon2 in the
  benchmark. Degenerate facets (zero `planeNormals.v0`) can't hit in either form: their distance is 0/0 = NaN.
- Self-occlusion: the origin's node is looked up in the leaf's span once per call (a sequential scan) and its lane
  is masked, so no facet index of the origin is needed.
- Memory: 52 B per leaf entry (48 B plane normals, 4 B facet index); dragon2 has 351507 entries, about 18.3 MB.
- Timing and verification: `DONE/soa-mesh-layout.md`.

## Commits

- `5c8a256` perf(raytracing): test mesh leaves in 4-wide facet blocks
