# Architecture Review

Review of the full codebase focusing on shortcomings, performance bottlenecks in data structures,
algorithms, and hardware-level SSE/AVX usage. Items already tracked in `.backlog/` are excluded.

## 5. SIMD — Suboptimal Element Extraction

**Severity: Medium | Impact: Hit-testing inner loops**

`accessors/component_128s.cpp:13-24` extracts Y/Z/W via swizzle-then-`_mm_cvtss_f32`. Using
`_mm_extract_ps()` (SSE4.1, already required) or a `_mm_shuffle_ps` with direct `_mm_cvtss_f32` would save
one instruction per extraction. In tight per-hit loops this adds up.

## 6. SIMD — Aligned Load Without Validation

**Severity: Low | Impact: Crash on misaligned data**

`v_f32_4/type.cpp:38` uses `_mm_load_ps()` (requires 16-byte alignment) on a user-provided `const float*`
with no assertion. Should either use `_mm_loadu_ps()` or add a `static_assert`/runtime check.

## 7. KD-Tree — O(N²) SAH Evaluation

**Severity: High | Impact: Build time for large meshes**

`BruteForceSAHKDTreeBalancer` calls `std::accumulate()` over all geometry for every split candidate
(3 candidates per primitive × N primitives = O(N²) per node). For a 1452-face teapot this is tolerable,
but for larger meshes (100k+ faces) build time becomes dominant.

**Fix:** Implement sweep-based incremental SAH (sort once, sweep left-to-right accumulating costs) for
O(N log N) per node. Or default to `SamplingKDTreeBalancer` for large geometry counts.

## 8. KD-Tree — Pointer-Based Node Layout (Cache Hostile)

**Severity: High | Impact: Traversal throughput**

`KDTreeNode` stores a raw pointer to `KDTreeNodeChildren`, which holds two `AxisAlignedBoundingBox` values
and two child `KDTreeNode`s. Every traversal step chases a heap pointer — no spatial locality, no cache-line
alignment (`alignas` absent).

**Fix:** Use a flat-array compact representation. Nodes stored contiguously in a `std::vector`, children
referenced by index. Split axis + leaf flag packed into low bits of the child index. This halves memory and
eliminates pointer-chasing.

## 10. KD-Tree — Recursive Traversal (Not Stackless)

**Severity: Medium | Impact: Stack pressure, branch prediction**

`NaiveKDTreeTraverser` uses recursion for one child and iteration for the other. A fully iterative traversal
with an explicit small stack (or rope-based stackless traversal) would reduce call overhead and give the
compiler more room for register allocation.

## 11. Ray-Primitive — No SIMD Batch Triangle Test

**Severity: Medium | Impact: Leaf intersection throughput**

`Facet.cpp:31-55` tests one triangle at a time. Leaf traversal in `Mesh.cpp:66-92` is a scalar loop over
up to 4 facets. With SoA triangle data, 4 triangles could be tested simultaneously using 128-bit SIMD
(or 8 with AVX). This is the natural complement to increasing leaf size (issue #9).

## 12. Mesh Data — AoS Layout

**Severity: Medium | Impact: Prevents batch intersection**

`Mesh.h:62-80` stores `std::vector<Facet>` as Array-of-Structures (3 vertices packed per facet). This
layout prevents SIMD batch intersection. Converting to SoA (separate arrays for each vertex component)
would enable issue #11.

## 17. Test Coverage — No Tests for Raytracing/Primitives

**Severity: Medium | Impact: Regression risk**

The vectorization library has excellent test coverage (120 files, edge cases for NaN/denormals/infinity).
However, there are **zero tests** for:

- Ray-geometry intersection (Sphere, Box, Plane, Mesh, Facet)
- KD-tree building and traversal
- Shading/BRDF calculations
- Scene management, camera, image processing

Any refactoring of the raytracing or primitives layers has no safety net.
