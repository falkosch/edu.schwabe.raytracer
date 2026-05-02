# Vectorization Library

SIMD wrapper library over SSE4/AVX/FMA/AVX2 intrinsics. Provides type-safe C++ abstractions over raw `__m128`,
`__m256`, and `__m128i` registers.

## Three-tier architecture

1. **Basic tier** (`architecture/`, `functions/`, `accessors/`, `blends/`, `selects/`, `swizzles/`, `swizzled_blends/`,
   `constants/`) — compiler/platform identification, intrinsic wrapper functions, and system-specific utilities (e.g.
   aligned memory allocation). Free functions overloaded for scalar types (`Float_32`, `Float_64`) and packed types
   (`PackedFloat4_128`, `PackedFloat2_128`, etc.). These call intrinsics directly.

2. **Mid tier** (`v_f32_4/`, `v_i32_4/`, `v_ui32_4/`, `v_ui64_2/`, `m_f32_4x4/`) — C++ type wrappers with fixed
   component count and fixed precision, aligning with SSE register width and seamlessly using AVX when available.
   Delegate to basic-tier functions via `.components`.

3. **API tier** (`api_type_definitions.h`) — exposes aliases (`Float4`, `Int4`, `Float44`, etc.) to mid-tier types
   based on a configured precision. Default is single precision (e.g. `Float4` → `v_f32_4`); defining
   `VECTORIZATION_HIGH_PRECISION` switches to double precision. This allows experimenting with higher precision at the
   cost of performance without changing raytracing code.

New operations must be added at both the basic and mid tiers.

## Directory layout

Parallel `include/` and `src/` trees with mirrored structure:

```
include/vectorization/
├── architecture/        — compiler defs, intrinsic aliases, meta types (6 files)
├── accessors/           — element get/replace per width suffix (10 files)
├── blends/              — template blending per width suffix (11 files)
├── constants/
│   ├── masks/           — MaskAll, MaskX, MaskXY, etc. (12 files)
│   └── values/          — Zero, One, Pi, Epsilon, Infinity, etc. (26 files)
├── conversions/         — native + complex (2 files)
├── functions/           — all math/arithmetic ops (47 files)
├── selects/             — conditional selection via masks (8 files)
├── swizzles/            — element reordering (4 files)
├── swizzled_blends/     — combined swizzle+blend (11 files)
├── v_f32_4/             — 4×f32 type: type, api, accessors, blends, constants,
│                          conversions, functions, functions_trigonometry,
│                          operators, selects, swizzles, swizzled_blends (12 files)
├── v_i32_4/             — 4×i32 type (9 files, same categories minus trig/conversions)
├── v_ui32_4/            — 4×u32 type (9 files)
├── v_ui64_2/            — 2×u64 type (9 files)
├── v_f32_4x4/           — 4×v_f32_4 stacking (6 files)
└── m_f32_4x4/           — 4×4 matrix type (6 files)
```

Aggregator headers at top level (`functions.h`, `accessors.h`, `blends.h`, etc.) include all sub-headers.

## Adding a new function

1. Create `include/vectorization/functions/<name>.h` — declare overloads for relevant scalar and packed types.
2. Create `src/vectorization/functions/<name>.cpp` — implement each overload.
3. Add `#include "functions/<name>.h"` to `include/vectorization/functions.h` (alphabetical).
4. For each high-level type that should expose the function:
    - Declare the wrapper in `include/vectorization/<type>/functions.h`.
    - Implement it in `src/vectorization/<type>/functions.cpp`.
5. Add all new `.h` and `.cpp` files to `CMakeLists.txt` — there is no globbing; every file must be listed explicitly
   in `SOURCES` and `HEADERS`.

## 256-bit overloads

Many existing `functions/*.h` headers declare 256-bit overloads (`PackedFloat8_256`, `PackedFloat4_256`) that are not
yet implemented in the corresponding `.cpp` files (legacy deferred-implementation pattern). **New generic functions
should fully implement all four packed float types** (`PackedFloat4_128`, `PackedFloat2_128`, `PackedFloat8_256`,
`PackedFloat4_256`). See `.backlog/packed-arithmetic-wrappers.md`.

## File naming and granularity

- One function category per file: `min.h`, `max.h`, `divide.h`, `sqrt.h`, etc.
- Width suffixes on packed-type files: `_128s` (4xf32), `_128d` (2xf64), `_128i` (integer), `_256s` (8xf32),
  `_256d` (4xf64), `_256i` (integer).
- Type-specific files use category names: `accessors.h`, `blends.h`, `constants.h`, `functions.h`, `operators.h`,
  `selects.h`, `swizzles.h`, `swizzled_blends.h`.
- Aggregator headers (`functions.h`, `accessors.h` at the top level) include all sub-headers.

## Operation categories — width coverage

Each category has a complete and symmetric set of width variants:

| Category              | Suffixes                                            |
|-----------------------|-----------------------------------------------------|
| accessors             | 128s, 128d, 128i, 256s, 256d                        |
| replace_component     | 128s, 128d, 128i, 256s, 256d                        |
| blends                | 128s, 128d, 256s, 256d                              |
| blend_masked          | 128s, 128d, 128i, 256s, 256d, 256i, scalar          |
| selects               | 128s, 128d, 128i, 256s, 256d, 256i, generic, scalar |
| swizzles              | 128s, 128d, 256s, 256d                              |
| swizzled_blend        | 128s, 128d, 256s, 256d                              |
| swizzled_blend_masked | 128s, 128d, 256s, 256d (template-only)              |

## Intrinsics level and conditional compilation

`VECTORIZATION_INTRINSICS_LEVEL` defaults to `VECTORIZATION_SSE4` (4) in `compiler_definitions.h`, but **auto-bumps**
when the compiler defines `__AVX__`, `__AVX2__`, or `__AVX512F__` (lines 68-81). With the project's `/arch:AVX2`
compiler flag, the effective level at build time is `VECTORIZATION_AVX2` (6).

Guard higher-level intrinsics with:

```cpp
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
  // FMA3, AVX2 intrinsics
#else
  // SSE4 fallback
#endif
```

AVX2 implies FMA3 support on all mainstream x86 CPUs (CPUID bits are technically independent;
the compiler can be invoked with `-mavx2 -mno-fma`). See `.backlog/fma-intrinsics.md` for the
dedicated `VECTORIZATION_FMA` flag that handles this edge case.

## Unity build

CMake unity build is enabled (`UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 0`). The fine-grained file splitting (one
category per file) exists partly to keep unity translation units manageable. Avoid large monolithic files.

## Packed types

| Alias              | Underlying | Width   | Element       |
|--------------------|------------|---------|---------------|
| `PackedFloat4_128` | `__m128`   | 128-bit | 4x `Float_32` |
| `PackedFloat2_128` | `__m128d`  | 128-bit | 2x `Float_64` |
| `PackedInts_128`   | `__m128i`  | 128-bit | integers      |
| `PackedFloat8_256` | `__m256`   | 256-bit | 8x `Float_32` |
| `PackedFloat4_256` | `__m256d`  | 256-bit | 4x `Float_64` |
| `PackedInts_256`   | `__m256i`  | 256-bit | integers      |

## High-level vector types

| Type        | Packed type        | Description           |
|-------------|--------------------|-----------------------|
| `v_f32_4`   | `PackedFloat4_128` | 4-wide float vector   |
| `v_i32_4`   | `PackedInts_128`   | 4-wide signed int     |
| `v_ui32_4`  | `PackedInts_128`   | 4-wide unsigned int   |
| `v_ui64_2`  | `PackedInts_128`   | 2-wide unsigned int64 |
| `m_f32_4x4` | 4x `v_f32_4`       | 4x4 float matrix      |

## Function signature conventions

- All functions are free functions in the `vectorization` namespace.
- All are `noexcept`.
- Packed-type parameters are `const &`; scalars are by value.
- Vector-result variants use a `v` suffix: `dotv()` returns a broadcast vector, `dot()` extracts a scalar.
- 3-component variants use a `3` suffix: `dot3()`, `length3()`, `normalize3()`.
- Inline-only / template-only headers (e.g. `broadcast.h`, `static_average.h`, `static_for.h`,
  `static_normalize.h`) have no corresponding `.cpp` — this is intentional.
