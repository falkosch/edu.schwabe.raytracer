# /fp:fast IEEE compliance issues

## Motivation

The project compiles with `/fp:fast` (MSVC) and `-fassociative-math -freciprocal-math -fno-signed-zeros` (GCC/Clang)
for performance. These flags relax strict IEEE 754 compliance, which causes subtle issues:

- Sign of zero is not preserved across arithmetic (`-0 + 0` may produce `-0` instead of `+0`)
- Operation reordering may change results for edge cases (NaN propagation, infinity arithmetic)
- Tests that assert strict IEEE behavior are fragile — they may pass or fail depending on inlining,
  optimization level, and LTCG codegen

## Known issues

- `v_f32_4_SpecialValuesTest::negativeZeroArithmetic` — asserts `-0 + 0 = +0` (IEEE 754 §6.3),
  fails when LTCG + `/d2SSAOptimizer-` changes codegen path

## Scope

Audit tests and runtime code for assumptions that require strict IEEE compliance. For each case, decide:

1. **Relax the test** — if the strict behavior is not needed for correctness
2. **Guard with `#pragma float_control`** — if a specific function needs IEEE semantics
3. **Use bitwise operations** — if sign preservation matters (e.g. `copysign` instead of arithmetic)
