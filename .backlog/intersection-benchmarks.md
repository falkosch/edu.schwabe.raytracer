# Separate executable for intersection test benchmarks

## Motivation

The intersection benchmarks previously lived in `raytracerui` as `Benchmarks.cpp` (removed alongside
`ManualTests.cpp`). They measured per-intersection cycle costs for the core ray-geometry primitives but
were tightly coupled to the UI executable — pinning process affinity, using `__rdtsc` directly, and
invoked via a commented-out call in `main.cpp`. A dedicated benchmark executable is cleaner: it can run
in CI, doesn't depend on the Win32 UI stack, and can be extended independently.

## Scope

Create a new CMake executable target (e.g. `benchmarks`) that benchmarks ray-primitive intersection
throughput. Benchmarks to port from the former `Benchmarks.cpp`:

- **Ray-AABB overlaps** — `overlaps(RayCast, AABB, AABB)` (models KD-tree traversal cost)
- **Ray-AABB nearest intersection** — `nearestIntersection(RayCast, AABB, mask)`
- **Ray-BoundingSphere nearest intersection** — `nearestIntersection(RayCast, BoundingSphere, mask)`
- **Ray-SplittingPlane nearest intersection** — `nearestIntersection(RayCast, SplittingPlane, mask)`
- **Ray-Triangle (Mesh Facet) nearest intersection** — `Mesh::findNearestIntersection`

Each benchmark ran 4 rays x 100M iterations and reported cycles per intersection via `__rdtsc`.

## Design considerations

- Link against `raytracing` (and transitively `primitives`, `vectorization`). No dependency on
  `raytracerui` or Win32 UI.
- Use a lightweight benchmark harness. Options: raw `__rdtsc` loop (as before), Google Benchmark, or
  Catch2 benchmarking (Catch2 is already a vcpkg dependency).
- Pin thread affinity for stable measurements (the old code pinned process affinity to a single core).
- Report cycles/intersection and wall-clock time. Optionally detect CPU clock frequency.
- The old `Benchmarks.cpp` source is available in git history at commit before the removal for reference.

## Files

```
sources/benchmarks/
  CMakeLists.txt
  src/main.cpp
  src/intersection_benchmarks.cpp
  include/intersection_benchmarks.h
```

Register in the top-level `CMakeLists.txt` via `add_subdirectory(sources/benchmarks)`.
