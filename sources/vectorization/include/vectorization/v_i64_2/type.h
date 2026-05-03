#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_i64_2 {
    static constexpr ASizeT SIZE = VectorSizes::Y;
    using ValueType = Int_64;
    using BoolType = ValueType;
    using VectorType = v_i64_2;
    using VectorBoolType = VectorType;
    using PackedType = PackedTypes<ValueType, SIZE>::Type;

    PackedType components;

    v_i64_2() noexcept;
    v_i64_2(const PackedType &vector) noexcept;
    explicit v_i64_2(ValueType scalar) noexcept;
    explicit v_i64_2(ValueType x, ValueType y) noexcept;
    explicit v_i64_2(const PackedType *vector) noexcept;
    explicit v_i64_2(const VectorType *vector) noexcept;

    /// @brief Constructs from a 16-byte aligned int64 array.
    explicit v_i64_2(const ValueType *values) noexcept;

    v_i64_2 &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };
}
