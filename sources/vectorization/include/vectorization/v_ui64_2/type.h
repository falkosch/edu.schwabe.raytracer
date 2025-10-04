#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_ui64_2 {
    static constexpr ASizeT SIZE = VectorSizes::Y;
    using ValueType = UInt_64;
    using BoolType = ValueType;
    using VectorType = v_ui64_2;
    using VectorBoolType = VectorType;
    using PackedType = PackedTypes<ValueType, SIZE>::Type;

    PackedType components;

    v_ui64_2() noexcept;
    v_ui64_2(const PackedType &vector) noexcept;
    explicit v_ui64_2(ValueType scalar) noexcept;
    explicit v_ui64_2(ValueType x, ValueType y) noexcept;

    v_ui64_2 &operator=(const PackedType &packed) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };
}
