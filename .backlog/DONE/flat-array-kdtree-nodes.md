# Flat-array compact KD-tree with iterative traversal

## Motivation

Two related issues in the KD-tree traversal hot path:

**Cache-hostile node layout:** `KDTreeNode` stores a `std::unique_ptr<KDTreeNodeChildren>` per internal
node. Each 128-byte `KDTreeNodeChildren` is individually heap-allocated, scattering nodes across memory.
During traversal (~636k rays per frame, multiple tree levels each), every step chases a heap pointer —
likely triggering L1/L2 cache misses at ~100 cycles each.

**Recursive traversal:** `NaiveKDTreeTraverser` uses recursion when both children are hit by a ray. Each
recursive call pushes a full stack frame (~6 arguments), causing register spills and branch predictor
pressure. Tree depth is ~12 for the teapot mesh, so stack overflow isn't the risk — the cost is function
call overhead and loss of register allocation across call boundaries.

These two issues are tightly coupled: converting to flat-array nodes and an explicit traversal stack at the
same time gives the biggest win and avoids touching the same code twice.

## Scope

Large structural refactor touching:

- `KDTreeNode` / `KDTreeNodeChildren` / `KDTreeRoot` — replace with `CompactKDNode` + vector
- `KDTreeBalancer::build()` / `KDTreeNode::grow()` — change from recursive allocation to index-based
- `NaiveKDTreeTraverser` / `VoxelizationKDTreeTraverser` — pointer+recursion → index+explicit stack
- Build phase needs a two-pass or resize-on-grow approach since tree size isn't known upfront

## Flat-array node layout

1. Store all nodes in a single `std::vector<CompactKDNode>`.
2. Reference children by index instead of pointer (e.g. stored offset or implicit `2*i+1`).
3. Pack split axis (2 bits) + leaf flag (1 bit) into low bits of the child index.
4. Bounding boxes stored inline or in a parallel array for SIMD-friendly access.

Benefits: halves memory (no unique_ptr/allocator overhead), guarantees sequential cache-line access during
depth-first traversal, enables prefetching.

## Iterative traversal with explicit stack

1. Replace recursive `findNearestIntersection` with a `while` loop and a small fixed-size stack
   (`std::array<TraversalEntry, MAX_DEPTH>`) allocated on the function stack.
2. When both children are hit, push one onto the explicit stack, continue with the other.
3. Eliminates function call overhead, lets the compiler keep ray state in registers throughout traversal.

### Resolution

- **Flat layout:** `KDTreeRoot` holds the inner nodes depth-first (child A right after its parent) in two parallel
  arrays: `childBoundings` (both child boxes, `alignas(64)`, one cache line per node) and `childLinks` (2 × 32 bit).
  Leaves are offset/count ranges (`KDTreeLeaf`) into one `leafGeometry` array. A `KDTreeLink` is an inner-node index,
  or a leaf index with bit 31 set. The tree tests child boxes, not split planes, so no split axis is stored.
- **Leaf payload:** `GeometryNodesTraverser` gets a leaf as a `GeometryNodeSpan` (`std::span<GeometryNode* const>`)
  instead of a `PGeometryNodeList`: `Mesh`, `SceneGeometry`, the tests' `BoxNodesTraverser` and the benchmark wrapper.
  The span's offset into `leafGeometry` identifies the leaf, which `DONE/kdtree-leaf-packed-facets.md` needs.
- **Iterative traversal:** `NaiveKDTreeTraverser` and `VoxelizationKDTreeTraverser` use an explicit
  `std::array<..., KDTreeRoot::MaxDepth>` stack (64; the build asserts the depth fits). Visit order, pruning and the
  any-hit early exit are unchanged.
- **Build:** two passes. The balancers still grow the heap-allocated `KDTreeNode` tree; `KDTreeBalancer::build()`
  then flattens it into `KDTreeRoot`. `KDTreeNode` and `KDTreeNodeChildren` are build-only now.
- A new test instantiates `VoxelizationKDTreeTraverser`; nothing compiled it before.
- **Traversal fix (found during this work, separate from the layout):** where a ray hit neither child box, both
  traversers still descended into child B (`x(leftRightHit) ? childA : childB`) down to a leaf and tested its facets.
  The code assumed a ray inside the parent box always hits a child box, but child boxes are shrunk to their geometry
  (`KDTreeBalancer.cpp:154-155`), and boxes beyond the current hit distance count as missed. Now such a subtree is
  skipped. A unit test checks that a ray between two clusters reaches no leaf.

  How often it happened, measured with a temporary probe in the mesh benchmark (nearest hit, before the fix):

  | Mesh    | Rays       | Rays with such a miss | Misses/ray | Facets/ray | Facets/ray with the fix |
  |---------|------------|----------------------:|-----------:|-----------:|------------------------:|
  | teapot  | coherent   |                 86.8% |       3.90 |      55.18 |            29.06 (-47%) |
  | teapot  | incoherent |                 67.6% |       2.59 |      61.91 |            39.67 (-36%) |
  | dragon2 | coherent   |                 98.6% |      13.48 |      47.33 |            13.25 (-72%) |
  | dragon2 | incoherent |                 97.4% |      12.35 |      50.74 |            17.13 (-66%) |

  The descent found the nearest hit first in some rays (153-2550 of 262144 per set), always at a point outside both
  missed boxes; the regular path finds the same hits, so the results don't change.

#### Verification

- After each stage (flat layout, then explicit stack), `benchmarks --only=meshes` reproduced the baseline of
  `DONE/mesh-traversal-benchmark.md` exactly: tree statistics, nodes/leaves/facets per ray, hits and facet index sums.
- UI first frame (Cornell box), before vs. after binary: identical ray counts and
  `kdTree nodesVisited=1307346 rootOverlapMisses=857161`. The scene tree (9 objects) is a single leaf, so the
  root-is-leaf path is covered.
- With the traversal fix: hits, facet index sums and distance sums of the benchmark are still identical (nearest and
  any hit); only the node/leaf/facet counters drop (see `DONE/mesh-traversal-benchmark.md`). The UI's first frame is
  byte-identical to the original binary's image, with the same ray counts; `kdTree nodesVisited` drops from 1307346
  to 1267412.
- Full `release` build with 398 raytracing and 61 primitives tests passing; the raytracing tests also pass in
  `debug` (asserts on). Two tests send rays through both children of 64-box trees, so the deferred-child path is
  covered by unit tests too. `clang-release` wasn't built.

#### Timing

Five alternating runs each of the before and after binary in one session, median of "best" in core cycles per ray
(see `DONE/mesh-traversal-benchmark.md` for the method):

| Mesh    | Query   | Rays       | Before | After  | Change |
|---------|---------|------------|-------:|-------:|-------:|
| teapot  | nearest | coherent   | 2205.3 | 2260.1 |  +2.5% |
| teapot  | any     | coherent   | 1350.7 | 1353.7 |  +0.2% |
| teapot  | nearest | incoherent | 4083.8 | 4021.0 |  -1.5% |
| teapot  | any     | incoherent | 2483.4 | 2415.9 |  -2.7% |
| dragon2 | nearest | coherent   | 2526.2 | 2530.6 |  +0.2% |
| dragon2 | any     | coherent   | 1628.8 | 1620.1 |  -0.5% |
| dragon2 | nearest | incoherent | 4984.7 | 4738.0 |  -4.9% |
| dragon2 | any     | incoherent | 3264.8 | 3096.6 |  -5.2% |

CV per variant was 0.1-3.2%, so the standard error of a change is about 1%. The gain is where traversal misses the
caches (dragon2, incoherent rays, -5%); on teapot and coherent rays the change is within ±2.5%, and teapot
nearest/coherent got slightly slower. Per ray, traversal visits 8-21 nodes but the leaf loop tests 34-62 facets, so
the leaf loop (`DONE/simd-batch-triangle-test.md`) likely dominates (not profiled).

Three-way comparison in a later session, 5 alternating runs each, median of "best" in core cycles per ray:

| Mesh    | Query   | Rays       | Original | Flat   | Flat + fix | Fix vs. flat | Fix vs. original |
|---------|---------|------------|---------:|-------:|-----------:|-------------:|-----------------:|
| teapot  | nearest | coherent   |   2195.0 | 2229.7 |     1358.6 |       -39.1% |           -38.1% |
| teapot  | any     | coherent   |   1340.3 | 1337.4 |      849.3 |       -36.5% |           -36.6% |
| teapot  | nearest | incoherent |   3963.9 | 3981.7 |     2817.2 |       -29.2% |           -28.9% |
| teapot  | any     | incoherent |   2463.8 | 2387.1 |     1613.5 |       -32.4% |           -34.5% |
| dragon2 | nearest | coherent   |   2500.9 | 2485.3 |     1080.0 |       -56.5% |           -56.8% |
| dragon2 | any     | coherent   |   1610.6 | 1609.0 |      666.8 |       -58.6% |           -58.6% |
| dragon2 | nearest | incoherent |   5453.7 | 4976.5 |     2327.2 |       -53.2% |           -57.3% |
| dragon2 | any     | incoherent |   3468.7 | 3240.5 |     1111.5 |       -65.7% |           -68.0% |

CV of the fixed binary: 0.6-1.9%, 4.1-4.6% for dragon2 incoherent. The fix dominates; the layout alone adds about
-7% to -9% on dragon2 incoherent here and stays within ±3% elsewhere.

## Index-based `build()` / `grow()` (measured, dropped)

- Index-based `build()` / `grow()`: the balancers still allocate one heap node per tree node, which is then copied.
  Only matters for build time (mesh load, balancer switch in the UI). Measured: at most about 1% of the build,
  not worth it.

### Build time split (2026-11-21, on top of `845d7f2`)

Temporary `__rdtsc` probes in `KDTreeBalancer::build` (`BruteForceSAHKDTreeBalancer`, three runs):

| Mesh    | Pointer-tree build (SAH), TSC cycles |     Flatten | Free pointer tree | Flatten + free |
|---------|-------------------------------------:|------------:|------------------:|---------------:|
| teapot  |                              30-34 M | 0.23-0.26 M |       0.09-0.29 M |       1.0-1.6% |
| dragon2 |                          3.93-4.08 G |     20-22 M |           14-18 M |       0.8-1.0% |

The SAH split search is nearly all of the build time. An index-based `build()` / `grow()` would only remove the
children allocations and the flatten copy, at most about 1% of the build: not worth it.

## Front-to-back child order for nearest queries

### Child order and re-testing deferred children (2026-11-21)

Two changes to the nearest query of `NaiveKDTreeTraverser`, built as switches and timed against each other:

- **Re-test on pop:** keep a pointer to the deferred child's box on the stack, and skip the child when the shortened
  ray misses it.
- **Front-to-back order:** where the ray hits both children, descend into the one whose box it enters first. The
  two-box `overlaps` already computes the entry distances; a new overload returns them.

All variants kept hits, distance sums and facet index sums identical. Per ray, the order cut nearest incoherent leaf
calls from 1.40 to 1.14 (teapot) and from 0.69 to 0.41 (dragon2); the re-test alone cut them only to 1.37 and 0.67.
Five alternating runs of all four builds, median "best" in core cycles per ray. "Neither" is the previous build with the
probe, compiled before the new `overlaps` overload; the other three use it:

| Mesh    | Query   | Rays       | Neither | Re-test | Order  | Both   | Re-test | Order  | Both   |
|---------|---------|------------|--------:|--------:|-------:|-------:|--------:|-------:|-------:|
| teapot  | nearest | coherent   |   334.0 |   333.3 |  332.3 |  341.8 |   -0.2% |  -0.5% |   2.3% |
| teapot  | any     | coherent   |   295.8 |   287.8 |  289.2 |  296.8 |   -2.7% |  -2.2% |   0.3% |
| teapot  | nearest | incoherent |  1012.5 |  1012.1 |  892.6 |  933.5 |   -0.0% | -11.8% |  -7.8% |
| teapot  | any     | incoherent |   682.0 |   677.1 |  681.7 |  671.3 |   -0.7% |  -0.0% |  -1.6% |
| dragon2 | nearest | coherent   |   379.2 |   392.0 |  383.1 |  419.4 |    3.4% |   1.0% |  10.6% |
| dragon2 | any     | coherent   |   322.7 |   330.8 |  330.6 |  330.9 |    2.5% |   2.4% |   2.5% |
| dragon2 | nearest | incoherent |   913.0 |   930.5 |  709.1 |  724.6 |    1.9% | -22.3% | -20.6% |
| dragon2 | any     | incoherent |   617.9 |   620.3 |  630.8 |  615.5 |    0.4% |   2.1% |  -0.4% |

The re-test costs a box test per pop and saves little: dropped. The order is adopted,
as a select between the two children rather than a branch. Final build against the probe-free `845d7f2` build, five
alternating runs each (the any-hit queries take the same path; their 1-2% differences in the table above came from the
probe build):

| Mesh    | Query   | Rays       | Before | After | Change | After faster |
|---------|---------|------------|-------:|------:|-------:|-------------:|
| teapot  | nearest | coherent   |  335.5 | 334.1 |  -0.4% |          2/5 |
| teapot  | any     | coherent   |  292.4 | 286.0 |  -2.2% |          3/5 |
| teapot  | nearest | incoherent |  980.7 | 887.7 |  -9.5% |          5/5 |
| teapot  | any     | incoherent |  704.7 | 693.8 |  -1.5% |          3/5 |
| dragon2 | nearest | coherent   |  377.9 | 378.1 |   0.1% |          3/5 |
| dragon2 | any     | coherent   |  330.9 | 328.7 |  -0.7% |          2/5 |
| dragon2 | nearest | incoherent |  909.7 | 717.5 | -21.1% |          5/5 |
| dragon2 | any     | incoherent |  633.2 | 636.9 |   0.6% |          1/5 |

- `TraverserTest::naiveTraverserNearestEntersNearerChildFirst` checks that a ray entering child B first calls only
  B's leaves; without the order it calls 4 instead of 2.
- Cornell box, first frame, five alternating runs each: median 35.4 → 35.7 ms, inconclusive (31-48 ms per frame);
  image byte-identical in all ten runs. Expected: its rays are mostly shadow rays (about 525k any-hit against 51k
  secondary rays per frame), and its scene tree (9 objects) is a single leaf, so the image doesn't exercise the new
  order outside the teapot's tree; the unit test covers the generic path.

## Measurements (2026-11-21, on top of `845d7f2`)

### Traversal share per ray (binary traversal, before the wide nodes)

The leaf-share probe (see `DONE/soa-mesh-layout.md`) got a traversal replay: the traverser runs with a leaf callback
that returns the recorded leaf results in order, so it takes the same path without leaf work. Traversal plus leaf
replay came to 96-102% of the traced time, and every chunk ended on the expected leaf call.

| Mesh    | Query   | Rays       | Traced | Traversal      | Leaf replay   | Nodes | Cycles per node |
|---------|---------|------------|-------:|---------------:|--------------:|------:|----------------:|
| teapot  | nearest | coherent   |  343.3 |  172.8 (50.3%) | 176.5 (51.4%) |  8.78 |              20 |
| teapot  | any     | coherent   |  297.3 |  162.3 (54.6%) | 141.5 (47.6%) |  7.99 |              20 |
| teapot  | nearest | incoherent | 1022.0 |  649.7 (63.6%) | 326.1 (31.9%) | 11.03 |              59 |
| teapot  | any     | incoherent |  682.5 |  497.5 (72.9%) | 183.4 (26.9%) |  8.75 |              57 |
| dragon2 | nearest | coherent   |  378.4 |  242.7 (64.1%) | 136.7 (36.1%) | 10.08 |              24 |
| dragon2 | any     | coherent   |  332.1 |  223.8 (67.4%) | 101.5 (30.6%) |  9.24 |              24 |
| dragon2 | nearest | incoherent |  910.5 |  611.7 (67.2%) | 316.3 (34.7%) | 10.05 |              61 |
| dragon2 | any     | incoherent |  611.8 |  473.7 (77.4%) | 144.6 (23.6%) |  8.10 |              58 |

- Traversal is now the larger part of a ray (50-77%).
- Incoherent rays cost about 3 times as much per node as coherent ones, on teapot too, whose tree (771 nodes, 49 KB of
  boxes) fits in L2. So memory is not the main traversal cost; mispredicted per-node branches are the likely cause.
  Prefetching and a cache-oriented layout can't remove that.
- Nearest queries (before the child order, see "Front-to-back child order" above): `NaiveKDTreeTraverser` always
  descended into child A first, and a deferred child is popped without testing its box against the ray shortened by a
  hit in the meantime. Leaf calls on such a shortened ray that find no nearer hit, per ray: teapot 0.122 of 0.90
  (coherent) and 0.476 of 1.40 (incoherent), dragon2 0.107 of 0.51 and 0.293 of 0.69.

### Branchless selects (2026-11-21, on top of `00ebdc1`)

MSVC compiled three selects of the last changes into conditional jumps: the child order in `NaiveKDTreeTraverser`
(two jumps), the back-face culling mask in `Mesh.cpp`'s `testBlock` and the origin's lane bit in `candidateLanes`. All
three were rewritten with masks (an XOR select for the child links), which the listings confirmed branch-free. Eight
alternating runs each against `2497174`, median "best" in core cycles per ray:

| Mesh    | Query   | Rays       | Before | All branchless | Change | Faster | Leaf code only | Change | Faster |
|---------|---------|------------|-------:|---------------:|-------:|-------:|---------------:|-------:|-------:|
| teapot  | nearest | coherent   |  335.0 |          360.8 |   7.7% |    0/8 |          328.9 |  -0.1% |    4/8 |
| teapot  | any     | coherent   |  282.4 |          278.1 |  -1.5% |    5/8 |          280.1 |  -1.8% |    5/8 |
| teapot  | nearest | incoherent |  866.9 |          833.0 |  -3.9% |    8/8 |          855.0 |  -1.1% |    5/8 |
| teapot  | any     | incoherent |  669.3 |          651.8 |  -2.6% |    6/8 |          656.7 |  -2.8% |    7/8 |
| dragon2 | nearest | coherent   |  372.5 |          393.6 |   5.7% |    1/8 |          368.1 |  -0.2% |    3/8 |
| dragon2 | any     | coherent   |  320.7 |          314.2 |  -2.0% |    6/8 |          320.0 |  -0.2% |    4/8 |
| dragon2 | nearest | incoherent |  718.5 |          702.5 |  -2.2% |    7/8 |          713.3 |   0.9% |    4/8 |
| dragon2 | any     | incoherent |  638.6 |          637.2 |  -0.2% |    4/8 |          635.3 |   1.9% |    2/8 |

("Leaf code only" ran in its own session; its "Before" medians differ by up to 2% from the column shown.)

- The branchless child order makes coherent nearest rays 6-8% slower and incoherent ones 2-4% faster. With a branch,
  the CPU predicts the next node and loads its boxes before the current box test is done; with the select, the next
  node's address depends on that test, so node visits serialize. Coherent rays predict well, so they lose that
  overlap; primary rays are coherent nearest queries, so the branch stays (now one `if`/`else`, one jump, with a
  comment saying why; `3ab28db`).
- The two leaf-code branches were predictable (per ray and mostly "no origin"): branchless is neutral there, and kept
  (`3ab28db`).
- Final state against `2497174`, six alternating runs: -4.4% to 1.9%, hits and checksums identical.

## Four-wide nodes (2026-11-21)

`KDTreeBalancer::build()` collapses the flat binary tree into `KDTreeRoot::wideNodes`: a `KDTreeWideNode` holds the
children of an inner node's children (2 to 4 lanes; a leaf child takes one lane), boxes component-wise in 6 `Float4`,
links to wide nodes or to the binary tree's leaves, and a mask of the used lanes (128 bytes). `overlappedLanes` is the
two-box `overlaps` lane-wise, with the same operations and operand order, so it decides like the binary traversal,
NaN cases included. `WideKDTreeTraverser` runs on it; `Resources::getMesh` and the mesh benchmark use it, while the
scene tree, the voxelization view and the existing tests keep the binary layout.

- Any-hit takes the hit lanes in lane order, depth-first. Child boxes lie inside their parent's, so it calls exactly
  the binary traversal's leaves in the same order: leaf calls and facets per ray were identical.
- Nearest goes into the hit lane with the smallest entry distance (lane order on a tie) and defers the others sorted,
  each with its entry distance; a popped child whose entry lies beyond the hit found in the meantime is dropped with
  one compare. Leaf calls match the binary "order + re-test" variant above (teapot 0.83 / 1.09, dragon2 0.46 / 0.38 per
  ray), hits, distance sums and facet index sums are identical.
- Nodes per ray drop to 0.55-0.60 of the binary traversal's (teapot nearest coherent 8.73 → 4.78).
- Memory: 179 wide nodes for teapot (23 KB), 15816 for dragon2 (2.0 MB), next to the binary arrays. Build time: the
  collapse runs for every tree (no balancer overrides `build()`); dragon2 median 3.72 G → 3.76 G TSC cycles (+1%).
- For meshes, `kdTreeNodesVisited` and the benchmark's "KD-tree nodes" per ray now count wide nodes.
- `TraverserTest::wideTraverserMatchesNaive` compares both traversers on random box trees from two balancers
  (single-leaf root, 2-, 3- and 4-lane nodes) with random, aimed and axis-parallel rays, NaN slabs included. Swapping
  the lane order or dropping a lane makes it fail.

Built in two stages, any-hit first (gate: about 10% or more and no row slower beyond noise). Six alternating runs each
against `3ab28db`, median "best" in core cycles per ray:

| Mesh    | Query   | Rays       | Before | Wide any-hit | Change | Faster | Before | Wide both | Change | Faster |
|---------|---------|------------|-------:|-------------:|-------:|-------:|-------:|----------:|-------:|-------:|
| teapot  | nearest | coherent   |  333.5 |        330.5 |  -0.9% |    2/6 |  332.6 |     261.1 | -21.5% |    6/6 |
| teapot  | any     | coherent   |  291.8 |        249.5 | -14.5% |    6/6 |  293.8 |     254.2 | -13.4% |    6/6 |
| teapot  | nearest | incoherent |  889.5 |        894.8 |   0.6% |    2/6 |  896.5 |     734.9 | -18.0% |    6/6 |
| teapot  | any     | incoherent |  650.4 |        537.3 | -17.4% |    6/6 |  661.7 |     558.4 | -15.6% |    6/6 |
| dragon2 | nearest | coherent   |  368.5 |        370.7 |   0.6% |    2/6 |  369.9 |     289.9 | -21.6% |    6/6 |
| dragon2 | any     | coherent   |  324.2 |        257.6 | -20.6% |    6/6 |  323.1 |     262.2 | -18.8% |    6/6 |
| dragon2 | nearest | incoherent |  716.6 |        714.1 |  -0.4% |    3/6 |  714.6 |     580.2 | -18.8% |    6/6 |
| dragon2 | any     | incoherent |  630.1 |        500.6 | -20.6% |    6/6 |  631.8 |     508.9 | -19.5% |    6/6 |

(The two stages ran in separate sessions, each against its own "Before".)

Cornell box, first frame, 15 alternating pairs against the `2497174` build: median 42.8 → 39.3 ms, faster in 10 of 15
pairs, single frames 29-55 ms, so unchanged within noise (the meshes are a small part of that frame); image
byte-identical in all runs.

### Resolution

Implemented as described; the helper types of `KDTreeRoot.h` (`KDTreeLink`, `KDTreeChildBoundings`,
`KDTreeChildLinks`, `KDTreeLeaf`) moved into their own headers first, and the wide types got their own files
(`KDTreeWideNode`, `KDTreeWideRay`).

## Eight-wide nodes (2026-11-21)

`KDTreeWideNode` takes its width from one alias now: `KDTreeWideLanes` (`Float8`) sets `Width` (8) and `Levels` (3).
A node holds the descendants of an inner node three levels down (2 to 8 lanes; a leaf above that level takes one lane),
depth-first with child A first, boxes component-wise in 6 `Float8`, 8 links and the lanes mask (256 bytes). The
collapse moved from `KDTreeBalancer.cpp` into `KDTreeRoot::buildWideNodes()` and recurses over the levels.
`overlappedLanes` runs on `Float8`, with a new `outOfReach(RayCast, Float8)` overload in primitives that has the
`Float4` one's operand order (`RayCastTest::detectsOutOfReachPerLane` compares both, NaN included).

- Leaf calls, facets, hits, facet index sums and distance sums were identical to the four-wide build in all runs.
- Wide nodes per ray (the root's box test counts as one), four-wide → eight-wide, nearest / any: teapot coherent
  4.78 → 3.75 / 4.74 → 3.72, incoherent 5.60 → 4.25 / 5.10 → 3.92; dragon2 coherent 5.42 → 4.11 / 5.32 → 4.05,
  incoherent 4.97 → 3.74 / 4.87 → 3.67.
- Memory: teapot 122 wide nodes (31 KB, was 179 and 23 KB), dragon2 10694 (2.7 MB, was 15816 and 2.0 MB). Build time
  unchanged: dragon2 median 4.08 G → 4.03 G TSC cycles, six runs each.
- Stack: each wide node on the path defers up to `Width - 1` children (any-hit pushes up to `Width`, then pops one),
  and a path of `MaxDepth` nodes has at most `MaxDepth - 1` inner nodes, `Levels` per wide node. That gives
  `StackSize = 7 * 21 + 1 = 148`; the previous `2 * MaxDepth = 128` would not have held eight lanes.
  `TraverserTest::wideTraverserStackHoldsMaxDepthTree` builds a tree of `MaxDepth` levels in which every wide node has
  all lanes hit and defers all but its first: both queries fill the stack to the bound, and one entry less fires the
  traverser's assert in Debug.
- `TraverserTest::wideTraverserMatchesNaive` now requires wide nodes of every lane count from 2 to 8. Swapping child A
  and B in the collapse or dropping a node's last lane makes it fail.

Six alternating runs against the four-wide build (`71accae`), median "best" in core cycles per ray:

| Mesh    | Query   | Rays       | Before | Eight-wide | Change | Faster |
|---------|---------|------------|-------:|-----------:|-------:|-------:|
| teapot  | nearest | coherent   |  245.4 |      240.2 |  -2.1% |    6/6 |
| teapot  | any     | coherent   |  229.4 |      208.4 |  -9.1% |    5/6 |
| teapot  | nearest | incoherent |  646.6 |      588.1 |  -9.1% |    5/6 |
| teapot  | any     | incoherent |  513.5 |      421.0 | -18.0% |    6/6 |
| dragon2 | nearest | coherent   |  276.6 |      248.3 | -10.2% |    6/6 |
| dragon2 | any     | coherent   |  244.1 |      210.6 | -13.7% |    6/6 |
| dragon2 | nearest | incoherent |  571.5 |      505.1 | -11.6% |    5/6 |
| dragon2 | any     | incoherent |  502.4 |      408.9 | -18.6% |    6/6 |

## Wide nodes for the scene tree (2026-11-21)

`raytracerui`'s `main.cpp` builds the scene with `WideKDTreeTraverser<SceneIntersection>`, and `raytracing.h` exports
the wide traverser. The Cornell box's scene tree is a single leaf, so the comparison ran on `ProceduralScene<1, 10>`
(8000 objects, 27 lights; scene tree depth 12, 1827 nodes), selected in `main.cpp` for the measurement only. First
frame, six alternating renders each:

- Naive scene traverser: median 529.7 ms; wide: 507.4 ms (-4.2%), faster in 5 of 6 pairs. Ray counters identical and
  the image byte-identical in all 12 renders.
- Cornell box against the `71accae` build, ten alternating pairs: median 51.1 → 55.0 ms, faster in 4 of 10, single
  frames 35-66 ms, so unchanged within noise; ray counters identical and the image byte-identical in all 20 runs.

## Branch-light node step (2026-11-21, measured and dropped)

Two variants of the eight-wide node step against the per-lane `if`s:

- **Bit scan:** visit only the hit lanes, with `countr_zero` (nearest) or `bit_width` (any-hit).
- **Branchless:** any-hit stores every lane's link and advances the stack by the lane's hit bit. Nearest ranks each
  hit lane by entry distance (SIMD compares, popcount, lane order on a tie) and stores it straight into its stack slot;
  rank 0 is the next node, and missed lanes go to a spare slot.

Six alternating runs of all three builds, median "best" in core cycles per ray:

| Mesh    | Query   | Rays       | Per lane | Bit scan | Change | Faster | Branchless | Change | Faster |
|---------|---------|------------|---------:|---------:|-------:|-------:|-----------:|-------:|-------:|
| teapot  | nearest | coherent   |    236.2 |    299.4 |  26.7% |    0/6 |      374.4 |  58.5% |    0/6 |
| teapot  | any     | coherent   |    208.8 |    272.9 |  30.7% |    0/6 |      291.8 |  39.8% |    0/6 |
| teapot  | nearest | incoherent |    593.5 |    545.0 |  -8.2% |    6/6 |      589.5 |  -0.7% |    6/6 |
| teapot  | any     | incoherent |    427.3 |    423.1 |  -1.0% |    3/6 |      401.6 |  -6.0% |    6/6 |
| dragon2 | nearest | coherent   |    253.5 |    295.0 |  16.4% |    0/6 |      372.4 |  46.9% |    0/6 |
| dragon2 | any     | coherent   |    211.5 |    264.4 |  25.0% |    0/6 |      286.8 |  35.6% |    0/6 |
| dragon2 | nearest | incoherent |    535.0 |    500.1 |  -6.5% |    5/6 |      543.3 |   1.6% |    2/6 |
| dragon2 | any     | incoherent |    420.6 |    441.2 |   4.9% |    0/6 |      450.4 |   7.1% |    2/6 |

Both lose 16-59% on coherent rays and win at most 8% on incoherent ones. With the per-lane branches, the CPU predicts
which lanes hit and loads the next node before the box test is done. Without them, the next node's address waits for
the test, the same serialization as the branchless child order under "Branchless selects" above. The per-lane
branches stay; the hit lanes' leaf calls and facets were identical in all variants.

## Prefetching (2026-11-21, measured and dropped)

`_mm_prefetch` of the four cache lines of each hit lane's wide node, issued where the lane is pushed or sorted. Two
variants, each in its own session against the eight-wide build: inner lanes only (an `isLeaf` branch per hit lane),
and branch-free (a leaf link prefetches some wide node). Six alternating runs each, median "best" in core cycles per
ray:

| Mesh    | Query   | Rays       | Before | Inner only | Change | Faster | Before | Branch-free | Change | Faster |
|---------|---------|------------|-------:|-----------:|-------:|-------:|-------:|------------:|-------:|-------:|
| teapot  | nearest | coherent   |  238.6 |      255.8 |   7.2% |    0/6 |  236.4 |       266.9 |  12.9% |    0/6 |
| teapot  | any     | coherent   |  205.2 |      234.1 |  14.1% |    0/6 |  209.4 |       241.7 |  15.4% |    0/6 |
| teapot  | nearest | incoherent |  574.6 |      639.4 |  11.3% |    1/6 |  593.0 |       622.1 |   4.9% |    0/6 |
| teapot  | any     | incoherent |  419.5 |      469.1 |  11.8% |    0/6 |  423.2 |       462.2 |   9.2% |    0/6 |
| dragon2 | nearest | coherent   |  252.6 |      288.1 |  14.0% |    0/6 |  249.4 |       279.0 |  11.9% |    0/6 |
| dragon2 | any     | coherent   |  210.6 |      245.9 |  16.8% |    0/6 |  209.4 |       249.6 |  19.1% |    0/6 |
| dragon2 | nearest | incoherent |  501.9 |      522.8 |   4.2% |    2/6 |  508.9 |       516.9 |   1.6% |    2/6 |
| dragon2 | any     | incoherent |  397.0 |      436.4 |  10.0% |    1/6 |  399.9 |       425.6 |   6.4% |    0/6 |

Every row is slower. Dragon2 incoherent, whose rays spread over 2.7 MB of wide nodes, loses least. The prefetches
cost more than the misses they could hide, which fits the measurements above: the traversal is bound by per-node
branches, not by memory.

## Key files

- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNode.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNodeChildren.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeRoot.h`
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeWideLanes.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeWideNode.h`
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeRoot.cpp` (`buildWideNodes`)
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/WideKDTreeTraverser.h`

## Commits

- `e65a989` refactor(raytracing): store the KD-tree in flat arrays
- `cf08b14` fix(raytracing): skip KD-tree subtrees whose child boxes the ray misses
- `2497174` perf(raytracing): traverse the nearer KD-tree child first for nearest hits
- `d37b52b` refactor(raytracing): move the KD-tree helper types into their own files
- `2c388c1` perf(raytracing): traverse mesh KD-trees through four-wide nodes
- `f8f19f1` feat(primitives): add a Float8 overload of outOfReach
- `fb5449b` perf(raytracing): widen the KD-tree's wide nodes to eight lanes
- `b310a89` perf(raytracerui): traverse the scene tree through wide nodes
