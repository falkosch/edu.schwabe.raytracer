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

## Dot product findings (2026-11-21)

From a search for hand-written dot products and sums that mirror `dot3v` / `dotv` (`_mm_dp_ps`); the compiler
findings and the open clang-cl check stay in the open item.

- `v_f64_4::horizontalSumv` (and its `dotv`) now sums `(p0 + p1) + (p2 + p3)` like `v_f32_4`, whose
  high-precision counterpart it is; `SceneObject`'s orientation check uses `dot3v`, as a 4-component `dotv` of a
  direction with a negated normal (w = -0) could return -0 for a perpendicular ray. `v_f32_8::horizontalSumv` keeps
  its order `((p0 + p4) + (p3 + p7)) + ((p1 + p5) + (p2 + p6))`, which the spectral conversions depend on; its
  misleading variable names are fixed.
- Left as is: `v_f32_4::horizontalSum3v` returns `(x + y) + z` (may be -0, unlike `dot3v`; it is a sum, not a dot
  product); `m_f32_4x4` vector × matrix is an FMA chain while matrix × vector matches `dotv` (nothing compares them).
- The mesh block test's lane dot products are explicit `multiplyAdd` chains now (`DONE/simd-batch-triangle-test.md`).

## Commits

- `32cf1df` fix(raytracing): use explicit broadcasts and FMA in the mesh block test
- `90f4065` fix: sum v_f64_4 like v_f32_4 and use dot3v for the orientation check
