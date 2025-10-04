#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_ui32_4 {
    static constexpr ASizeT SIZE = VectorSizes::W;
    using ValueType = UInt_32;
    using BoolType = ValueType;
    using VectorType = v_ui32_4;
    using VectorBoolType = VectorType;
    using PackedType = PackedTypes<ValueType, SIZE>::Type;

    PackedType components;

    v_ui32_4() noexcept;
    v_ui32_4(const PackedType &vector) noexcept;
    explicit v_ui32_4(ValueType scalar) noexcept;
    explicit v_ui32_4(ValueType x, ValueType y) noexcept;
    explicit v_ui32_4(ValueType x, ValueType y, ValueType z) noexcept;
    explicit v_ui32_4(ValueType x, ValueType y, ValueType z, ValueType w) noexcept;

    v_ui32_4 &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };
}
