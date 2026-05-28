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

## Scope

- Audit each mid-tier type against these lists
- Add missing functions
- Add tests for all new functions
- Ensure consistent parameter naming and signature style across types
