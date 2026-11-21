# /fp:fast IEEE compliance issues

## Motivation

The project compiles with `/fp:fast` (MSVC) and `-fassociative-math -freciprocal-math -fno-signed-zeros` (GCC/Clang)
for performance. These flags relax strict IEEE 754 compliance, which causes subtle issues:

- Sign of zero is not preserved across arithmetic (`-0 + 0` may produce `-0` instead of `+0`)
- Operation reordering may change results for edge cases (NaN propagation, infinity arithmetic)
- Tests that assert strict IEEE behavior are fragile — they may pass or fail depending on inlining,
  optimization level, and LTCG codegen

## Already fixed

The `negativeZeroArithmetic` failure and the other special-value, blackbody and spectral-wrap cases found so far are
fixed; see `DONE/fp-fast-ieee-compliance.md`. Those fixes came from failing tests, not from a systematic audit.

## Scope

Audit tests and runtime code for assumptions that require strict IEEE compliance. For each case, decide:

1. **Relax the test** — if the strict behavior is not needed for correctness
2. **Guard with `#pragma float_control`** — if a specific function needs IEEE semantics
3. **Use bitwise operations** — if sign preservation matters (e.g. `copysign` instead of arithmetic)

## Findings (2026-11-21, dot product search)

A search for hand-written dot products and sums that mirror `dot3v` / `dotv` (`_mm_dp_ps`), in a two-file LTCG /
ThinLTO model with the project flags:

- `x + 0` is dropped by MSVC `/fp:fast` with LTCG, and even by `/fp:precise` in a single file; MSVC Debug keeps it
  (no cross-file inlining), so Debug and Release can differ. Don't rely on `+ 0` to turn -0 into +0.
- clang-cl's flags fuse `a * b + c` into FMAs and reorder sums; MSVC Release only keeps the written order because of
  `/d2SSAOptimizer-`. Code that must round in a fixed way should use `multiplyAdd` / `multiplySub` explicitly. Done for
  the mesh block test; that and the fixes of the search are in `DONE/fp-fast-ieee-compliance.md`.
- Open: run `MeshTest::leafBlocksMatchNodeLoop` and the mesh benchmark checksums under `clang-release`; the
  single-facet test's own `determinant - (detU + detV)` may be reordered there too.
