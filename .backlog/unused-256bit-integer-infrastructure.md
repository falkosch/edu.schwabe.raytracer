# Complete 256-bit integer SIMD support with wrapper types

## Motivation

`blendMasked_256i.h` and `selects_256i.h` already provide operations on `PackedInts_256`, but no wrapper types
(`v_i32_8`, `v_ui32_8`, etc.) exist to consume them through the type-safe API. The infrastructure is in place but
unreachable. Completing the wrapper types makes this investment usable.

## Already implemented (256i operations)

- `sources/vectorization/include/vectorization/blends/blendMasked_256i.h`
- `sources/vectorization/src/vectorization/blends/blendMasked_256i.cpp`
- `sources/vectorization/include/vectorization/selects/selects_256i.h`
- `sources/vectorization/src/vectorization/selects/selects_256i.cpp`

## Still needed

### Wrapper types

- `v_i32_8` -- 8x int32, 256-bit, wrapping `PackedInts_256`
- `v_ui32_8` -- 8x uint32, 256-bit, wrapping `PackedInts_256`

Each type needs the full file set following the `v_f32_4` / `v_i32_4` pattern:

- `type.h`, `api.h`
- `accessors.h` (requires `component_256i.h` / `replaceComponent_256i.h`)
- `blends.h` (wire to existing `blendMasked_256i`, add `blend_256i` template)
- `constants.h`, `conversions.h`
- `functions.h`, `operators.h`
- `selects.h` (wire to existing `selects_256i`)
- `swizzles.h` (requires `swizzle_256i.h`)
- `swizzledBlends.h` (requires `swizzledBlend_256i.h`)

### Width-specific operations not yet present

- `component_256i.h` -- `_mm256_extract_epi32` / `_mm256_extract_epi64` (AVX2)
- `replaceComponent_256i.h` -- `_mm256_insert_epi32` / `_mm256_insert_epi64` (AVX2)
- `blend_256i.h` -- `_mm256_blend_epi32` (AVX2)
- `swizzle_256i.h` -- `_mm256_shuffle_epi32`, `_mm256_permute4x64_epi64` (AVX2)
- `swizzledBlend_256i.h` -- combining the above

### Tests

Full test suite for `v_i32_8` and `v_ui32_8` mirroring the `v_f32_4` test structure.

## Relation to other backlog items

- Depends on patterns established in the integer-accessor-operations item (128i accessors/swizzles)
- Overlaps with the planned-types item (`v_i32_8` / `v_ui32_8` are listed there too); this item focuses
  specifically on making the existing 256i infrastructure reachable
