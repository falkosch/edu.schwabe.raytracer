# Vectorization Library

SIMD wrapper library over SSE4.2/AVX2/FMA intrinsics based on a 3-tier architecture:

1. **Basic tier:** responsible for compiler identification, platform targeting, intrinsic wrapper functions, and
   system-specific utilities e.g., aligned memory allocation. Free functions overloaded for primitive/scalar types (
   `int`, `float`, `double`, ...), and packed types (`__m128`, `__m128d`, `__m128i`, `__m256`, `__m256d`, `__m256i`).

2. **Mid-tier** C++ type wrappers with fixed component count and fixed precision, aligning with SSE register width and
   seamlessly using AVX when available. Delegates operations on packed types to basic-tier functions via `.components`.

3. **API tier** exposes precision-free aliases to mid-tier types (`Float4`, `Int4`, `Float44`, etc.). Default is single
   precision (e.g. `Float4` → `v_f32_4`). Defining `VECTORIZATION_HIGH_PRECISION` switches to double precision. This
   allows experimenting with higher precision at the cost of performance without changing raytracing code.

| Tier       | Paths                                                                                                               |
|------------|---------------------------------------------------------------------------------------------------------------------|
| Basic tier | `architecture/`, `functions/`, `accessors/`, `blends/`, `selects/`, `swizzles/`, `swizzled_blends/`,   `constants/` |
| Mid tier   | `v_f32_4/`, `v_i32_4/`, `v_i32_8/`, `v_ui32_4/`, `v_ui32_8/`, `v_ui64_2/`, `m_f32_4x4/`                             |
| API tier   | `api_type_definitions.h`                                                                                            |

Consumers of the library CAN broad-include via `#include <vectorization.h>`.

### Packed types

| Alias              | Underlying | Width   | Element       |
|--------------------|------------|---------|---------------|
| `PackedFloat4_128` | `__m128`   | 128-bit | 4x `Float_32` |
| `PackedFloat2_128` | `__m128d`  | 128-bit | 2x `Float_64` |
| `PackedInts_128`   | `__m128i`  | 128-bit | integers      |
| `PackedFloat8_256` | `__m256`   | 256-bit | 8x `Float_32` |
| `PackedFloat4_256` | `__m256d`  | 256-bit | 4x `Float_64` |
| `PackedInts_256`   | `__m256i`  | 256-bit | integers      |

### High-level vector types

| Type        | Packed type        | Description              |
|-------------|--------------------|--------------------------| 
| `v_f32_4`   | `PackedFloat4_128` | 4-wide float vector      |
| `v_i32_4`   | `PackedInts_128`   | 4-wide signed int        |
| `v_i32_8`   | `PackedInts_256`   | 8-wide signed int        |
| `v_ui32_4`  | `PackedInts_128`   | 4-wide unsigned int      |
| `v_ui32_8`  | `PackedInts_256`   | 8-wide unsigned int      |
| `v_ui64_2`  | `PackedInts_128`   | 2-wide unsigned int64    |
| `m_f32_4x4` | 4x `v_f32_4`       | 4x4 float matrix         |

## Development guidelines

- New operations MUST be added at both the basic and mid-tiers.
- C++ wrapper types SHOULD be exposed via alias types in the API tier.
- Inside `vectorization/`, compilation units MUST use narrow includes and SHOULD NOT use the aggregator headers at top
  level.

### 256-bit overloads

Many existing `functions/*.h` headers declare 256-bit overloads (`PackedFloat8_256`, `PackedFloat4_256`) that are not
yet implemented in the corresponding `.cpp` files (legacy deferred-implementation pattern). **New generic functions
should fully implement all four packed float types** (`PackedFloat4_128`, `PackedFloat2_128`, `PackedFloat8_256`,
`PackedFloat4_256`). See `.backlog/packed-arithmetic-wrappers.md`.

### File naming and granularity

- One function category per file: `min.h`, `max.h`, `divide.h`, `sqrt.h`, etc.
- Width suffixes on packed-type files: `_128s` (4xf32), `_128d` (2xf64), `_128i` (integer), `_256s` (8xf32),
  `_256d` (4xf64), `_256i` (integer).
- Type-specific files use category names: `accessors.h`, `blends.h`, `constants.h`, `functions.h`, `operators.h`,
  `selects.h`, `swizzles.h`, `swizzled_blends.h`.
- Aggregator headers (`functions.h`, `accessors.h` at the top level) include all sub-headers.

### Intrinsics level and conditional compilation

- `VECTORIZATION_INTRINSICS_LEVEL` defaults to `VECTORIZATION_SSE4` (4 – means use all up to SSE4.2) in
  `compiler_definitions.h`
- **auto-bumps** when the compiler defines `__AVX__`, `__AVX2__`, or `__AVX512F__` (lines 68-81). With the project's
  `/arch:AVX2` compiler flag, the effective level at build time is `VECTORIZATION_AVX2` (6 – means use all up to AVX2).
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
    - Mid-tier: C++ wrapper types MUST guard using AVX packed types and fallback to SSE packed types (e.g. `v_i32_8`):
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
- When not using the specific FMA wrapper functions such as `multiplyAdd`, FMA intrinsics MUST be guarded with:
    ```cpp
    #if VECTORIZATION_FMA
      // FMA intrinsics
    #else
      // chained non-FMA intrinsics fallback
    #else
    ```

### Function signature conventions

- All functions are free functions in the `vectorization` namespace.
- All are `noexcept`.
- Packed-type parameters are `const`; scalars are by value.
- 3-component variants of 4-components types use a `3` suffix: `dot3()`, `length3()`, `normalize3()`.
- Vector-result variants use a `v` suffix: `dotv()` returns a broadcast vector, `dot()` extracts a scalar.
- Vector-result variants and 3-component variants can be combined: `dot3v()` returns broadcast vector of scalar returned
  by `dot3()`.
