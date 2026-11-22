# Unify vector type function interfaces

## Problem

The function sets across vector types are inconsistent. Each type has a different subset of
functions, with no clear contract for what a vector type must implement.

## Specification

### Base functions (all vector types)

Every vector type must implement:

- `min`, `minv`, `max`, `maxv`
- `horizontalSum`, `horizontalSumv`
- `dot`, `dotv`
- `length`, `lengthv` (integer types compute Manhattan distance, not Euclidean)
- `distance`, `distancev`
- `multiplyAdd`, `multiplySub`, `multiplyAddSub`, `multiplySubAdd`
- `negativeMultiplyAdd`, `negativeMultiplySub`
- `addSubtract`
- `sqr`
- `modulo`
- `clamp`
- `argmin`, `argmax`
- `anyTrue`, `anyFalse`, `allTrue`, `allFalse`

### Float vector types (additional)

- `rlength`, `rlengthv`, `normalize`
- `reflect`, `refractEta`, `refract`
- `mix`
- `isNegative`, `isNaN`
- `sqrt`, `reciprocal`, `rsqrt`
- `ceil`, `floor`, `round`, `fract`
- `abs`

### Signed integer vector types (additional)

- `isNegative`
- `abs`

### 4D vector types (additional)

- `oneW`, `zeroW`
- 3-component variants: `min3`, `min3v`, `max3`, `max3v`, `horizontalSum3`, `horizontalSum3v`,
  `dot3`, `dot3v`, `length3`, `length3v`, `distance3`, `distance3v`, `argmin3`, `argmax3`,
  `anyTrue3`, `anyFalse3`, `allTrue3`, `allFalse3`
- Float 4D additionally: `rlength3`, `rlength3v`, `normalize3`, `cross3`, `mapOrthogonal`,
  `mapSpherical`

## Known inconsistencies

- `isNegative` of a `v_i32_4` returns its 4 lane bits repeated in all four nibbles (16 bits, from `_mm_packs_epi32`,
  `_mm_packs_epi16` and `_mm_movemask_epi8`), while `v_f32_4`, `v_f32_8`, `v_f64_2`, `v_f64_4` and `v_i32_8` return
  one bit per lane. Callers that mask the result with their lanes don't notice; a caller that compares or shifts the
  raw mask does. Found 2026-11-21 by `RayCastTest::detectsOutOfReachPerLane`, which masks it.

## Scope

- Audit each mid-tier type against these lists
- Add missing functions
- Add tests for all new functions
- Ensure consistent parameter naming and signature style across types

### Resolution

All twelve mid-tier vector types declare every function of their lists; a script over the `functions.h` headers and
the `functions_test.cpp` files found no gaps (`v_f32_4`'s multiply-add family is tested in `fma_functions_test.cpp`).
Every added function has tests, including ties, `w` components that must not leak into `3` variants, unsigned values
above the signed range and negative `modulo` operands. The function set and its semantics are now documented in
`sources/vectorization/CLAUDE.md` ("Function set of the vector types").

Semantics settled across the types:

- `v` variants broadcast to all lanes.
- Integer `length`/`distance` are Manhattan lengths; unsigned `distance` subtracts the smaller from the larger
  component.
- The integer multiply-add family and `dot` wrap around on overflow; alternating operations keep the float lane
  convention (even lanes subtract in `addSubtract` and `multiplyAddSub`).
- Integer `modulo` truncates like `%`, float `modulo` floors, as in the basic tier.
- `argmin`/`argmax` return the highest index among equal extrema, like the existing `v_f32_4::argmin3`. A new
  basic-tier helper `highestLane()` turns the lane mask into that index (0 when no bit is set, e.g. for NaN).
- `anyTrue` and friends are bitwise on integer types and test the sign bit on float types.
- `refract` on `v_f32_8` and `v_f64_2` takes `eta` like `v_f32_4`: entering ratio first, leaving ratio second.
- Parameters are named `v`, `a`, `b`, `c`, `lower`, `upper` everywhere; each file keeps its layout (`v_i64_4` and
  `v_ui64_4` stay in the 2-space style).

Found and fixed on the way:

- `isNegative(v_i32_4)` returns one bit per lane (`_mm_movemask_ps`), the known inconsistency above.
- `v_f32_4::minv`/`maxv` filled only lane 0 with the result, `v_i64_2`/`v_ui64_2` only lane 0 (their test asserted
  the wrong `y`).
- `v_i64_2::length` summed the components without `abs` (its test asserted 5 for `{10, -5}`).
- `distance` of `v_ui32_4` and `v_ui64_2` wrapped around where a component of `b` exceeded `a`.
- `v_ui64_2::operator>` compared signed, so `min`, `max` and `clamp` failed above `INT64_MAX`.
- `v_i64_2::operator*` multiplied signed scalars (undefined on overflow); it multiplies unsigned now.
- The 256-bit integer types tested `allTrue`/`anyFalse` per lane ("no lane is zero"), the 128-bit ones per bit; all
  test bits now (`_mm256_testc_si256`).

`v_i64_4` and `v_ui64_4` got `operator*`, which `sqr`, `dot` and the multiply-add family need. Integer operations stay
in the mid tier with direct intrinsics: `__m128i`/`__m256i` don't carry the element type that basic-tier overloads
would need. 64-bit `min`, `max`, `abs` and multiplication are emulated without AVX-512 intrinsics.

Verified with the `debug` and `release` presets (MSVC, AVX2), with `clang-release` and with an MSVC `/arch:SSE4.2` build
that compiles the 128-bit fallback halves of the 256-bit types: all targets build warning-free (SSE4.2: only C4324 in
the untouched `KDTreeWideNode.h`), and the vectorization (2008), primitives (68) and raytracing (403) tests pass in all
four. `RayCastTest::detectsOutOfReachPerLane` no longer masks the `isNegative` nibbles.

## Commits

- `9fca255` fix(vectorization): compare v_ui64_2 unsigned and let v_i64_2 products wrap
- `078a251` feat(vectorization): give every vector type the same function set
