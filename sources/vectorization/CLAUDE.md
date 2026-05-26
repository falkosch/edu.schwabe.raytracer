# Vectorization Library

SIMD wrapper library over SSE4.2/AVX2/FMA intrinsics based on a 3-tier architecture:

1. **Basic tier:** compiler identification, platform targeting, intrinsic wrapper functions, aligned memory allocation.
   Free functions overloaded for scalar types (`int`, `float`, `double`, ...) and packed types (`__m128`, `__m128d`,
   `__m128i`, `__m256`, `__m256d`, `__m256i`).

2. **Mid-tier:** C++ type wrappers with fixed component count and precision. Delegates to basic-tier via `.components`.

3. **API tier:** precision-free aliases (`Float4`, `Int4`, `MFloat4x4`, etc.). Default single precision (e.g.
   `Float4` -> `v_f32_4`). Define `VECTORIZATION_HIGH_PRECISION` to switch to double.

| Tier       | Paths                                                                                                                                                            |
|------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Basic tier | `architecture/`, `functions/`, `accessors/`, `blends/`, `selects/`, `swizzles/`, `swizzled_blends/`, `constants/`, `conversions/`                                |
| Mid tier   | `v_f32_4/`, `v_f32_8/`, `v_f64_2/`, `v_f64_4/`, `v_i32_4/`, `v_i32_8/`, `v_i64_2/`, `v_i64_4/`, `v_ui32_4/`, `v_ui32_8/`, `v_ui64_2/`, `v_ui64_4/`, `m_f32_4x4/` |
| API tier   | `api_type_definitions.h`                                                                                                                                         |

Consumers CAN broad-include via `#include <vectorization.h>`.

### Packed types

| Alias              | Underlying | Width   | Element       |
|--------------------|------------|---------|---------------|
| `PackedFloat4_128` | `__m128`   | 128-bit | 4x `Float_32` |
| `PackedFloat2_128` | `__m128d`  | 128-bit | 2x `Float_64` |
| `PackedInts_128`   | `__m128i`  | 128-bit | integers      |
| `PackedFloat8_256` | `__m256`   | 256-bit | 8x `Float_32` |
| `PackedFloat4_256` | `__m256d`  | 256-bit | 4x `Float_64` |
| `PackedInts_256`   | `__m256i`  | 256-bit | integers      |

### Mid-tier vector types

| Type        | Packed type        | Description           |
|-------------|--------------------|-----------------------|
| `v_f32_4`   | `PackedFloat4_128` | 4-wide float32 vector |
| `v_f32_8`   | `PackedFloat8_256` | 8-wide float32 vector |
| `v_f64_2`   | `PackedFloat2_128` | 2-wide float64 vector |
| `v_f64_4`   | `PackedFloat4_256` | 4-wide float64 vector |
| `v_i32_4`   | `PackedInts_128`   | 4-wide signed int32   |
| `v_i32_8`   | `PackedInts_256`   | 8-wide signed int32   |
| `v_i64_2`   | `PackedInts_128`   | 2-wide signed int64   |
| `v_i64_4`   | `PackedInts_256`   | 4-wide signed int64   |
| `v_ui32_4`  | `PackedInts_128`   | 4-wide unsigned int32 |
| `v_ui32_8`  | `PackedInts_256`   | 8-wide unsigned int32 |
| `v_ui64_2`  | `PackedInts_128`   | 2-wide unsigned int64 |
| `v_ui64_4`  | `PackedInts_256`   | 4-wide unsigned int64 |
| `m_f32_4x4` | 4x `v_f32_4`       | 4x4 float32 matrix    |

## Development guidelines

- New operations MUST be added at both the basic and mid-tiers.
- C++ wrapper types SHOULD be exposed via alias types in the API tier.
- Inside `vectorization/`, compilation units MUST use narrow includes and SHOULD NOT use aggregator headers.

### Third-party dependencies

- `3rdparty/sse_mathfun.h` — SSE float32 transcendentals (sin, cos, exp, log)
- `3rdparty/avx_mathfun.h` — AVX float32 transcendentals (8-lane variants)
- No SIMD transcendentals for float64; those types fall back to scalar `<cmath>`.

### File naming and granularity

- One function category per file: `min.h`, `max.h`, `divide.h`, `sqrt.h`, etc.
- Width suffixes on packed-type files: `_128s` (4xf32), `_128d` (2xf64), `_128i` (integer), `_256s` (8xf32),
  `_256d` (4xf64), `_256i` (integer).
- Type-specific files use category names: `accessors.h`, `blends.h`, `constants.h`, `functions.h`, `operators.h`,
  `selects.h`, `swizzles.h`, `swizzled_blends.h`.
- Aggregator headers (`functions.h`, `accessors.h` at the top level) include all sub-headers.

### Intrinsics level and conditional compilation

- `VECTORIZATION_INTRINSICS_LEVEL` defaults to `VECTORIZATION_SSE4` (4) in `compiler_definitions.h`.
- **Auto-bumps** when the compiler defines `__AVX__`, `__AVX2__`, or `__AVX512F__`. With MSVC's `/arch:SSE4.2` the
  effective level stays `VECTORIZATION_SSE4`; with clang-cl's `/arch:AVX2` or GCC's `-march=native` (on AVX2 CPUs) it
  becomes `VECTORIZATION_AVX2` (6).
- Guard higher-level intrinsics with:
    ```cpp
    #if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
      // AVX2 intrinsics
    #elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      // AVX intrinsics
    #else
      // SSE4.2 fallback
    #endif
    ```
    - Basic tier: Code using AVX-native packed types (e.g. `__m256`, `PackedFloat8_256`) MUST guard AVX2 intrinsics:
        ```cpp
        #if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
          // AVX2 intrinsics
        #else
          // AVX intrinsics fallback
        #endif
        ```
    - Mid-tier: 256-bit C++ wrapper types MUST guard using AVX packed types and fall back to a pair of 128-bit halves:
        ```cpp
        using HalfPackedType = PackedTypes<ValueType, HALF_SIZE>::Type;

        #if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
            using PackedType = PackedTypes<ValueType, SIZE>::Type;
        #else
            struct PackedType {
                HalfPackedType lo;
                HalfPackedType hi;
            };
        #endif
        ```
- FMA intrinsics (outside the `multiplyAdd`/`multiplySub` wrapper functions) MUST be guarded with:
    ```cpp
    #if VECTORIZATION_FMA
      // FMA intrinsics
    #else
      // chained non-FMA intrinsics fallback
    #endif
    ```

### Function signature conventions

- All functions are free functions in the `vectorization` namespace.
- All are `noexcept`.
- Packed-type parameters are `const`; scalars are by value.
- 3-component variants use a `3` suffix: `dot3()`, `length3()`, `normalize3()`.
- Vector-result variants use a `v` suffix: `dotv()` returns a broadcast vector, `dot()` extracts a scalar.
- Suffixes combine: `dot3v()` returns broadcast vector of scalar returned by `dot3()`.
