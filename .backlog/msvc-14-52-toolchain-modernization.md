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
- The redundant `/O2` is gone (`CMAKE_CXX_FLAGS_RELEASE` already holds `/O2 /Ob2`), and so is the unused `catch2`
  vcpkg dependency.
- clang-cl compiles again, without warnings: `::_mm_load_si128` in `shift.cpp`, `override` on the `findSplitter`
  overrides, `[[maybe_unused]]` on the reference-only `g_lambda` table. All three presets build with 0 warnings.
- The test host crashes are fixed (they were all there at HEAD 11fd329, before the toolchain changes):
  - **Release, 0xC0000005 after 378/384 raytracing tests:** the CppUnitTest executor `FreeLibrary()`s the test DLL,
    which unloaded `vcomp140.dll` while the OpenMP worker threads were still parked inside it. The
    `TEST_MODULE_INITIALIZE` in `tests/raytracing.native-test/src/test_module.cpp` pins the OpenMP runtime.
  - **Debug, exit code 3:** a failed `assert()` opened a modal Abort/Retry/Ignore box. Each test module now sends
    asserts and debug CRT reports to stderr. The asserts it uncovered:
    - Tests that passed out-of-range indices to `replaceComponent()` on purpose. Those checks now run only in Release
      (`#ifdef NDEBUG`).
    - Wrong lane-count asserts in `replace_component_256d.cpp` / `_256s.cpp` (`< 2` instead of `< 4` / `< 8`).
    - Unaligned `Float result[8]` buffers fed to the aligned `Float8` constructor in `illuminants.cpp` and `smits.cpp`.
      This was a real bug in the library, not only in the tests.
    - A zero-length half vector in `GGXTest::evaluateGGX_DGZeroBelowHorizon`.
    - `SmitsTest` reading lane 8 past the end of a 7-element array.
    - 518 test buffers (`std::array`, C arrays, `StandardSample` copies) passed to aligned SIMD loads/stores without
      `alignas`. MSVC happened to place them on 16-byte boundaries; clang does not, so its `movaps` faulted. This was
      the cause of the ~160 random clang-cl 0xC0000005 failures.
  - Now: MSVC Debug 1858/61/384 pass, MSVC Release 1857/61/384 (only `negativeZeroArithmetic`, see 1.), clang-cl
    Release 1853/61/381 (see 3.), with no crashes in any configuration.

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
- Evaluate `/Ob3`.

### 2. constexpr `<cmath>` / LLVM-libc math runtime (`/Zc:cmath`)

In 14.52, `/std:c++23 /Zc:cmath` enables P0533R9 (constexpr `<cmath>`) and a correctly-rounded math runtime based on
LLVM-libc. It is opt-in for now, but Microsoft says `/std:c++23` should imply `/Zc:cmath` in a future release.
Before that happens, measure the effect on scalar `std::exp/pow/sqrt/sin` hot paths (spectral sampling, Fresnel,
noise) with `/fp:fast`, and decide whether to opt in early or pin the old behavior.

### 3. clang-cl (`clang-release` preset): remaining test failures

It builds without warnings, the app renders, and no test crashes. Eight tests still fail deterministically, most likely
because of clang's fast-math flags (`-fassociative-math -freciprocal-math -fno-signed-zeros -ffp-contract=fast`),
which are not equivalent to MSVC's `/fp:fast`:

- vectorization: `v_f32_4_OperatorsTest::testBitwiseAndOperator`, `testBitwiseOrOperator`,
  `testCompoundBitwiseAndAssignOperator`, `testCompoundBitwiseOrAssignOperator` (`Expected:<-nan> Actual:<-nan>`:
  NaN comparison), `computesExp` (relative tolerance at 7.6e+24).
- raytracing: `SphereTest::findNearestIntersectionSetsNormal` (`-1` vs `-1`, probably `-0`/tolerance),
  `BlackbodyTest::hotterObjectPeaksAtShorterWavelength`, `WavelengthsTest::wrapToVisibleWrapsUpperBound` (780 stays
  780 instead of wrapping to 380).

The Cornell box render differs from MSVC in 318 pixels (0.12 %), mostly along the floor/right-wall seam.

The test DLLs and `raytracerui.exe` still need `libomp.dll` (`VC/Tools/Llvm/x64/bin`) on `PATH`.

## Out of scope

- `/feature:APX`, AVX10.2 codegen (depends on hardware; would go with an intrinsics-level bump in `vectorization`).
- ARM64 / ARM64EC.

## Key files

- `CMakeLists.txt` (root compiler / linker flag setup)
- `tests/*/src/test_module.cpp` (test-host setup: OpenMP pin, assert reporting)
- `tests/*/CMakeLists.txt`
