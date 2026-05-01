# Introduce FMA intrinsic wrappers as generic functions

## Motivation

The library has zero explicit FMA usage despite targeting AVX2+FMA. The `a*b - c*d` pattern in `cross3`
(and similar multiply-add/subtract patterns elsewhere) benefits from FMA both for performance (single
fused op vs. separate mul+sub) and numerical precision (no intermediate rounding, reducing catastrophic
cancellation).

Currently, adding FMA would require conditional compilation branches at every call site, which is noisy
and error-prone.

## Prerequisite (done)

The basic packed arithmetic wrappers (`add`, `subtract`, `multiply`, `horizontalAdd`,
`horizontalSubtract`, `addSubtract`) plus scalar-in-packed variants (`addX`, `subtractX`, `multiplyX`)
are implemented. The FMA fallback paths use these. The `X` suffix convention is established.

---

## Stage 1 — `VECTORIZATION_FMA` flag and generic FMA wrappers

### Detection flag

Introduce `VECTORIZATION_FMA` in `architecture/compiler_definitions.h`:

```cpp
#if defined(__FMA__)
  #define VECTORIZATION_FMA 1
#elif defined(__AVX2__)
  // MSVC does not define __FMA__; /arch:AVX2 enables FMA3 implicitly.
  #define VECTORIZATION_FMA 1
#else
  #define VECTORIZATION_FMA 0
#endif
```

Trade-off: `VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2` already works in practice on MSVC
(where AVX2 implies FMA3) and on GCC/Clang with `-march=native`. The dedicated flag adds robustness for
the edge case `-mavx2 -mno-fma` and self-documents intent.

### Wrappers to introduce

| Wrapper               | Semantics              | Intrinsics                                    |
|-----------------------|------------------------|-----------------------------------------------|
| `multiplyAdd`         | `a * b + c`            | `_mm_fmadd_ps/pd`, `_mm256_fmadd_ps/pd`       |
| `multiplySub`         | `a * b - c`            | `_mm_fmsub_ps/pd`, `_mm256_fmsub_ps/pd`       |
| `multiplyAddSub`      | `a*b` alt. add/sub `c` | `_mm_fmaddsub_ps/pd`, `_mm256_fmaddsub_ps/pd` |
| `multiplySubAdd`      | `a*b` alt. sub/add `c` | `_mm_fmsubadd_ps/pd`, `_mm256_fmsubadd_ps/pd` |
| `negativeMultiplyAdd` | `-(a * b) + c`         | `_mm_fnmadd_ps/pd`, `_mm256_fnmadd_ps/pd`     |
| `negativeMultiplySub` | `-(a * b) - c`         | `_mm_fnmsub_ps/pd`, `_mm256_fnmsub_ps/pd`     |

Each fully implemented for all four packed float types (`__m128`, `__m128d`, `__m256`, `__m256d`).
The non-FMA fallback path uses the basic `add`/`subtract`/`multiply` wrappers from
`packed-arithmetic-wrappers.md`. The `multiplyAddSub` fallback is `addSubtract(multiply(a, b), c)`. The
`multiplySubAdd` fallback requires lane-inverted semantics — `addSubtract(multiply(a, b), negate(c))`
or equivalent — since `_mm_fmsubadd_ps` has opposite lane polarity to `_mm_addsub_ps`.

### Scalar-in-packed variants

For `multiplyAdd`, `multiplySub`, `negativeMultiplyAdd`, `negativeMultiplySub`: provide additional
`PackedFloat4_128`-input wrappers operating on the x-component only, with y/z/w passed through from the
first argument. These wrap `_mm_fmadd_ss`/`_mm_fmsub_ss`/`_mm_fnmadd_ss`/`_mm_fnmsub_ss` and avoid the
round-trip through `Float_32` when the operand already lives in a packed register.

```cpp
// naming suffix to be decided (e.g. multiplyAddX vs multiplyAdd_x)
PackedFloat4_128 multiplyAddX(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept;
```

No scalar-in-packed variants for `multiplyAddSub` / `multiplySubAdd` — `_ss` intrinsics do not exist for
the alternating add/sub family.

Plain `Float_32 multiplyAdd(Float_32, Float_32, Float_32)` overloads are **not** introduced — the
compiler is expected to fuse `a * b + c` under `/fp:fast` / `-mfma`.

### Files

```
include/vectorization/functions/{multiply_add,multiply_sub,multiply_add_sub,multiply_sub_add,negative_multiply_add,negative_multiply_sub}.h
src/vectorization/functions/{multiply_add,multiply_sub,multiply_add_sub,multiply_sub_add,negative_multiply_add,negative_multiply_sub}.cpp
```

Plus `#include` lines in `include/vectorization/functions.h` (alphabetical) and entries in
`sources/vectorization/CMakeLists.txt`.

### Tests

Extend `tests/vectorization.native-test/` with full coverage:

- Each variant for each overload (packed and scalar-in-packed)
- Edge cases: NaN, infinity, signed zero, large/small values exercising precision differences
  between fused and unfused paths
- Verify lane-pass-through semantics for scalar-in-packed variants

---

## Stage 2 — Type-wrapper integration and call-site adoption

After the generic wrappers exist:

### `v_f32_4` wrappers

Declare in `include/vectorization/v_f32_4/functions.h` and implement in `src/vectorization/v_f32_4/functions.cpp`:

```cpp
v_f32_4 multiplyAdd(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept;
v_f32_4 multiplySub(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept;
v_f32_4 multiplyAddSub(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept;
v_f32_4 multiplySubAdd(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept;
v_f32_4 negativeMultiplyAdd(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept;
v_f32_4 negativeMultiplySub(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept;
```

These delegate to the generic wrappers and contain no `#if VECTORIZATION_FMA` guards — the FMA
gating lives entirely in the generic functions.

### Primary call-site application: `cross3`

`v_f32_4/functions.cpp` — hot path in ray-triangle intersection:

```cpp
// before
return yzxw(a) * zxyw(b) - zxyw(a) * yzxw(b);
// after
return multiplySub(yzxw(a), zxyw(b), zxyw(a) * yzxw(b));
```

### Other call-site candidates (review and apply)

- `refractEta` in `v_f32_4/functions.cpp` — `eta * eta - etaNdotI * etaNdotI`,
  `eta * incident - (etaNdotI + sqrt(cosSqrPhiT)) * normal`
- `reflect` — `incident - (t + t)` where `t = normal * NdotI` (becomes
  `negativeMultiplyAdd(normal, twoNdotI, incident)`)
- `mix` — `x + (y - x) * factor` (becomes `multiplyAdd(y - x, factor, x)`)
- `m_f32_4x4/operators.cpp:38-41` — `vector * matrix` broadcast-multiply-add chain:
  `xxxx(v)*row0 + yyyy(v)*row1 + zzzz(v)*row2 + wwww(v)*row3` — four FMAs, the most obvious
  target in the library
- `m_f32_4x4/functions.cpp` — `rotate()` matrix construction
- Newton-Raphson refinement in `divide` / `reciprocal` — `(2 - b*x) * x` pattern
- Newton-Raphson refinement in `rsqrt` — `y * (3/2 - x*y*y/2)` pattern (different shape, still
  FMA-amenable)
- BRDF / shading code in the `raytracing` library

Apply where measured benefit (precision or perf) justifies; do not blindly fuse every `a*b+c`.

### Tests for type-wrapper layer

Cover the `v_f32_4` overloads and add regression tests for the changed call sites (e.g. `cross3`
correctness under representative inputs).

---

## Note: `matrix * vector` vs `vector * matrix` semantics

`matrix * vector` (`m_f32_4x4/operators.cpp:23-35`) uses `_mm_hadd_ps` for row-dot-product semantics.
`vector * matrix` (`m_f32_4x4/operators.cpp:38-41`) uses broadcast-multiply-add. Do not substitute one
for the other — they compute different operations. The broadcast-multiply-add chain in `vector * matrix`
is the FMA candidate; the `_mm_hadd_ps` approach in `matrix * vector` must stay.
