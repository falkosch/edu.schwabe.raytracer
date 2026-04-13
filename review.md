# Architecture Review

Review of the full codebase focusing on shortcomings, performance bottlenecks in data structures,
algorithms, and hardware-level SSE/AVX usage. Items already tracked in `.backlog/` are excluded.

## 1. Build System — LTO/IPO Disabled for All Libraries

**Severity: Critical | Impact: Entire codebase**

`sources-build-aggregate/CMakeLists.txt:19` explicitly sets `INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE`.
The three static libraries (vectorization, primitives, raytracing) inherit this default — none override it.
Only `raytracerui` (executable) and the test DLL set `INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE`.

On MSVC this means the static libraries are compiled **without `/GL`**, so the linker cannot inline across
translation units even though the executable requests `/LTCG`. Since all SIMD operators live in `.cpp` files
(e.g. `v_f32_4/operators.cpp`), every `+`, `-`, `*`, `/` on a `v_f32_4` is a real function call in the
generated binary. This is likely the single largest performance issue in the project.

Additionally, GCC builds are missing `-flto` and `-ffast-math` in the root `CMakeLists.txt:42`.

**Recommendation:** Set `CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE` in the root `CMakeLists.txt`
(before `add_subdirectory`) and remove the explicit `FALSE` from `sources-build-aggregate`. See section 11
for details.

## 2. SIMD — Missing `__forceinline` on Wrappers

**Severity: High | Impact: All SIMD operations**

`compiler_definitions.h` defines `ARCH_NOINLINE` but has no corresponding `ARCH_FORCEINLINE` macro. No SIMD
wrapper function uses `__forceinline` (MSVC) or `__attribute__((always_inline))` (GCC). Without LTO (issue
#1), this means the compiler cannot inline these 1-3 instruction wrappers across TUs. Even with LTO, an
explicit force-inline is insurance against the optimizer deciding not to inline.

## 3. SIMD — Inefficient Matrix-Vector Multiply

**Severity: High | Impact: Every ray transformation**

`m_f32_4x4/operators.cpp:32-35` uses `_mm_hadd_ps()` for 4×4 matrix × vector dot products. `hadd` has 3-6
cycle latency on Skylake (vs. 1 cycle for vertical ops). A shuffle-based reduction tree using
`_mm_shuffle_ps` + vertical `_mm_add_ps` would be 2-3× faster for each dot product.

The commented-out `_mm_dp_ps` version (line 25-29) is also suboptimal (5-cycle latency + port pressure).
The best approach is broadcast-multiply-add: broadcast each vector component, multiply by the corresponding
matrix row, and vertically add the four results.

## 4. SIMD — Scalar Broadcast Overhead

**Severity: Medium | Impact: Every scalar-vector operation**

8 operator overloads in `v_f32_4/operators.cpp` call `_mm_set_ps1(b)` per invocation. When the scalar is
already in a register, `_mm_broadcastss_ps` (AVX) or `_mm_shuffle_ps(v, v, 0)` would avoid a potential
store-forward stall. Consider caching the broadcast in a local or providing `v_f32_4(float)` broadcast
constructor that the compiler can CSE.

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

## 9. KD-Tree — Overly Small Leaf Size

**Severity: Medium | Impact: Traversal depth**

`KDTreeBalancer.cpp:46` sets `maxNodesSize = 4`. Modern ray tracers use 8-32 primitives per leaf. Smaller
leaves mean a deeper tree and more traversal steps (each with a cache miss per issue #8). Increasing to
16-32 is a trivial parameter change with measurable impact.

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

## 13. Mesh Data — Unused `facetEdges` Storage

**Severity: Low | Impact: Wasted memory**

`Mesh.h:77` allocates `std::vector<FacetEdges> facetEdges` for the commented-out Moller-Trumbore algorithm.
This wastes memory proportional to triangle count.

## 14. OpenMP — Global Critical Section for Normal Accumulation

**Severity: Medium | Impact: Mesh preprocessing throughput**

`facets.cpp:62-68` uses `#pragma omp critical` (a single global lock) for vertex normal accumulation during
mesh loading. Every thread serializes on this lock for every triangle.

**Fix:** Use per-thread accumulation buffers merged after the loop, or `#pragma omp parallel for reduction`.

## 15. OpenMP — KD-Tree Build Serialization

**Severity: Low | Impact: Build parallelism**

`KDTreeBalancer.cpp:113-159` uses two named `#pragma omp critical` sections to merge left/right geometry
lists via `vector::insert()` (O(N) copy each). With enough geometry the critical sections become the
bottleneck.

**Fix:** Pre-size output vectors and use parallel partitioning, or thread-local buffers with final merge.

## 16. Memory Safety — Raw `new`/`delete` Throughout

**Severity: Medium | Impact: Leak risk on early returns/exceptions**

~40+ instances of raw `new`/`delete` with zero `std::unique_ptr`/`std::shared_ptr` usage:

- Scene construction (`ProceduralScene.h`, `TestScene*.h`) — all raw `new`
- `KDTreeBalancer.cpp:50-93` — `new KDTreeRoot` with manual `delete` on error paths
- `RaytracerUI.cpp` — raw `delete` for output buffers

No RAII for dynamically allocated resources. Any exception or early return leaks.

## 17. Test Coverage — No Tests for Raytracing/Primitives

**Severity: Medium | Impact: Regression risk**

The vectorization library has excellent test coverage (120 files, edge cases for NaN/denormals/infinity).
However, there are **zero tests** for:

- Ray-geometry intersection (Sphere, Box, Plane, Mesh, Facet)
- KD-tree building and traversal
- Shading/BRDF calculations
- Scene management, camera, image processing

Any refactoring of the raytracing or primitives layers has no safety net.

## 18. Build — PCH Not Configured in CMake

**Severity: Low | Impact: Compile time**

`stdafx.h` files exist in `raytracing/src/` and `raytracerui/src/` but are not wired up via
`target_precompile_headers()` in CMake. With 291 headers across the project, enabling PCH would improve
incremental build times.

---

## Priority Ranking

| #  | Issue                                | Severity | Effort  |
|----|--------------------------------------|----------|---------|
| 1  | Enable LTO/IPO for all targets       | Critical | Low     |
| 2  | Add `ARCH_FORCEINLINE` macro         | High     | Low     |
| 3  | Shuffle-based matrix-vector multiply | High     | Low     |
| 7  | Sweep-based incremental SAH          | High     | Medium  |
| 8  | Flat-array KD-tree nodes             | High     | High    |
| 9  | Increase leaf size to 16-32          | Medium   | Trivial |
| 11 | SIMD batch triangle intersection     | Medium   | High    |
| 12 | SoA mesh data layout                 | Medium   | High    |
| 14 | Replace omp critical with reduction  | Medium   | Low     |
| 16 | Replace raw new/delete with RAII     | Medium   | Medium  |
| 17 | Add raytracing/primitives tests      | Medium   | High    |
| 4  | Scalar broadcast optimization        | Medium   | Low     |
| 5  | Direct element extraction            | Medium   | Low     |
| 10 | Stackless KD-tree traversal          | Medium   | Medium  |
| 13 | Remove unused facetEdges             | Low      | Trivial |
| 6  | Aligned load validation              | Low      | Trivial |
| 15 | KD-tree build parallelism            | Low      | Medium  |
| 18 | Wire up PCH in CMake                 | Low      | Low     |
