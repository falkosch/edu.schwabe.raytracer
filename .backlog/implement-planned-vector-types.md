# Implement planned vector types declared in conversions/complex.h

## Motivation

`conversions/complex.h` contains forward declarations for ~15 additional vector types that are not yet implemented.
These represent the intended scope of the library and should be implemented to enable future use in the raytracer
and primitives layers.

## Planned types

### 8-bit integer vectors

- `v_ui8_16` -- 16x uint8, 128-bit
- `v_ui8_32` -- 32x uint8, 256-bit
- `v_i8_16` -- 16x int8, 128-bit
- `v_i8_32` -- 32x int8, 256-bit

### 16-bit integer vectors

- `v_ui16_8` -- 8x uint16, 128-bit
- `v_ui16_16` -- 16x uint16, 256-bit
- `v_i16_8` -- 8x int16, 128-bit
- `v_i16_16` -- 16x int16, 256-bit

### 256-bit integer vectors (32/64-bit lanes)

- `v_ui32_8` -- 8x uint32, 256-bit
- `v_i32_8` -- 8x int32, 256-bit
- `v_ui64_4` -- 4x uint64, 256-bit
- `v_i64_2` -- 2x int64, 128-bit
- `v_i64_4` -- 4x int64, 256-bit

### Integer matrices

- `m_ui32_4x4` -- 4x4 uint32 matrix
- `m_i32_4x4` -- 4x4 int32 matrix

### Double-precision float vectors

- `v_f64_2` -- 2x float64, 128-bit
- `v_f64_4` -- 4x float64, 256-bit

### 256-bit float vector

- `v_f32_8` -- 8x float32, 256-bit

## Approach

Each type needs the full set of type-level files following the `v_f32_4` pattern:

- `type.h` -- struct wrapping the intrinsic
- `api.h` -- public API
- `accessors.h`, `blends.h`, `constants.h`, `conversions.h`
- `functions.h`, `operators.h`, `selects.h`, `swizzles.h`, `swizzledBlends.h`

Plus corresponding tests mirroring the `v_f32_4` test structure.

## Suggested implementation order

1. `v_f64_2` / `v_f64_4` -- double-precision floats can reuse most of the 128d/256d infrastructure already in place
2. `v_f32_8` -- 256-bit float can reuse 256s infrastructure
3. `v_i32_8` / `v_ui32_8` -- 256-bit integers, unblocks the 256i operations already present
4. `v_i64_2` / `v_ui64_4` / `v_i64_4` -- 64-bit integer variants
5. 16-bit and 8-bit types -- most specialized, least urgent
6. Integer matrices -- only after the underlying integer vector types are solid
