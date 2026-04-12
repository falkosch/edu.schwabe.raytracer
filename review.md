## Correctness Issues

### 3. `benchmarkMachineEpsilon` may infinite-loop under `/fp:fast`

**File:** `src/vectorization/constants/values/Epsilon.cpp`

`while (f + One<T>() != One<T>())` — under `/fp:fast` the compiler may reason algebraically that
`f + 1.0 != 1.0` is always true for positive `f`, optimizing to an infinite loop. Currently works
in practice with MSVC but is fragile. Needs a `volatile`-based fix that actually survives
`/fp:fast + /O2` (previous attempts with `volatile` still produced NaN in release).

### 17/22. `select` semantics: sign-bit vs. non-zero

**Files:** `src/vectorization/selects/selects_128s.cpp`, `selects_128d.cpp`

Since commit `c687b58`, the 128s/128d `select` overloads use `_mm_blendv_ps`/`_mm_blendv_pd`
(sign-bit selection). The tests expect "non-zero = true" semantics and pass masks like `42`
(sign bit clear), causing 6 test failures. The raytracer works correctly with sign-bit semantics.
Either update the tests to use proper all-bits-set comparison masks, or document that `select`
requires sign-bit masks (as produced by `_mm_cmpgt_ps` etc.).

## Numerical Stability Issues

### 18. `min`/`max` NaN asymmetry

**Files:** `src/vectorization/functions/min.cpp:43-57`, `max.cpp:43-57`

SSE `MINSS`/`MAXSS` have asymmetric NaN handling: `min(NaN, 5.0f)` returns `5.0f` but
`min(5.0f, NaN)` returns `NaN`. This propagates inconsistently through `clamp` as well.

## Design/API Issues

### 21. `operator!` uses integer zero comparison -- `-0.0` is "truthy"

**File:** `src/vectorization/v_f32_4/operators.cpp:19`

`_mm_cmpeq_epi32` against integer zero means `-0.0` (bit pattern `0x80000000`) is treated as non-zero,
inconsistent with IEEE-754 where `-0.0 == +0.0`.

### 24. `C_PI` macro pollutes global namespace

**File:** `include/vectorization/constants/values/Pi.h:5`

Should be a `constexpr` variable in a namespace.

### 25. 21 named swizzle functions duplicated across two layers

`swizzle_128s.cpp` and `v_f32_4/swizzles.cpp` implement the same swizzles independently.
