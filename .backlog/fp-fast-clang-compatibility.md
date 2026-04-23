# Make critical math paths robust under /fp:fast (clang -ffast-math)

## Motivation

Clang's `/fp:fast` maps to `-ffast-math`, which is far more aggressive than MSVC's `/fp:fast`. MSVC only
enables operation reordering and FMA contraction, while clang additionally enables:

- `-ffinite-math-only` — assumes Inf/NaN never occur, may optimize away checks
- `-fno-signed-zeros` — treats -0.0 and +0.0 as identical
- `-fassociative-math` — allows reassociating float operations
- `-freciprocal-math` — may substitute approximate reciprocals

This breaks rendering because the raytracer relies on IEEE-754 Inf/NaN semantics and signed-zero
bit patterns in several critical code paths. The goal is to make these paths robust so `/fp:fast`
can be used globally while the sensitive functions remain correct.

## Approach

Use `#pragma clang fp` or `__attribute__((optnone))` / `__attribute__((optimize("no-fast-math")))`
on individual functions to disable dangerous optimizations only where needed. Alternatively, compile
the affected source files with `-fno-finite-math-only -fno-signed-zeros` while keeping `-ffast-math`
project-wide.

## Affected code paths

### 1. AABB ray-slab intersection (Inf-dependent) — CRITICAL

**Files:** `primitives/src/primitives/AxisAlignedBoundingBox.cpp` (~lines 117-170)

`computeBoxIntersectionCoefficients()` multiplies `(box.min - ray.origin) * ray.reciprocalDirection`.
When a ray direction component is 0, `reciprocalDirection` is ±Inf. The slab test relies on
Inf × positive = Inf, Inf × 0 = NaN, and min/max propagating these correctly. Under
`-ffinite-math-only` the compiler may assume Inf never occurs and reorder/eliminate comparisons.

Both the single-box and dual-box overloads are affected.

### 2. Ray reciprocal direction precomputation (Inf source)

**Files:** `primitives/src/primitives/Ray.cpp` (~lines 11-18)

`reciprocal(direction)` produces ±Inf for axis-aligned rays. This is the entry point for all
Inf values consumed by the slab tests. The reciprocal itself is correct, but downstream consumers
break when the compiler assumes Inf cannot exist.

### 3. NaN checks in outOfReach (optimized away)

**Files:** `primitives/src/primitives/RayCast.cpp` (~line 86)

```cpp
return isNaN(t) | andnot(isNaN(rMaxDistanceV), t >= rMaxDistanceV);
```

`isNaN()` calls are optimized to `false` under `-ffinite-math-only`, making invalid intersections
pass through.

### 4. Havel ray-triangle sign-bit XOR (signed-zero dependent)

**Files:** `primitives/src/primitives/Facet.cpp` (~lines 38, 45, 50)

```cpp
if (const auto checkT = determinant * maxDistance - detT; isNegative(detT ^ checkT)) {
```

This XOR-of-floats trick checks whether two values share the same sign via the IEEE sign bit.
It depends on -0.0 and +0.0 being distinct at the bit level. `-fno-signed-zeros` breaks this.

### 5. Moeller ray-triangle reciprocal (Inf on degenerate triangles)

**Files:** `primitives/src/primitives/FacetEdges.cpp` (~line 19)

`reciprocal(dotv(edge1, pv))` produces Inf for degenerate (zero-area) triangles. The subsequent
`u`, `v`, `d` comparisons against 0 and 1 naturally reject these when Inf semantics hold.

### 6. AABB overlap boolean checks

**Files:** `primitives/src/primitives/AxisAlignedBoundingBox.cpp` (~lines 180-211)

`overlaps()` and `nearestIntersection()` call `computeBoxIntersectionCoefficients()` and
`outOfReach()`, combining both Inf-dependent slab tests and NaN checks.

## Suggested fix strategy

The affected functions are all in the `primitives` library. Options (from least to most invasive):

1. **Per-file flag override in CMake** — compile `AxisAlignedBoundingBox.cpp`, `Facet.cpp`,
   `FacetEdges.cpp`, and `RayCast.cpp` with `-fno-finite-math-only -fno-signed-zeros` while
   keeping `-ffast-math` for everything else. This is the smallest change.

2. **Per-function pragma** — `#pragma clang fp exceptions(strict)` or compiler-specific attributes
   on the ~5 affected functions. More granular but less portable.

3. **Algorithmic fix** — replace Inf-dependent slab tests with branch-free formulations that avoid
   division-by-zero (e.g., the Tavian Barnes robust slab test), replace XOR sign checks with
   explicit comparisons, and replace `isNaN()` guards with finite-value sentinel checks. Most
   portable and cleanest long-term, but largest code change.
