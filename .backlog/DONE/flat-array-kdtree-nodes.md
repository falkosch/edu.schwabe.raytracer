# Flat-array compact KD-tree with iterative traversal (done parts)

Carved out of `.backlog/flat-array-kdtree-nodes.md`. Index-based tree building, a SIMD-friendly bounding layout
and prefetching stay open there.

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
  The span's offset into `leafGeometry` identifies the leaf, which `kdtree-leaf-packed-facets.md` needs.
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

## Commits

- `e65a989` refactor(raytracing): store the KD-tree in flat arrays
- `cf08b14` fix(raytracing): skip KD-tree subtrees whose child boxes the ray misses
