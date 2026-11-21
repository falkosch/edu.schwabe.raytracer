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

The flat-array layout, the explicit stack and a traversal fix found on the way are done, the index-based build was
measured and dropped, nearest queries take the nearer child first, and meshes traverse four-wide nodes; see
`DONE/flat-array-kdtree-nodes.md`. The open points below remain.

## Scope

Large structural refactor touching:

- `KDTreeNode` / `KDTreeNodeChildren` / `KDTreeRoot` — replace with `CompactKDNode` + vector
- `KDTreeBalancer::build()` / `KDTreeNode::grow()` — change from recursive allocation to index-based
- `NaiveKDTreeTraverser` / `VoxelizationKDTreeTraverser` — pointer+recursion → index+explicit stack
- Build phase needs a two-pass or resize-on-grow approach since tree size isn't known upfront

## Open points

- Eight-wide nodes with AVX (`Float8`, three levels per node), and wide nodes for the scene tree (a single leaf in
  the Cornell box today). The four-wide nodes are done, see `DONE/flat-array-kdtree-nodes.md`.
- Not attempted: prefetching. The measurements below point at per-node branches rather than memory, so it is unlikely
  to help.
- Not attempted: a node step with fewer data-dependent branches (incoherent rays cost about 3 times as much per node
  as coherent ones, see below). Mind the trade-off measured below: a branchless select on the path to the next node
  makes every node visit wait for the previous box test, which coherent rays pay for.

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
- Nearest queries (before the child order, see `DONE/flat-array-kdtree-nodes.md`): `NaiveKDTreeTraverser` always
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

## Key files

- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNode.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeNodeChildren.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/KDTreeRoot.h`
- `sources/raytracing/src/raytracing/geometry/partitioning/KDTreeBalancer.cpp`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h`
