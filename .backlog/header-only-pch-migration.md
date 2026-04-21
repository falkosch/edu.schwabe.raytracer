# Header-only migration with precompiled headers

## Motivation

Move vectorization function bodies from `.cpp` into `.h` headers (as `inline`) to enable
cross-library inlining without LTCG. Use precompiled headers (PCH) on consuming targets
to avoid compilation time explosion.

## Completed preparation

- Narrowed all `.cpp` umbrella includes to specific sub-headers (breaks circular dependencies)
- Fixed template specialization ordering: packed before scalar in constant value files
- Fixed constant scalar specializations to use static variables where available (pi pattern)
- Verified no circular include dependencies remain in `.cpp` files

## Remaining steps

### 1. Extract `x()` into a minimal header

Create `accessors/x_element.h` containing only `component<X>` and `x()` for
`PackedFloat4_128` and `PackedFloat2_128`. Depends only on `architecture.h`.

This allows `blend_masked_scalar.h` and constant value headers to include `x_element.h`
directly when their bodies become inline, avoiding the swizzle -> blend cycle.

### 2. Prototype on a single file

Pick a simple file (e.g. `architecture/memory.h`), do the full migration, verify it
compiles in isolation and as part of the full build before proceeding.

### 3. Migrate safe files (no cross-category dependencies)

- `architecture/` (memory, compiler_intrinsics)
- `blends/` (all — primary templates already in headers for blend_128s/d, blend_256s/d)
- `swizzles/` (all)
- `swizzled_blends/` (all)
- `constants/masks/` (inter-mask deps only)
- `constants/values/` without accessor dependency (zero, one, two, etc.)
- `v_f32_4x4/`, `m_f32_4x4/` (all)

### 4. Migrate files with accessor dependency

After `x_element.h` exists:

- `blend_masked_scalar.h`, constant value headers, `mask_all.h`, `mask_none.h`

### 5. Migrate files with cross-function dependencies

Leaf functions first, then composites:
`floor` -> `fract`, `divide + floor` -> `modulo` -> `modpad`,
`reciprocal + sqrt` -> `rsqrt`, `copysign` -> `sign`

### 6. Migrate higher-level type files

`v_f32_4/`, `v_i32_4/`, `v_ui32_4/`, `v_ui64_2/`, `conversions/`

### 7. Set up PCH

```cmake
target_precompile_headers(primitives PRIVATE <vectorization.h>)
target_precompile_headers(raytracing REUSE_FROM primitives)
target_precompile_headers(raytracerui REUSE_FROM primitives)
```

### 8. Clean up

Remove unity builds. Consider removing empty `.cpp` stubs from SOURCES.

### 9. Verify

- Build all targets including tests
- Run test suite
- Compare raytrace output against reference (Cornell box teapot)
- Benchmark runtime performance vs. pre-migration baseline
