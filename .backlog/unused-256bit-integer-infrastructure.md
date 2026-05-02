# Complete 256-bit integer SIMD support with wrapper types

## Motivation

`blendMasked_256i.h` and `selects_256i.h` already provide operations on `PackedInts_256`, but not all wrapper types
exist to consume them through the type-safe API.

## Implemented

### Width-specific operations (basic tier)

All 256i basic-tier infrastructure is now in place with AVX2-native + AVX-fallback (float-domain cast) paths:

- `component_256i.h` / `.cpp` — 8-lane int32 extract via swizzle + `_mm256_cvtsi256_si32`
- `replace_component_256i.h` / `.cpp` — 8-lane int32 insert via `blend_256i`
- `blend_256i.h` / `.cpp` — `_mm256_blend_epi32` (AVX2), `_mm256_blend_ps` cast fallback (AVX)
- `blend_masked_256i.h` / `.cpp` — `_mm256_blendv_epi8` (pre-existing)
- `selects_256i.h` / `.cpp` — scalar select (pre-existing)
- `swizzle_256i.h` / `.cpp` — `_mm256_shuffle_epi32` (AVX2), `_mm256_permute_ps` cast fallback (AVX);
  8-lane swizzle via `_mm256_permute2x128_si256` + blend
- `swizzled_blend_256i.h` — template-only, combines swizzle + blend
- `swizzled_blend_masked_256i.h` — template-only, combines swizzle + blendMasked

### Wrapper types

- `v_i32_8` — 8x int32, three-tier: AVX2 native / AVX decompose / SSE 2×128-bit. Full file set: type, api,
  accessors, blends, constants, functions, operators, swizzles, swizzled_blends.

## Still needed

### Wrapper types

- `v_ui32_8` — 8x uint32, 256-bit, wrapping `PackedInts_256`. Same three-tier pattern as `v_i32_8`.

### Tests

Full test suites for `v_i32_8` and `v_ui32_8` mirroring the `v_i32_4` test structure.

## Relation to other backlog items

- Overlaps with `implement-planned-vector-types.md` (`v_i32_8` / `v_ui32_8` are listed there too)
