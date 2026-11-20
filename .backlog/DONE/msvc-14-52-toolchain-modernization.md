# MSVC 14.52 / VS 2026 18.12 build-system follow-ups (done parts)

Carved out of `.backlog/msvc-14-52-toolchain-modernization.md`. Re-enabling the SSA optimizer and the open clang-cl
points stay there.

## Toolchain adoption

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

## constexpr `<cmath>` / LLVM-libc math runtime (`/Zc:cmath`): done, pinned off

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

## Commits

- `7a8504a` feat(build): adopt MSVC 14.52 toolchain and CMake presets
- `bc553dc` fix(build): restore warning-free clang-cl build and drop redundant flags
- `46e61b6` fix(raytracing, vectorization): align spectral Float8 buffers and fix 256-bit lane asserts
- `c7353c9` fix(tests): resolve test host crashes and Debug aborts
- `eee6a08` build(msvc): pin /Zc:cmath- and document why the SSA optimizer stays off
- `bf0dfe0` fix(raytracing): keep spectral wrap in range and compute blackbody in nanometers
- `668837b` test: make IEEE special-value and compiler-sensitive tests robust
