# Implement planned vector types declared in conversions/complex.h

## Motivation

`conversions/complex.h` contains commented-out includes for ~15 additional vector wrapper types that are not yet
implemented. These represent the intended scope of the library and would enable future use in the raytracer and
primitives layers (e.g. 8-lane float trig via `avx_mathfun`, 256-bit integer ops already half-built).

## Planned types

### 8-bit integer vectors

- `v_ui8_16` — 16x uint8, 128-bit
- `v_ui8_32` — 32x uint8, 256-bit
- `v_i8_16` — 16x int8, 128-bit
- `v_i8_32` — 32x int8, 256-bit

### 16-bit integer vectors

- `v_ui16_8` — 8x uint16, 128-bit
- `v_ui16_16` — 16x uint16, 256-bit
- `v_i16_8` — 8x int16, 128-bit
- `v_i16_16` — 16x int16, 256-bit

### 256-bit integer vectors (32/64-bit lanes)

- `v_ui32_8` — 8x uint32, 256-bit
- `v_i32_8` — 8x int32, 256-bit
- `v_ui64_4` — 4x uint64, 256-bit
- `v_i64_2` — 2x int64, 128-bit
- `v_i64_4` — 4x int64, 256-bit

### Integer matrices

- `m_ui32_4x4` — 4x4 uint32 matrix
- `m_i32_4x4` — 4x4 int32 matrix

### Double-precision float vectors

- `v_f64_2` — 2x float64, 128-bit
- `v_f64_4` — 4x float64, 256-bit

### 256-bit float vector

- `v_f32_8` — 8x float32, 256-bit

## Existing infrastructure (readiness)

Most of the low-level plumbing is already in place, which lowers the cost of adding a type:

- **`PackedTypes<T, N>` specializations** in `architecture/meta_packed_types.h` exist for **all** planned types'
  underlying native primitives (e.g. `PackedTypes<Float_32, 8>::Type = PackedFloat8_256`,
  `PackedTypes<UInt_8, 16>::Type = PackedInts_128`). A new wrapper type consumes these — no native-type work needed.
- **Cross-cutting category headers** already exist for all widths:
    - `accessors/` — `component_{128d,128i,128s,256d,256i,256s}.h`,
      `replace_component_{128d,128i,128s,256d,256i,256s}.h`
    - `blends/` — `blend_{128d,128s,256d,256i,256s}.h`,
      `blend_masked_{128d,128i,128s,256d,256i,256s,scalar}.h`
    - `swizzles/` — `swizzle_{128d,128s,256d,256i,256s}.h`
    - `swizzled_blends/` — full `swizzled_blend_*` and `swizzled_blend_masked_*` set for float widths and 256i
    - `selects/` — `selects_{128d,128i,128s,256d,256i,256s,generic,scalar}.h`
    - `constants/masks/` — `mask_{all,none,x,y,z,w,xy,yz,zw,xyz,yzw,xyzw}.h` with specializations for
      `PackedFloat{2,4}_128`, `PackedFloat{4,8}_256`
- **3rd-party transcendentals** for `v_f32_8`: `3rdparty/avx_mathfun.h` ships
  `sin256_ps`/`cos256_ps`/`sincos256_ps`/`log256_ps`/`exp256_ps`. Commented `#include` already in
  `functions_trigonometry.cpp` — just needs enabling.

### Still missing (prerequisites for specific types)

- `swizzle_128i.h`, `swizzled_blend_*_128i.h` — needed for 128i wrapper types
- `mask_{all,none,x,...}` specializations for new packed types (e.g. `PackedInts_128` for int types,
  `PackedFloat*` variants that don't match existing)
- 8-bit and 16-bit width helpers have **no** cross-cutting headers today — much bigger undertaking

### Recently completed prerequisites

- **256i basic-tier infrastructure**: `component_256i`, `replace_component_256i`, `blend_256i`, `swizzle_256i`,
  `swizzled_blend_256i`, `swizzled_blend_masked_256i` — all with AVX2-native + AVX-fallback paths
- **Basic-tier `PackedFloat8_256` + `PackedFloat4_256` functions**: All 18 previously-declared-but-unimplemented
  overloads (abs, ceil, clamp, copysign, divide, floor, fract, isNaN, max, min, mix, modulo, reciprocal, round,
  rsqrt, sign, sqr, sqrt) now have implementations
- **`v_i32_8`**: Full 8×int32 wrapper type with three-tier SSE/AVX/AVX2 support and tests (prerequisite for
  `v_f32_8`'s `VectorBoolType`)
- **`v_ui32_8`**: Full 8×uint32 wrapper type with three-tier SSE/AVX/AVX2 support and tests. Unsigned min/max
  (`_mm256_min_epu32`/`_mm256_max_epu32`), logical right shift (`_mm256_srlv_epi32`), no sign-dependent functions.

## Per-type file set (template based on current `v_f32_4`)

Using `v_f32_4` as the reference, each new type needs:

### Headers (`sources/vectorization/include/vectorization/{type}/`)

| File                       | Required for                            |
|----------------------------|-----------------------------------------|
| `type.h`                   | all                                     |
| `api.h`                    | all (umbrella include)                  |
| `accessors.h`              | all                                     |
| `blends.h`                 | all                                     |
| `constants.h`              | all                                     |
| `conversions.h`            | all                                     |
| `functions.h`              | all                                     |
| `functions_trigonometry.h` | float types only (`v_f32_8`, `v_f64_*`) |
| `operators.h`              | all                                     |
| `selects.h`                | all                                     |
| `swizzled_blends.h`        | all                                     |
| `swizzles.h`               | all                                     |

### Sources (`sources/vectorization/src/vectorization/{type}/`)

One `.cpp` per header above **except** `api.h` (header-only). Gated implementations (e.g. AVX2-only paths) need
`#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2` guards; follow the patterns already in the 256-bit
sources.

### Tests (`tests/vectorization.native-test/src/vectorization/{type}/`)

One `*_test.cpp` per header — float types additionally get a `special_values_test.cpp` (NaN / ±inf / denormal /
signed-zero behaviour) that does not apply to integer types. The current `v_f32_4` test set is 12 files and is the
reference count.

### CMakeLists.txt registration

Every new header/source/test must be listed explicitly in the corresponding `CMakeLists.txt` — there is no
glob. For matrix-type additions, remember to register both `m_*` and `v_*_4x4` (SOA) variants if both apply.

### `conversions/complex.h` updates

For each new type:

- Uncomment the corresponding `#include "vectorization/{type}.h"` line.
- Add the cross-type `convert<>` template specializations (e.g. `v_f32_4 → v_f32_8`, `v_i32_4 → v_i32_8`).

## Type-specific concerns

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

### Integer types (all `v_i*`, `v_ui*`)

- No `functions_trigonometry.{h,cpp}` — integers have no transcendentals.
- No `special_values_test.cpp` — denormal/NaN/inf don't apply; edge cases (overflow, sign extension) go into
  the per-category tests (`operators_test.cpp`, `conversions_test.cpp`).
- Shift operators (`<<`, `>>`) need per-type tests if the wrapper exposes them (see `v_i32_4/operators.cpp`).
- 256-bit variants (`v_i32_8`, `v_ui32_8`, `v_i64_4`, `v_ui64_4`) depend on the still-missing 256i helper headers
  (`component_256i.h` etc.) — see `unused-256bit-integer-infrastructure.md`.

### Matrix types (`m_ui32_4x4`, `m_i32_4x4`)

- Per the current `m_f32_4x4` pattern, matrices need 5 header/source/test file sets: `type`, `accessors`,
  `constants`, `functions`, `operators`. No `swizzles`/`swizzled_blends`/`selects`/`blends`/`conversions` on matrices.
- Integer matrices need a **reduced** `functions.h` — inverse / rotate / ortho / perspectiveFov / unproject don't
  make sense on integer matrices. Plausible surface: `transpose`, `translate`, `scale`, maybe `+`/`*`.
- The v_f32_4x4 SOA analogue may or may not be wanted for integers — scope decision per type.

### SOA-of-vector types

- `v_f32_4x4` exists as the SOA analogue to the 4-lane float vector. If `v_f32_8`, `v_f64_2`, `v_f64_4` are
  introduced, consider whether a matching `v_{type}_{N}x{N}` SOA wrapper is also needed, or whether the raytracer
  can rely on ad-hoc SOA at call sites.

## Tests per type (explicit file list for non-float type)

Using `v_i32_4` as reference (integer — no trig, no special_values):

```
{type}/
    accessors_test.cpp
    blends_test.cpp
    constants_test.cpp
    conversions_test.cpp   (omit if no cross-type conversions defined)
    functions_test.cpp
    operators_test.cpp
    selects_test.cpp
    swizzled_blends_test.cpp
    swizzles_test.cpp
    type_test.cpp
```

For float types, add `functions_trigonometry_test.cpp` and `special_values_test.cpp` (12 files total, matching
`v_f32_4`).

Cross-cutting test files (non-type-specific) may also need to grow:

- `swizzled_blend_masked_{width}{suffix}_test.cpp` — if a new width introduces new masked-blend paths
- `select_{width}{suffix}_test.cpp` — similar
- Per-width `component_*_test.cpp` under `accessors/` if a new integer width introduces new extract/insert paths

## Scope estimate

Rough sizing based on the `v_f32_4` reference (12 headers + 11 sources + 12 tests ≈ **35 files per type**,
call it **~1500–2500 LOC** including tests and boilerplate per type):

| Tier                                         | Types                                        | Approx. files |
|----------------------------------------------|----------------------------------------------|---------------|
| Reuses existing width helpers                | `v_f32_8`, `v_f64_2`, `v_f64_4`              | ~35 each      |
| Needs 256i helpers first                     | `v_i32_8`, `v_ui32_8`, `v_i64_4`, `v_ui64_4` | ~35 + shared  |
| Needs 128i swizzle/swizzled-blend            | `v_i64_2`                                    | ~35 + shared  |
| Needs entirely new 8/16-bit category helpers | `v_{i,ui}{8,16}_*`                           | ~50+ each     |
| Matrix types                                 | `m_{i,ui}32_4x4`                             | ~25 each      |

## Suggested implementation order (updated)

1. ~~**`v_i32_8`**~~ — **DONE.** Full three-tier SSE/AVX/AVX2 implementation with tests. Prerequisite for `v_f32_8`.
2. ~~**`v_ui32_8`**~~ — **DONE.** Unsigned counterpart to `v_i32_8` with tests. Three-tier SSE/AVX/AVX2, reduced
   function surface (no `isNegative`, `abs`, unary `-`; unsigned min/max/shift).
3. **`v_f32_8`** — highest ROI: reuses existing 256s infrastructure, unlocks 8-lane SIMD trig via `avx_mathfun`,
   opens the door to wider primary-ray packets in the raytracer. `VectorBoolType = v_i32_8` (now available).
   No `*3` partial-lane variants (homogeneous W semantic doesn't apply at 8 lanes). Add `Float8` alias to
   `api_type_definitions.h`. Same three-tier SSE/AVX/AVX2 pattern as `v_i32_8`.
4. **`v_f64_2` / `v_f64_4`** — float API symmetry. Reuse 128d/256d infrastructure. No SIMD trig, but completes
   the double-precision surface and is a prerequisite if the raytracer ever adopts double for numerically
   sensitive stages (e.g. geometric intersection tests).
5. **`v_i64_2` / `v_ui64_4` / `v_i64_4`** — 64-bit integer variants; low urgency.
6. **16-bit and 8-bit types** — most specialized, least urgent. These require entirely new cross-cutting helper
   headers (there are no `blend_128_16`/`swizzle_128_8` files today) and would roughly double the per-type effort.
7. **Integer matrices (`m_ui32_4x4`, `m_i32_4x4`)** — only after the underlying integer vector types are solid.

## Related backlog items

- ~~**`unused-256bit-integer-infrastructure.md`**~~ — **COMPLETED AND REMOVED.** All 256i infrastructure, `v_i32_8`,
  and `v_ui32_8` are implemented with full test suites.
- **`header-only-pch-migration.md`** — if migration lands first, new types should be born header-only (inline) to
  benefit from cross-library inlining. Otherwise retrofit later.
- **`soa-mesh-layout.md` / `simd-batch-triangle-test.md`** — would become *much* more attractive once `v_f32_8`
  exists (process 8 triangles per ray in one SIMD op instead of 4).
