# Close test coverage gaps across vectorization library

## Motivation

`v_f32_4` has 11 test files covering every operation category. The integer types, matrix types, and some operation
variants have zero test coverage. Untested code cannot be trusted during refactoring or when adding new SIMD widths.

## Coverage status

| Type / Category            | Test files | Status           |
|----------------------------|------------|------------------|
| `v_f32_4`                  | 11         | Complete         |
| `v_i32_4`                  | **0**      | Missing entirely |
| `v_ui32_4`                 | **0**      | Missing entirely |
| `v_ui64_2`                 | **0**      | Missing entirely |
| `m_f32_4x4`                | 1          | Minimal          |
| `v_f32_4x4`                | **0**      | Missing entirely |
| `swizzledBlendMasked_128s` | **0**      | Missing          |
| `swizzledBlendMasked_256s` | **0**      | Missing          |
| `selects_generic`          | **0**      | Missing          |
| `meta_bool_types`          | **0**      | Missing          |
| `meta_packed_types`        | **0**      | Missing          |

## What's needed

### Integer vector type tests (highest priority)

Mirror the `v_f32_4` test structure for each integer type:

- `v_i32_4`: type, accessors, blends, constants, operators, functions, swizzles, selects
- `v_ui32_4`: type, accessors, blends, constants, operators, functions, swizzles, selects
- `v_ui64_2`: type, accessors, blends, constants, operators, functions, swizzles

Note: some of these tests depend on the operations from the integer-accessor-operations backlog item being
implemented first. Tests for already-existing operations (operators, blendMasked, constants) can proceed now.

### Matrix type tests

- `m_f32_4x4`: expand beyond the single existing test file to cover accessors, constants, functions, operators
- `v_f32_4x4`: add tests for accessors, constants, functions, operators

### Missing operation variant tests

- `swizzledBlendMasked_128s_test.cpp`
- `swizzledBlendMasked_256s_test.cpp`
- `selects_generic_test.cpp`

### Architecture meta tests

- `meta_bool_types_test.cpp`
- `meta_packed_types_test.cpp`

## Test framework

- Windows/VS: MS CppUnitTest (`TEST_CLASS` / `TEST_METHOD` macros, `Assert::AreEqual`)
- Linux/GCC: Catch2
- File naming convention: `component_128d.cpp` -> `component_128d_test.cpp`
- Tests live in `tests/vectorization.native-test/src/vectorization/`
