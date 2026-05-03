#pragma once

#include "vectorization/v_ui64_2/type.h"

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_f64_2 {
    static constexpr ASizeT SIZE = VectorSizes::Y;
    using ValueType = Float_64;
    using BoolType = v_ui64_2::ValueType;
    using VectorType = v_f64_2;
    using VectorBoolType = v_ui64_2;
    using PackedType = PackedFloat2_128;

    PackedType components;

    v_f64_2() noexcept;
    v_f64_2(const PackedType &vector) noexcept;
    explicit v_f64_2(ValueType scalar) noexcept;
    explicit v_f64_2(ValueType x, ValueType y) noexcept;
    explicit v_f64_2(const PackedType *vector) noexcept;
    explicit v_f64_2(const VectorType *vector) noexcept;

    /// @brief Constructs from a 16-byte aligned double array.
    explicit v_f64_2(const ValueType *values) noexcept;

    VectorType &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };

  void store(const v_f64_2 &src, v_f64_2 *dst) noexcept;

  void store(const v_f64_2 &src, v_f64_2::PackedType *dst) noexcept;

  void store(const v_f64_2 &src, v_f64_2::ValueType *dst) noexcept;
}
