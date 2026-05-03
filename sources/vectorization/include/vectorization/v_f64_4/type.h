#pragma once

#include "vectorization/v_i64_4/type.h"

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_f64_4 {
    static constexpr ASizeT SIZE = VectorSizes::W;
    static constexpr ASizeT HALF_SIZE = VectorSizes::Y;
    using ValueType = Float_64;
    using BoolType = v_i64_4::ValueType;
    using VectorType = v_f64_4;
    using VectorBoolType = v_i64_4;
    using HalfPackedType = PackedTypes<ValueType, HALF_SIZE>::Type;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    using PackedType = PackedTypes<ValueType, SIZE>::Type;
#else
    struct PackedType {
      HalfPackedType lo;
      HalfPackedType hi;
    };
#endif

    PackedType components;

    v_f64_4() noexcept;
    v_f64_4(const PackedType &vector) noexcept;
    explicit v_f64_4(ValueType scalar) noexcept;
    explicit v_f64_4(ValueType x, ValueType y, ValueType z, ValueType w) noexcept;
    explicit v_f64_4(const ValueType *values) noexcept;

    VectorType &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };

  void store(const v_f64_4 &src, v_f64_4 *dst) noexcept;

  void store(const v_f64_4 &src, v_f64_4::PackedType *dst) noexcept;

  void store(const v_f64_4 &src, v_f64_4::ValueType *dst) noexcept;
}
