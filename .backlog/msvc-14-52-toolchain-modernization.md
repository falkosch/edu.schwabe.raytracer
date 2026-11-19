# MSVC 14.52 / VS 2026 18.12 build-system follow-ups

## Motivation

Visual Studio 2026 18.12 (MSVC Build Tools 14.52, `cl` 19.52.37025) is installed. These changes are already done:

- `/std:c++23` for MSVC ≥ 19.52. CMake ≤ 4.4.2 would otherwise pass `-std:c++latest` (C++26 draft).
- `/Zc:__cplusplus /Zc:preprocessor` for MSVC.
- `cmake_minimum_required(VERSION 4.0...4.3)`. The runtime library, debug info format and runtime checks now come from
  CMake's abstractions (CMP0091/0141/0184). The `/W`/`/MD` regex hacks are gone. Module scanning is off
  (`CMAKE_CXX_SCAN_FOR_MODULES`).
- `CMAKE_MSVC_RUNTIME_LIBRARY` matches the vcpkg triplet: `/MT`/`/MTd` for `*-static`, which fixes the old `/MD` vs.
  static-vcpkg CRT mix.
- `CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE` replaces the hand-written `/GL` + `/LTCG`. MSVC now also runs
  `lib /LTCG` on the static libraries. clang-cl, which ignored `/GL`, now gets ThinLTO with lld-link.
- `CMakePresets.json` (`debug`, `release`, `clang-release`) replaces `CMakeSettings.json`. The MinGW configuration was
  dropped: it hard-coded `C:/msys64` (not installed) and GCC 7.3, which can't build C++23, and the CppUnitTest tests
  are MSVC-only anyway.
- The redundant `/O2` and the unused `catch2` vcpkg dependency are gone. clang-cl compiles without warnings. All three
  presets build with 0 warnings.
- The test host crashes and Debug aborts are fixed (OpenMP runtime pinned, asserts to stderr, 518 test buffers
  aligned, several test and library bugs; see `c7353c9`, `46e61b6`).
- The `/fp:fast` special-value tests (`negativeZeroArithmetic` etc.) ran their arithmetic on compile-time constants,
  so they tested the compiler's constant folding. Their operands now go through `opaque()`
  (`tests/vectorization.native-test/include/opaque.h`), so the SIMD instructions really execute.
- clang-cl: all tests pass.
  - The bitwise-operator tests compare bit patterns (mask lanes are NaNs).
  - `computesExp` allows 4 ε (`exp_ps` is 2 ULP off `std::exp` with clang, 1 ULP with MSVC).
  - The sphere-normal test has a tolerance.
  - `wrapToVisible()`/`wavelengthsAt()` fold boundary results back into [380, 780). Reassociation used to leave
    780 at 780.
  - `blackbody()` works in nanometers. In meters, λ⁵ ≈ 1e-32 and the (1e-9)⁵ factor are at the edge of the float
    range under reassociation.
  - The blackbody ratio test takes its temperatures at runtime. With every input constant, clang's folding fails it.
- Result: 1858/61/384 tests pass in MSVC Debug, MSVC Release and clang-cl Release. The MSVC Release render differs
  from 11fd329 in 5 pixels by 1/255 (blackbody constants now derived in double).

## Findings (2026-11-19)

### Build times (clean Release build, compile and link measured separately)

615 objects, Ninja, 32 logical cores:

| Variant | compile | link phase | `raytracerui.exe` link | `benchmarks.exe` link |
|---|---|---|---|---|
| A: `/d2SSAOptimizer-`, `/Ob2` (current) | 10.3 s | 9.7 s | 3.3 s | 3.3 s |
| B: SSA on, `/Ob2` | 10.5 s | 10.8 s | 3.3 s | 3.2 s |
| C: SSA on, `/Ob3` | 14.0 s | 12.3 s | 10.5 s | 5.7 s |
| D: `/d2SSAOptimizer-`, `/Ob3` | 13.3 s | 12.8 s | 11.3 s | 5.2 s |

The 14.50 LTCG link stall that `/d2SSAOptimizer-` was introduced for is gone in 14.52. `/Ob3` triples the exe link
time.

### Benchmarks (`benchmarks.exe`, cycles/intersection, min / median of 6 interleaved runs)

| Benchmark | A | B | C |
|---|---|---|---|
| AABBx2 overlaps | 10.04 / 10.62 | 9.98 / 10.79 | 9.78 / 10.40 |
| AABB | 9.76 / 10.73 | 9.34 / 10.00 | 9.52 / 9.85 |
| BoundingSphere | 19.83 / 20.68 | 19.00 / 20.04 | 18.85 / 19.80 |
| SplittingPlane | 9.70 / 10.22 | 9.84 / 11.00 | 9.67 / 10.26 |
| Mesh facet | 44.01 / 46.02 | 42.66 / 43.92 | 41.46 / 43.35 |

Cornell box render, 512×512 (min / median of 6): A 50.5 / 61.8 ms, B 44.5 / 56.0 ms, C 43.2 / 48.7 ms.

D (`/d2SSAOptimizer-` + `/Ob3`) was only measured in a first, noisier batch of 3 runs, where A had:

| Benchmark | A | D |
|---|---|---|
| AABBx2 overlaps | 10.08 / 11.18 | 10.52 / 11.93 |
| AABB | 9.68 / 10.51 | 9.82 / 11.53 |
| BoundingSphere | 19.48 / 24.63 | 24.20 / 27.18 |
| SplittingPlane | 9.51 / 10.79 | 12.65 / 14.23 |
| Mesh facet | 45.25 / 51.43 | 45.34 / 45.36 |

D's render time was not measured.

### `/d2SSAOptimizer-` stays: MSVC 14.52 miscompiles with the SSA optimizer

With `/GL` + `/fp:fast` and the SSA optimizer on, `(x * c) * c` evaluates to 0 when `c` is a float constant built
from integer bits (`_mm_castsi128_ps`), as the vectorization library builds `Two<…>()`, `Half<…>()`, `One<…>()` and
others. A float-literal constant (`_mm_set1_ps(2.0f)`) is fine, and so is `x * (c * c)`. `(x * One) * One` happened
to work. Without LTCG, with `/d2SSAOptimizer-`, or with `/fp:precise` the result is correct. Repro (two TUs,
`cl /O2 /GL /arch:AVX2 /fp:fast lib.cpp main.cpp && link /LTCG lib.obj main.obj`):

```cpp
// lib.cpp
#include <immintrin.h>
__m128 two() noexcept { return _mm_castsi128_ps(_mm_set1_epi32(0x40000000)); }
// main.cpp
#include <immintrin.h>
#include <cstdio>
__m128 two() noexcept;
int main() {
    volatile float vx = 1.19209290e-07f;
    const __m128 x = _mm_set1_ps(vx);
    std::printf("%g\n", _mm_cvtss_f32(_mm_mul_ps(_mm_mul_ps(x, two()), two())));  // prints 0, expected 4.76837e-07
}
```

In the test suite this zeroed the `computesExp` tolerance (`Epsilon * Two * Two`). With the SSA optimizer on, the
render differed in 0.93 % of the pixels. In a standalone repro the same optimizer also rewrote a machine-epsilon
loop like the one in `epsilon.cpp`, `while (f + 1 != 1)`, into `while (f != 0)` (allowed under `/fp:fast`), which
measures 1.4e-45 instead of 1.19e-07. In the library LTCG then folded `Epsilon` inconsistently with its runtime
value. The startup measurement stays (the hardware epsilon is not guaranteed to be `numeric_limits`), which is
another reason to keep the SSA optimizer off.

## Scope (still open)

### 1. Re-enable the SSA optimizer later

Decided 2026-11-19: the SSA optimizer stays off. It would be worth about 1–4 % on the micro-benchmarks and about
10 % render time. Possible later:

- Re-run the repro above on newer MSVC versions.
- Build the float constants from float literals instead of integer bit patterns, then re-run the test suite and
  the render diff with the SSA optimizer on. The machine-epsilon measurement would need a look as well.

`/Ob3` is not applied either (decided 2026-11-19). With the SSA optimizer on it adds 0–3 %. With it off, which is
the current setting, it was not faster (D above, 3 runs only). It also raises compile time by about 30 % and the
`raytracerui.exe` link from 3.3 s to 11.3 s. If the SSA optimizer is ever re-enabled, measure `/Ob3` again: 6
interleaved runs plus render time.

### 2. constexpr `<cmath>` / LLVM-libc math runtime (`/Zc:cmath`): done, pinned off

In 14.52, `/std:c++23 /Zc:cmath` enables P0533R9 (constexpr `<cmath>`) and the LLVM-libc based math runtime. It is
opt-in for now, but Microsoft says `/std:c++23` should imply `/Zc:cmath` in a future release.

- `cl` accepts `/Zc:cmath` and `/Zc:cmath-`. With `/Zc:cmath`, `constexpr std::floor/fmod/copysign/fmin` compile;
  without it or with `/Zc:cmath-` they don't. `sqrt`/`exp`/`sin` are not constexpr in C++23 either way. The project
  doesn't use constexpr `<cmath>`.
- Scalar float functions, standalone (`/O2 /fp:fast /arch:AVX2 /MT`, no auto-vectorization, 1M inputs; ns/call):

  | function | `/Zc:cmath-` (current) | `/Zc:cmath` |
  |---|---|---|
  | exp | 2.07 | 7.46 |
  | log | 2.16 | 2.71 |
  | pow | 6.23 | 9.35 |
  | sin | 6.96 | 8.62 |
  | cos | 6.65 | 9.92 |
  | tan | 5.30 | 9.39 |
  | acos | 8.23 | 8.09 |
  | atan2 | 15.06 | 14.83 |
  | sqrt | 0.63 | 0.63 |

  Accuracy is the same in both modes: 0 ULP against the double result rounded to float (`pow` at most 1 ULP).
- The project with `/Zc:cmath`: all tests pass and the Cornell box render is bit-identical. Render time and the
  intersection benchmarks show no difference beyond the noise, as the hot paths use the vectorized
  `sse_mathfun`/`avx_mathfun` code, not `<cmath>`. Scalar `<cmath>` is reached through the vectorization scalar wrappers,
  `v_f64_4`, `v_f32_4::pow` (tone mapping), texture-coordinate generation and HDR loading. Build time is unchanged.
- Decided and applied 2026-11-19: `/Zc:cmath-` is pinned for MSVC (root `CMakeLists.txt`). It keeps the faster
  current runtime when the default flips, and nothing in the project needs constexpr `<cmath>`. Re-evaluate if
  constexpr `<cmath>` becomes useful or the LLVM-libc runtime gets faster.

### 3. clang-cl (secondary)

MSVC is the focus. For clang-cl, keep it compiling and spot-check performance, without chasing every difference.

- Performance spot check (2026-11-19, MSVC with `/d2SSAOptimizer-` and `/Ob2` vs clang-cl ThinLTO, min / median):
  Cornell box render 51.8 / 67.2 ms vs 35.5 / 53.9 ms. Cycles/intersection: AABBx2 13.51 / 14.28 vs
  6.89 / 7.75, AABB 10.68 / 15.04 vs 9.63 / 9.71, BoundingSphere 22.92 / 27.68 vs 19.36 / 25.72,
  SplittingPlane 10.88 / 12.58 vs 8.06 / 9.40, Mesh facet 49.96 / 64.06 vs 37.14 / 46.94.

- The test DLLs and `raytracerui.exe` still need `libomp.dll` (`VC/Tools/Llvm/x64/bin`) on `PATH`.
- The render differs from MSVC in about 0.12 % of the pixels (floor/right-wall seam, silhouettes).
- `BlackbodyTest::hotterObjectPeaksAtShorterWavelength` failed only when every input was a compile-time constant,
  and passed as soon as any value was observed or the temperatures came in at runtime. The root cause in clang's
  folding was not analyzed. `LightInfo::blackbody(3000.f, …)` in the Cornell box also passes a constant
  temperature, so app code could in theory be affected the same way; the clang-cl render looks correct.

## Out of scope

- `/feature:APX`, AVX10.2 codegen (depends on hardware; would go with an intrinsics-level bump in `vectorization`).
- ARM64 / ARM64EC.

## Key files

- `CMakeLists.txt` (root compiler / linker flag setup)
- `sources/vectorization/src/vectorization/constants/` (integer-built float constants)
- `tests/*/src/test_module.cpp`, `tests/vectorization.native-test/include/opaque.h`
