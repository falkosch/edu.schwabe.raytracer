#pragma once

#include "vectorization/v_i32_4/type.h"

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_f32_4 {
    static constexpr ASizeT SIZE = VectorSizes::W;
    using ValueType = Float_32;
    using BoolType = v_i32_4::ValueType;
    using VectorType = v_f32_4;
    using VectorBoolType = v_i32_4;
    using PackedType = PackedTypes<ValueType, SIZE>::Type;

    PackedType components;

    v_f32_4() noexcept;
    v_f32_4(const PackedType &vector) noexcept;
    explicit v_f32_4(ValueType scalar) noexcept;
    explicit v_f32_4(ValueType x, ValueType y) noexcept;
    explicit v_f32_4(ValueType x, ValueType y, ValueType z) noexcept;
    explicit v_f32_4(ValueType x, ValueType y, ValueType z, ValueType w) noexcept;
    explicit v_f32_4(const PackedType *vector) noexcept;
    explicit v_f32_4(const VectorType *vector) noexcept;

    /// @brief Constructs from a 16-byte aligned float array.
    explicit v_f32_4(const ValueType *values) noexcept;

    VectorType &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };

  void store(const v_f32_4 &src, v_f32_4 *dst) noexcept;

  void store(const v_f32_4 &src, v_f32_4::PackedType *dst) noexcept;

  void store(const v_f32_4 &src, v_f32_4::ValueType *dst) noexcept;
}
