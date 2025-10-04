#pragma once

#include "vectorization/constants.h"

namespace vectorization {
  struct v_i32_4 {
    static constexpr ASizeT SIZE = VectorSizes::W;
    typedef Int_32 ValueType;
    typedef ValueType BoolType;
    typedef v_i32_4 VectorType;
    typedef VectorType VectorBoolType;
    typedef PackedTypes<ValueType, SIZE>::Type PackedType;

    PackedType components;

    v_i32_4() noexcept;
    v_i32_4(const PackedType &v) noexcept;
    explicit v_i32_4(ValueType s) noexcept;
    explicit v_i32_4(ValueType x, ValueType y) noexcept;
    explicit v_i32_4(ValueType x, ValueType y, ValueType z) noexcept;
    explicit v_i32_4(ValueType x, ValueType y, ValueType z, ValueType w) noexcept;

    v_i32_4 &operator=(const PackedType &v) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };
}
