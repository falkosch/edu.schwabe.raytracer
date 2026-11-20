# /fp:fast IEEE compliance issues (done parts)

Carved out of `.backlog/fp-fast-ieee-compliance.md`. The general audit of tests and runtime code stays open there.

## Known issues (resolved)

- `v_f32_4_SpecialValuesTest::negativeZeroArithmetic` — asserts `-0 + 0 = +0` (IEEE 754 §6.3),
  fails when LTCG + `/d2SSAOptimizer-` changes codegen path

### Resolution

The special-value tests ran their arithmetic on compile-time constants, so they tested the compiler's `/fp:fast`
constant folding rather than the SIMD instructions. Fixed in `668837b` (test: make IEEE special-value and
compiler-sensitive tests robust):

- `opaque()` (`tests/vectorization.native-test/include/opaque.h`) routes test operands through volatile memory, so the
  instructions under test really execute. Used in the -0/inf/NaN arithmetic tests of `v_f32_4` and `v_f32_8`
  (`special_values_test.cpp`) and in the movemask and multiply tests.
- Bitwise-operator tests compare bit patterns (mask lanes are NaNs).
- `computesExp` allows 4 ε (`exp_ps` is 1 ULP off `std::exp` with MSVC, 2 ULP with clang-cl).
- The sphere-normal test compares with a tolerance.
- The blackbody ratio test takes its temperatures at runtime; clang-cl's folding of the all-constant expression
  failed it.

Runtime code fixed alongside in `bf0dfe0` (fix(raytracing): keep spectral wrap in range and compute blackbody in
nanometers):

- `wrapToVisible()`/`wavelengthsAt()` fold boundary results back into [380, 780); reassociation used to leave 780 at
  780.
- `blackbody()` works in nanometers; in meters, λ⁵ ≈ 1e-32 and the (1e-9)⁵ factor were at the edge of the float range
  under reassociation.

All tests pass in MSVC Debug, MSVC Release and clang-cl Release.
