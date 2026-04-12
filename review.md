# Code Review: Vectorization Library — SSE/AVX Performance Focus

**Branch:** `refactoring`
**Scope:** `sources/vectorization/` — SIMD intrinsic usage, data access patterns, missed optimizations

---

## P0 — Bugs (broken under `VECTORIZATION_APPROXIMATIONS`)

### 1. Stale parameter name `v` in approximation paths — will not compile

Three files reference the pre-refactoring parameter name `v` inside `#ifdef VECTORIZATION_APPROXIMATIONS` blocks, while
the parameter was renamed to `value`/`values`. These are currently dead code (the define is commented out in
`compiler_definitions.h:11`), but they are **compile-breaking** if enabled.

**`reciprocal.cpp:17`** — `_mm_set_ss(v)` should be `_mm_set_ss(value)`:

```cpp
Float_32 reciprocal(const Float_32 value) noexcept {
    ...
    const auto N = _mm_set_ss(v); // <-- v is undefined
```

**`reciprocal.cpp:42-44`** — `_mm_rcp_ps(v)` / `_mm_mul_ps(v, x0)` should use `values`:

```cpp
PackedFloat4_128 reciprocal(const PackedFloat4_128 &values) noexcept {
    ...
    const auto x0 = _mm_rcp_ps(v); // <-- v is undefined
```

**`rsqrt.cpp:21-26`** — `_mm_set_ss(v)` and `_mm_rsqrt_ss(_mm_set_ss(v))`:

```cpp
Float_32 rsqrt(const Float_32 value) noexcept {
    ...
    const auto N = _mm_set_ss(v); // <-- v is undefined
```

**`rsqrt.cpp:42-46`** — `_mm_rsqrt_ps(v)` and `_mm_mul_ps(v, ...)` should use `values`.

These will break the build the moment someone uncomments `VECTORIZATION_APPROXIMATIONS`. Fix: rename `v` to `value`/
`values` in all `#ifdef` branches.

---

## P1 — Performance Issues

### 2. `horizontalSum` uses `_mm_hadd_ps` — slow on all x86 microarchitectures

`v_f32_4/functions.cpp:71-72`:

```cpp
const v_f32_4::PackedType t = _mm_hadd_ps(v.components, v.components);
return _mm_hadd_ps(t, t);
```

`_mm_hadd_ps` decodes to 2-3 uops and has 3-6 cycle latency on Intel (it internally shuffles, then adds). Two
back-to-back `hadd` calls create a serial dependency chain. The standard fast alternative:

```cpp
// shuffle+add approach: 2 shuffles + 2 adds, all 1-cycle latency
auto t = _mm_add_ps(v, _mm_movehl_ps(v, v));        // [x+z, y+w, -, -]
return _mm_add_ss(t, _mm_movehdup_ps(t));             // [x+z+y+w, -, -, -]
// broadcast if needed: _mm_shuffle_ps(result, result, 0)
```

This is the same pattern already used by `minv`/`maxv` (shuffle + reduce) — inconsistent that `horizontalSum` uses the
slower `hadd` path instead.

Same concern in `m_f32_4x4/operators.cpp:38-41` for matrix-vector multiply — three nested `_mm_hadd_ps` calls. The
commented-out AVX path at line 24-36 would be better, but even the SSE path could use dot-product instructions (
`_mm_dp_ps`) instead, which is what `dotv` already does.

### 3. `operator/` creates temporary `v_f32_4` for scalar division

`v_f32_4/operators.cpp:62-68`:

```cpp
v_f32_4 operator/(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return a / v_f32_4(b);  // constructs v_f32_4 -> calls _mm_set_ps1 -> calls divide()
}
v_f32_4 operator/(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return v_f32_4(a) / b;
}
```

Compare with `operator*` which directly broadcasts:

```cpp
v_f32_4 operator*(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return _mm_mul_ps(a.components, _mm_set_ps1(b));  // direct, no temporary
}
```

The division path goes through `v_f32_4(b)` constructor -> `_mm_set_ps1` -> `operator/(v_f32_4, v_f32_4)` -> `divide()`.
The optimizer may sort this out, but the `+`, `-`, `*` operators all do the direct intrinsic call — division should too
for consistency and to avoid relying on inlining:

```cpp
return divide(a.components, _mm_set_ps1(b));
```

### 4. `select()` for 128-bit packed floats uses compare-then-blend instead of direct `blendv`

`selects_128s.cpp:12-14`:

```cpp
PackedFloat4_128 select(const PackedInts_128 &mask, ...) noexcept {
    return blendMasked(onTrue, onFalse, _mm_cmpeq_epi32(mask, Zero<PackedInts_128>()));
}
```

This compares the mask against zero to invert it, then calls `blendMasked` (which calls `_mm_blendv_ps`). The `onTrue`/
`onFalse` are also swapped in the call (comment says "reverse onTrue & onFalse since we test for mask components to be
zero"). This works correctly but is confusing and costs an extra `_mm_cmpeq_epi32`.

A clearer and faster approach — pass the mask directly and swap the arguments:

```cpp
return _mm_blendv_ps(onFalse, onTrue, _mm_castsi128_ps(mask));
```

This eliminates the zero-compare instruction entirely.

### 5. `scale()` and `translate()` transpose twice

`m_f32_4x4/functions.cpp:58-63`:

```cpp
m_f32_4x4 scale(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &scale) noexcept {
    const auto transposed = transpose(matrix);
    return transpose(m_f32_4x4(
        row<VectorIndices::X>(transposed) * xxxx(scale), ...
    ));
}
```

Two full 4x4 transposes (each is 4 shuffles + 4 unpacks = 8 instructions). The scale operation can be done directly on
rows without transposing — multiply each row element-wise by the scale vector, since scaling columns in a row-major
matrix is equivalent to `row[i] * scale` for each row. Worth auditing whether the double-transpose can be eliminated.

---

## P2 — Minor Performance Observations

### 6. Component extraction via full 4-wide swizzle

`component_128s.cpp:13-14`:

```cpp
Float_32 component<VectorIndices::Y>(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X>(yyww(values));  // _mm_movehdup_ps + _mm_cvtss_f32
}
```

For Y extraction specifically, a single `_mm_shuffle_ps(v, v, 0x55)` + `_mm_cvtss_f32` would be equivalent. The current
approach works but depends on the compiler recognizing that `yyww` is just a `movshdup`. For Z and W, the swizzle
choices (`zwzw`, `wwww`) are reasonable.

### 7. `refractEta` branches on SIMD data with scalar `if`

`v_f32_4/functions.cpp:219`:

```cpp
if (isNegative(cosSqrPhiT)) {
    return Zero<v_f32_4>();
}
```

`isNegative` uses `_mm_movemask_ps` which extracts sign bits of all 4 lanes. For total internal reflection this is
correct (all lanes have the same value from the broadcast computation), but it's a scalar branch on SIMD data — the
compiler cannot auto-vectorize across this. In a packet raytracer where different rays may or may not experience TIR,
this forces scalar fallback. This is an architectural limitation, not a simple fix.

### 8. `argmin3`/`argmax3` use `typedef` instead of `using`

`v_f32_4/functions.cpp:371-372`:

```cpp
typedef v_f32_4::VectorBoolType VBT;
typedef v_f32_4::BoolType BT;
```

Minor inconsistency with the modernization effort — these should be `using`.

---

## Summary

| # | Severity    | Issue                                                                 | Location                                     |
|---|-------------|-----------------------------------------------------------------------|----------------------------------------------|
| 1 | **P0 Bug**  | `v` vs `value`/`values` — broken `VECTORIZATION_APPROXIMATIONS` paths | reciprocal.cpp, rsqrt.cpp                    |
| 2 | **P1 Perf** | `_mm_hadd_ps` for horizontal sum (3-6 cy) vs shuffle+add (1 cy each)  | functions.cpp:71, m_f32_4x4/operators.cpp:38 |
| 3 | **P1 Perf** | Scalar division creates unnecessary temporary, unlike `+`/`-`/`*`     | operators.cpp:62-68                          |
| 4 | **P1 Perf** | `select()` has redundant zero-compare before `blendv`                 | selects_128s.cpp:12                          |
| 5 | **P1 Perf** | `scale()`/`translate()` do two full 4x4 transposes                    | m_f32_4x4/functions.cpp:58-63                |
| 6 | P2 Minor    | Component extraction via full swizzle                                 | component_128s.cpp                           |
| 7 | P2 Note     | Scalar branch on SIMD data in refraction (architectural)              | functions.cpp:219                            |
| 8 | P2 Style    | Stale `typedef` in `argmin3`/`argmax3`                                | functions.cpp:371-372                        |
