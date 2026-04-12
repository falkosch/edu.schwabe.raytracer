# Introduce FMA intrinsic wrappers as generic functions

## Motivation

The library has zero explicit FMA usage despite targeting AVX2+FMA. The `a*b - c*d` pattern in `cross3`
(and similar multiply-add/subtract patterns elsewhere) benefits from FMA both for performance (single
fused op vs. separate mul+sub) and numerical precision (no intermediate rounding, reducing catastrophic
cancellation).

Currently, adding FMA would require `#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2` branches
at every call site, which is noisy and error-prone.

## Proposal

Introduce generic FMA wrapper functions (e.g. `fmadd`, `fmsub`, `fnmadd`, `fnmsub`) that:

- Use `_mm_fmadd_ps` / `_mm_fmsub_ps` etc. when `VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2`
- Fall back to `a * b + c` / `a * b - c` otherwise
- Provide overloads for all packed float types (128s, 128d, 256s, 256d) and scalars
- Keep the `#if` guard in one place (the wrapper), not at every call site

## Primary candidate

`cross3` in `v_f32_4/functions.cpp:352-354` -- hot path in ray-triangle intersection:

```cpp
// current
return yzxw(a) * zxyw(b) - zxyw(a) * yzxw(b);

// with wrapper
return fmsub(yzxw(a), zxyw(b), zxyw(a) * yzxw(b));
```

## Other candidates

- `dot` / `dotv` -- `a*b + c` accumulation patterns
- `rotate()` matrix construction in `m_f32_4x4/functions.cpp`
- Any `a * b + c` or `a * b - c` pattern in the shading/BRDF code

## Note: `matrix * vector` uses `_mm_dp_ps` intentionally (#14)

Broadcast-multiply-add computes `vector * matrix`, not `matrix * vector`. The `_mm_dp_ps` approach
is correct for row-dot-product semantics and must stay.
