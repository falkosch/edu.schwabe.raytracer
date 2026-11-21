# Flat-array compact KD-tree with iterative traversal (done parts)

Carved out of `.backlog/flat-array-kdtree-nodes.md`. A SIMD-friendly bounding layout, a branch-light node step and
prefetching stay open there.

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
the leaf loop (`simd-batch-triangle-test.md`) likely dominates (not profiled).

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

## Commits

- `e65a989` refactor(raytracing): store the KD-tree in flat arrays
- `cf08b14` fix(raytracing): skip KD-tree subtrees whose child boxes the ray misses
- `2497174` perf(raytracing): traverse the nearer KD-tree child first for nearest hits
