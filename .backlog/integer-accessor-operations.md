# Bring integer vector types to parity with float types

## Motivation

`v_f32_4` has full coverage across all operation categories (accessors, blends, swizzles, swizzledBlends, selects,
conversions, functions, trigonometry). The integer types (`v_i32_4`, `v_ui32_4`, `v_ui64_2`) are missing large
categories of operations, making them second-class citizens in the library.

## Gaps by category

### Accessors

No `component_128i.h` or `replaceComponent_128i.h`. Cannot get/set individual elements in integer vectors through
the abstraction layer.

- Add `component_128i.h` using `_mm_extract_epi32` / `_mm_extract_epi64` (SSE4.1)
- Add `replaceComponent_128i.h` using `_mm_insert_epi32` / `_mm_insert_epi64` (SSE4.1)
- Wire into `v_i32_4/accessors.h`, `v_ui32_4/accessors.h`, `v_ui64_2/accessors.h`

### Swizzles

No `swizzle_128i.h`. Component reordering only works on float types.

- Add `swizzle_128i.h` using `_mm_shuffle_epi32` (SSE2) for 32-bit lanes
- Wire into `v_i32_4/swizzles.h`, `v_ui32_4/swizzles.h`
- `v_ui64_2` may need separate handling (`_mm_shuffle_epi32` with 64-bit granularity or `_mm_unpacklo/hi_epi64`)

### Template blends

Only `blendMasked` has 128i variants; the compile-time `blend<>` templates are float-only.

- Add `blend_128i.h` using `_mm_blend_epi16` / `_mm_blend_epi32` (SSE4.1 / AVX2)
- Wire into `v_i32_4/blends.h`, `v_ui32_4/blends.h`, `v_ui64_2/blends.h`

### SwizzledBlends

Entirely absent for integer types.

- Add `swizzledBlend_128i.h` combining the new integer swizzle + blend primitives
- Wire into type-level headers

### Conversions

`v_i32_4` and `v_ui32_4` have limited conversion support compared to `v_f32_4`. `v_ui64_2` has minimal coverage.

- Review and fill gaps in `v_i32_4/conversions.h`, `v_ui32_4/conversions.h`, `v_ui64_2/conversions.h`

### v_ui64_2 specifically

Only 8 operational files vs. 11 for `v_f32_4`. The most limited of all vector types. Needs attention across all
categories above, plus any missing operators.
