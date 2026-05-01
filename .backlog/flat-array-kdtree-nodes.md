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

## Proposed fix

### Flat-array node layout

1. Store all nodes in a single `std::vector<CompactKDNode>`.
2. Reference children by index instead of pointer (e.g. stored offset or implicit `2*i+1`).
3. Pack split axis (2 bits) + leaf flag (1 bit) into low bits of the child index.
4. Bounding boxes stored inline or in a parallel array for SIMD-friendly access.

Benefits: halves memory (no unique_ptr/allocator overhead), guarantees sequential cache-line access during
depth-first traversal, enables prefetching.

### Iterative traversal with explicit stack

1. Replace recursive `findNearestIntersection` with a `while` loop and a small fixed-size stack
   (`std::array<TraversalEntry, MAX_DEPTH>`) allocated on the function stack.
2. When both children are hit, push one onto the explicit stack, continue with the other.
3. Eliminates function call overhead, lets the compiler keep ray state in registers throughout traversal.

## Scope

Large structural refactor touching:

- `KDTreeNode` / `KDTreeNodeChildren` / `KDTreeRoot` — replace with `CompactKDNode` + vector
- `KDTreeBalancer::build()` / `KDTreeNode::grow()` — change from recursive allocation to index-based
- `NaiveKDTreeTraverser` / `VoxelizationKDTreeTraverser` — pointer+recursion → index+explicit stack
- Build phase needs a two-pass or resize-on-grow approach since tree size isn't known upfront

## Key files

- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNode.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNodeChildren.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeRoot.h`
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h`
