# Expose and test vectorized trigonometric functions

## Motivation

The 3rdparty `sse_mathfun.h` and `avx_mathfun.h` provide vectorized `sin_ps`, `cos_ps`, `sincos_ps` (and 256-bit
variants `sin256_ps`, `cos256_ps`, `sincos256_ps`). Currently only `log_ps` and `exp_ps` are wrapped in the public API
as `vectorization::log(v_f32_4)` and `vectorization::exp(v_f32_4)`. The scalar `sin`/`cos`/`tan` just delegate to
`std::sin`/`std::cos`/`std::tan` and don't use SIMD at all.

## What's needed

### Vectorized wrappers (v_f32_4)

Add to `v_f32_4/functions_trigonometry.h` and implement in `.cpp`:

- `v_f32_4 sin(const v_f32_4 &v) noexcept;` — wraps `sin_ps`
- `v_f32_4 cos(const v_f32_4 &v) noexcept;` — wraps `cos_ps`
- `void sincos(const v_f32_4 &v, v_f32_4 &outSin, v_f32_4 &outCos) noexcept;` — wraps `sincos_ps`
- `v_f32_4 sin3(const v_f32_4 &v) noexcept;` / `v_f32_4 cos3(...)` — 3-component variants (matching `log3`/`exp3`)

### Tests

Mirror the existing `computesLog`/`computesLogEdgeCases`/`computesExp`/`computesExpEdgeCases` pattern:

- Test against `std::sin`/`std::cos` using `StandardSample::transcendentalFloatConstants` with relative epsilon
- Test edge cases and document deviations from IEEE (sse_mathfun may deviate for inf, NaN, extreme inputs)
- Test critical angles: `0`, `pi/2`, `pi`, `3*pi/2`, `2*pi` and their negatives
- Test symmetry: `sin(-x) == -sin(x)`, `cos(-x) == cos(x)`
- Test identity: `sin(x)^2 + cos(x)^2 ~= 1` across the transcendental constants
- Test `sincos` consistency: outputs match individual `sin`/`cos` calls

### 256-bit variants (future)

When 256-bit float vector types are added, wrap `sin256_ps`, `cos256_ps`, `sincos256_ps` similarly.

## Notes

- The sse_mathfun implementations intentionally deviate from IEEE for some edge cases (similar to `log_ps`/`exp_ps`).
  Tests should document these deviations rather than assert IEEE compliance.
- Existing scalar `sin`/`cos`/`tan` in `functions/` should remain as-is (they serve different callers).
