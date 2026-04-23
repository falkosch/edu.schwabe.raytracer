#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_i32_4 {
    static constexpr ASizeT SIZE = VectorSizes::W;
    using ValueType = Int_32;
    using BoolType = ValueType;
    using VectorType = v_i32_4;
    using VectorBoolType = VectorType;
    using PackedType = PackedTypes<ValueType, SIZE>::Type;

    PackedType components;

    v_i32_4() noexcept;
    v_i32_4(const PackedType &vector) noexcept;
    explicit v_i32_4(ValueType scalar) noexcept;
    explicit v_i32_4(ValueType x, ValueType y) noexcept;
    explicit v_i32_4(ValueType x, ValueType y, ValueType z) noexcept;
    explicit v_i32_4(ValueType x, ValueType y, ValueType z, ValueType w) noexcept;
    explicit v_i32_4(const PackedType *vector) noexcept;
    explicit v_i32_4(const VectorType *vector) noexcept;

    /// @brief Constructs from a 16-byte aligned int array.
    explicit v_i32_4(const ValueType *values) noexcept;

    v_i32_4 &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };
}
