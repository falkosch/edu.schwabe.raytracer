# Sweep-based incremental SAH evaluation for KD-tree building

## Motivation

`SAHKDTreeBalancer::testSplit()` evaluates each split candidate independently: two `std::copy_if` passes
to partition geometry, two `findMinimumBoundingOfGeometry` calls, and two `std::accumulate` calls in
`calculateSplitCost()` — ~6 linear passes per candidate. `BruteForceSAHKDTreeBalancer` tests 3N candidates
(min/center/max per geometry node), making the total cost O(N²) per tree node. Even
`FixedIterationsSAHKDTreeBalancer(K)` pays O(K·N) with high constant factors due to vector allocations and
redundant passes per candidate.

For the 1452-face teapot this is tolerable, but for meshes with 100k+ faces build time becomes dominant.

## Proposed fix

Implement sweep-based incremental SAH (Wald & Havran, 2006):

1. **Sort geometry once** by split axis coordinate — O(N log N).
2. **Sweep left-to-right**, maintaining running left/right bounding boxes and geometry counts.
3. **Evaluate SAH cost** at each split position in O(1) using the incremental state.
4. Total: **O(N log N)** per node instead of O(N²).

## Scope

- Refactor `SAHKDTreeBalancer::testSplit()` or introduce a new sweep-based evaluation path.
- Affects both `BruteForceSAHKDTreeBalancer` and `FixedIterationsSAHKDTreeBalancer`.
- `SamplingKDTreeBalancer` already provides `geometrySortByMinimum`/`geometrySortByMaximum` utilities.
- `calculateSplitCost()` in `KDTreeBalancer` can remain as a reference/validation implementation.

## Key files

- `sources/raytracing/src/raytracing/geometry/partitioning/treebalancer/SAHKDTreeBalancer.cpp`
- `sources/raytracing/src/raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.cpp`
- `sources/raytracing/src/raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.cpp`
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp` (calculateSplitCost)
