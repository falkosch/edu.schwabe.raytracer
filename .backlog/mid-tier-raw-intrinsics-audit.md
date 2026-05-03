# Audit mid-tier raw intrinsics usage

## Motivation

Mid-tier C++ wrapper types should delegate to basic-tier wrapper functions where possible. Several
operations across multiple types use raw intrinsics directly because no basic-tier wrapper exists:

- `operator-` (negate via xor with NegativeZero)
- `operator~` (bitwise not via xor with MaskAll)
- `operator!` (equality comparison with zero)
- `operator&`, `operator|`, `operator^` (bitwise ops on packed floats)
- All 6 comparison operators (`<`, `>`, `<=`, `>=`, `==`, `!=`)
- `isNegative` (movemask)
- `anyTrue`/`allTrue`/`allFalse`/`anyFalse` (movemask-based)

This pattern exists in v_f32_4, v_f32_8, and partially in integer types.

## Scope

1. Identify all raw intrinsic calls in mid-tier source files
2. For each, determine whether a basic-tier wrapper exists or should be created
3. Add missing basic-tier wrappers (e.g. `negate_256s`, `bitNot_256s`, `compareLess_256s`)
4. Refactor mid-tier to delegate to the new basic-tier wrappers
5. Ensure SSE/AVX/AVX2 conditional compilation lives in basic-tier only

## Priority

Low — current code is correct and tested. This is a structural consistency improvement.
