# Review and fix shift operator semantics across all C++ wrapper types

## Problem

Shift operators in mid-tier types have inconsistent and ambiguous semantics.

### Current behavior

`_mm_sll_epi32(a, b)` takes the lower 64 bits of `b` as a single uniform shift count and applies it to all
lanes of `a`. This is used by v_f32_4, v_i32_4, and v_ui32_4 — meaning `a << b` shifts all components of `a`
by the count extracted from `b[0:63]`, NOT per-component.

v_i32_8 and v_f32_8 use `_mm256_sllv_epi32` (AVX2, per-lane) with scalar loop fallback — these correctly
implement per-component semantics but only on AVX2.

### Target semantics

**I1 — Scalar right operand** (`v << int`): Vectorized component-wise shift by the broadcasted bit count in
scalar integer operand `b`. Each component shifts by the same amount.

**I2 — Vector right operand** (`v << v_int`): Vectorized component-wise shift by the component-wise bit count
in vector operand `b`. Each component shifts by its corresponding count.

Both semantics should hold across all C++ wrapper types equally.

### Affected types

| Type     | Current `<<`/`>>` signature | Current intrinsic        | Correct? |
|----------|-----------------------------|--------------------------|----------|
| v_f32_4  | `(v_f32_4, v_i32_4)`        | `_mm_sll_epi32`          | NO (I2)  |
| v_i32_4  | `(v_i32_4, v_i32_4)`        | (check)                  | NO (I2)  |
| v_ui32_4 | `(v_ui32_4, v_ui32_4)`      | (check)                  | NO (I2)  |
| v_i32_8  | `(v_i32_8, v_i32_8)`        | `_mm256_sllv_epi32`/loop | YES      |
| v_ui32_8 | `(v_ui32_8, v_ui32_8)`      | `_mm256_srlv_epi32`/loop | YES      |
| v_f32_8  | `(v_f32_8, v_i32_8)`        | `_mm256_sllv_epi32`/loop | YES      |
| v_i64_4  | no shift operators          | —                        | MISSING  |
| v_f64_2  | no shift operators          | —                        | N/A      |
| v_f64_4  | `(v_f64_4, v_i64_4)`        | `_mm256_sllv_epi64`/loop | YES      |

### Required changes

1. Add scalar-operand overloads (`v << int`) to all types for I1 semantics (broadcast + uniform shift)
2. Fix 128-bit types' vector overloads to implement true per-lane shift (I2):
    - SSE4 has no per-lane variable shift — requires scalar loop or SSE4.1 `_mm_insert_epi32` tricks
    - Or accept scalar loop for 128-bit per-lane shift (matches v_i32_8's non-AVX2 pattern)
3. Ensure right-shift (`>>`) follows same semantics (arithmetic for signed, logical for unsigned/float)

## Priority

Medium — current behavior is undocumented and inconsistent but rarely used in practice.
