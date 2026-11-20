# Implement planned vector types declared in conversions/complex.h (done parts)

Carved out of `.backlog/implement-planned-vector-types.md`. The 8/16-bit types and the integer matrices remain open
there.

## Implemented types

### 256-bit integer vectors (32/64-bit lanes)

- `v_ui32_8` — 8x uint32, 256-bit
- `v_i32_8` — 8x int32, 256-bit
- `v_ui64_4` — 4x uint64, 256-bit
- `v_i64_2` — 2x int64, 128-bit
- `v_i64_4` — 4x int64, 256-bit

### Double-precision float vectors

- `v_f64_2` — 2x float64, 128-bit
- `v_f64_4` — 4x float64, 256-bit

### 256-bit float vector

- `v_f32_8` — 8x float32, 256-bit

## Completed prerequisites

- **256i basic-tier infrastructure**: `component_256i`, `replace_component_256i`, `blend_256i`, `swizzle_256i`,
  `swizzled_blend_256i`, `swizzled_blend_masked_256i` — all with AVX2-native + AVX-fallback paths
- **Basic-tier `PackedFloat8_256` + `PackedFloat4_256` functions**: All 18 previously-declared-but-unimplemented
  overloads (abs, ceil, clamp, copysign, divide, floor, fract, isNaN, max, min, mix, modulo, reciprocal, round,
  rsqrt, sign, sqr, sqrt) now have implementations
- **`v_i32_8`**: Full 8×int32 wrapper type with three-tier SSE/AVX/AVX2 support and tests (prerequisite for
  `v_f32_8`'s `VectorBoolType`)
- **`v_ui32_8`**: Full 8×uint32 wrapper type with three-tier SSE/AVX/AVX2 support and tests. Unsigned min/max
  (`_mm256_min_epu32`/`_mm256_max_epu32`), logical right shift (`_mm256_srlv_epi32`), no sign-dependent functions.
- **3rd-party transcendentals** for `v_f32_8`: `3rdparty/avx_mathfun.h` (`sin256_ps`/`cos256_ps`/`sincos256_ps`/
  `log256_ps`/`exp256_ps`) enabled in `v_f32_8`'s `functions_trigonometry.cpp`.

## Type-specific notes (float types, as implemented)

### Float types (`v_f32_8`, `v_f64_2`, `v_f64_4`)

- `functions_trigonometry.{h,cpp}` wraps `sin`/`cos`/`sincos`/`sin3`/`cos3`/`log`/`log3`/`exp`/`exp3`/`pow`/`pow3`
  mirroring the patterns recently added/extended for `v_f32_4` (commit `fb1a16a` and follow-up).
- **SIMD transcendentals available:** `v_f32_8` via `avx_mathfun.h` — genuine 8-lane speedup. Enable the
  currently-commented AVX include in `functions_trigonometry.cpp`.
- **No SIMD transcendentals:** `v_f64_2` / `v_f64_4` — `sse_mathfun` / `avx_mathfun` ship float32 only. Fall back to
  scalar per-lane (`std::sin(x0), std::sin(x1), ...`). Still implement for API symmetry, but no perf win versus the
  scalar `<cmath>` calls.
- **`special_values_test.cpp`:** the `v_f32_4` test covers NaN propagation, ±inf handling, denormals, and
  signed-zero. Mirror for each new float type, documenting any intentional deviations (sse_mathfun / avx_mathfun
  have known edge-case differences — see `functions_trigonometry_test.cpp::computesLogEdgeCases`).
- **`sincos` API shape:** returns two out-params (`void sincos(const v_f32_4 &v, v_f32_4 &outSin, v_f32_4 &outCos)`).
  Keep the same signature for new float types.
- **`*3` variants:** 3-component forms (e.g. `sin3`) compute the op on X/Y/Z and preserve W — for 256-bit float types
  (`v_f32_8`, `v_f64_4`) decide whether to extend to a 7-component variant that preserves the last lane, or keep the
  W-preserves-lane-4 semantic for 4-lane-like usage. Default: keep the `*3` function and add `*7` for
  `v_f32_8` / `*3` for `v_f64_4` matching the lane count.

### Integer types

- 256-bit variants (`v_i32_8`, `v_ui32_8`, `v_i64_4`, `v_ui64_4`) depended on the then-missing 256i helper headers
  (`component_256i.h` etc.) — see `unused-256bit-integer-infrastructure.md`.

## Scope estimate (completed tiers)

| Tier                              | Types                                        | Status       |
|-----------------------------------|----------------------------------------------|--------------|
| Reuses existing width helpers     | `v_f32_8`, `v_f64_2`, `v_f64_4`              | **ALL DONE** |
| Needs 256i helpers first          | `v_i32_8`, `v_ui32_8`, `v_i64_4`, `v_ui64_4` | **ALL DONE** |
| Needs 128i swizzle/swizzled-blend | `v_i64_2`                                    | **DONE**     |

## Implementation order (completed steps)

1. **`v_i32_8`** — **DONE.** Full three-tier SSE/AVX/AVX2 implementation with tests. Prerequisite for `v_f32_8`.
2. **`v_ui32_8`** — **DONE.** Unsigned counterpart to `v_i32_8` with tests. Three-tier SSE/AVX/AVX2, reduced
   function surface (no `isNegative`, `abs`, unary `-`; unsigned min/max/shift).
3. **`v_f32_8`** — **DONE.** Full 2-tier AVX/SSE implementation with 8-lane SIMD trig via `avx_mathfun`,
   99 tests, `Float8`/`Float8_32` aliases. Includes horizontal reductions (dot, length, normalize, distance),
   element-wise ops, FMA variants, and trigonometry. No `*3` partial-lane variants.
4. **`v_f64_2` / `v_f64_4`** — **DONE.** 2-wide and 4-wide float64 vectors with full operation surface
   (accessors, blends, swizzles, selects, conversions, functions, trigonometry via scalar fallback). API aliases
   `Float1_64`/`Float2_64` -> `v_f64_2`, `Float3_64`/`Float4_64` -> `v_f64_4`.
5. **`v_i64_2` / `v_i64_4` / `v_ui64_4`** — **DONE.** 64-bit integer vectors implemented with full operation
   surface. `v_i64_2` (2-wide signed), `v_i64_4` (4-wide signed), `v_ui64_4` (4-wide unsigned). API aliases
   `Int1_64`..`Int4_64` -> `v_i64_4`, `UInt3_64`/`UInt4_64` -> `v_ui64_4`.

## Related backlog items (completed and removed)

- **`unused-256bit-integer-infrastructure.md`** — All 256i infrastructure, `v_i32_8`, and `v_ui32_8` are implemented
  with full test suites.
- **`header-only-pch-migration.md`** — PCH added to all targets; header-only migration not needed (LTCG handles
  cross-library inlining).

## Commits

- `ab82daf` feat(vectorization): add 256-bit basic-tier functions, 256i infrastructure, and v_i32_8 wrapper type
- `139cd4b` feat(vectorization): add v_ui32_8 wrapper type, fix unsigned comparisons, add tests
- `7e9fe68` feat(vectorization): add v_f32_8 wrapper type with 8-lane SIMD trig and tests
- `61aa27f` feat(vectorization): add v_i64_2, v_i64_4, v_ui64_4, v_f64_2, v_f64_4 wrapper types
