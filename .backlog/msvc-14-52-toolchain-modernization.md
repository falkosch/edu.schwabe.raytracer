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

These items are still open.

## Scope

### 1. Optimizer switches (needs benchmarks: `benchmarks` + Cornell box render time)

- Drop `/d2SSAOptimizer-`. It was added in 24b51fe (May 2026, MSVC 14.50) to work around an O(n²) SSA optimizer
  stall during the LTCG link. Checked on 14.52.37025 (2026-11-19):
  - **The stall is gone.** A full Release build takes 21 s with the SSA optimizer on and 16 s with the flag.
  - **It pays off.** `benchmarks.exe`, cycles/intersection, measured with the old hand-written `/GL` + `/LTCG`
    setup and `/MD` (one run each):

    | Benchmark | `/d2SSAOptimizer-` | SSA on |
    |---|---|---|
    | AABBx2 overlaps | 9.69 | 9.94 |
    | AABB nearest | 10.34 | 8.93 |
    | BoundingSphere | 40.94 | 20.67 |
    | SplittingPlane | 9.87 | 10.68 |
    | Mesh facet | 45.02 | 40.31 |

    After the switch to CMake IPO (`lib /LTCG`) and `/MT`, with SSA still off, the numbers were 10.37 / 10.41 /
    **20.33** / 9.94 / 45.76. The BoundingSphere gain therefore comes from cross-library inlining, not from the SSA
    optimizer. Measure again on the current setup before deciding.
  - **Blocked by `fp-fast-ieee-compliance`.** With SSA on, 6 vectorization tests fail instead of 1:
    `negativeZeroArithmetic`, `movemaskDetectsNegativeZero`, `multipliesWithInfinity`, `infinityArithmetic`,
    `infinityMinusInfinityIsNaN`, `zeroTimesInfinityIsNaN`. Under `/fp:fast`, the SSA optimizer folds inf/NaN/−0
    arithmetic. Resolve those tests first (relax them or use `#pragma float_control`), then remove the flag.
- Release passes `/O2` twice: `CMAKE_CXX_FLAGS_RELEASE` already holds `/O2 /Ob2`. Evaluate `/Ob3`.

### 2. constexpr `<cmath>` / LLVM-libc math runtime (`/Zc:cmath`)

In 14.52, `/std:c++23 /Zc:cmath` enables P0533R9 (constexpr `<cmath>`) and a correctly-rounded math runtime based on
LLVM-libc. It is opt-in for now, but Microsoft says `/std:c++23` should imply `/Zc:cmath` in a future release.
Before that happens, measure the effect on scalar `std::exp/pow/sqrt/sin` hot paths (spectral sampling, Fresnel,
noise) with `/fp:fast`, and decide whether to opt in early or pin the old behavior.

### 3. Test host crashes (all were already there at HEAD 11fd329, before the toolchain changes)

- **MSVC Release:** `raytracing.native-test.dll` crashes the test host (0xC0000005) after 378 of 384 tests. The tests
  that don't run (`WavelengthsTest::wrapToVisible*`, `wavelengthsAt*`) pass when run on their own
  (`/TestCaseFilter:"FullyQualifiedName~WavelengthsTest"`), so an earlier test probably leaves bad state behind (heap
  corruption, or the logger worker thread at teardown).
- **MSVC Debug:** the test host aborts (exit code 3) after 23 vectorization tests and after 222 raytracing tests. It
  happens the same way with HEAD's `/MDd` + `-std:c++latest`. Probably a debug CRT or STL assertion that ends in
  `abort()`. Run it under the debugger, or with `_CrtSetReportMode` sending reports to stderr, to see which one.

### 4. clang-cl (`clang-release` preset) is broken

- **Compile error since f8d511e:** `shift.cpp:257` and `shift.cpp:274` call `_mm_load_si128(reinterpret_cast<const
  __m128i *>(va))` unqualified. Inside `namespace vectorization`, the overload `_mm_load_si128(const UInt_32 *)` from
  `compiler_intrinsics.h` hides the global intrinsic. MSVC accepts the call; clang rejects it. The fix is
  `::_mm_load_si128`. That change was tried on a scratch copy and not applied.
- **Runtime:** with the fix applied, vectorization has about 160 failures and raytracing about 15–18, almost all
  0xC0000005. Which tests fail changes from run to run, and it happens with or without ThinLTO. That pattern suggests
  memory alignment or stack corruption (check `/Zp16`, aligned loads of stack or unaligned data, and the CppUnitTest
  ABI under clang-cl). Primitives passes 61/61.
- The test DLLs need `libomp.dll` (`VC/Tools/Llvm/x64/bin`) on `PATH`. Without it the raytracing DLL doesn't load.
- 13 warnings: 12 × `findSplitter` overrides a member function but isn't marked `override`
  (`-Winconsistent-missing-override`), and 1 × unused `g_lambda`.

### 5. Minor

- `vcpkg.json` lists `catch2`, but nothing uses it. Every configure installs it for nothing.

## Out of scope

- `/feature:APX`, AVX10.2 codegen (depends on hardware; would go with an intrinsics-level bump in `vectorization`).
- ARM64 / ARM64EC.

## Key files

- `CMakeLists.txt` (root compiler / linker flag setup)
- `sources/vectorization/src/vectorization/functions/shift.cpp`
- `tests/*/CMakeLists.txt`
- `vcpkg.json`
