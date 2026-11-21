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

The flat-array layout, the explicit stack and a traversal fix found on the way are done, see
`DONE/flat-array-kdtree-nodes.md`. The open points below remain.

## Scope

Large structural refactor touching:

- `KDTreeNode` / `KDTreeNodeChildren` / `KDTreeRoot` — replace with `CompactKDNode` + vector
- `KDTreeBalancer::build()` / `KDTreeNode::grow()` — change from recursive allocation to index-based
- `NaiveKDTreeTraverser` / `VoxelizationKDTreeTraverser` — pointer+recursion → index+explicit stack
- Build phase needs a two-pass or resize-on-grow approach since tree size isn't known upfront

## Open points

- Index-based `build()` / `grow()`: the balancers still allocate one heap node per tree node, which is then copied.
  Only matters for build time (mesh load, balancer switch in the UI).
- Not attempted: SIMD-friendly bounding layout across several nodes, prefetching.

## Key files

- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNode.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNodeChildren.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeRoot.h`
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h`
