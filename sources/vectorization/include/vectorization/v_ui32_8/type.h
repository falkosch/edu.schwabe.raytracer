#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_ui32_8 {
    static constexpr ASizeT SIZE = VectorSizes::X8;
    static constexpr ASizeT HALF_SIZE = VectorSizes::W;
    using ValueType = UInt_32;
    using BoolType = ValueType;
    using VectorType = v_ui32_8;
    using VectorBoolType = VectorType;
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

    v_ui32_8() noexcept;
    v_ui32_8(const PackedType &vector) noexcept;
    explicit v_ui32_8(ValueType scalar) noexcept;
    explicit v_ui32_8(ValueType x1, ValueType x2, ValueType x3, ValueType x4) noexcept;
    explicit v_ui32_8(
        ValueType x1, ValueType x2, ValueType x3, ValueType x4,
        ValueType x5, ValueType x6, ValueType x7, ValueType x8
    ) noexcept;
    explicit v_ui32_8(const ValueType *values) noexcept;

    v_ui32_8 &operator=(const PackedType &vector) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };

  void store(const v_ui32_8 &src, v_ui32_8 *dst) noexcept;

  void store(const v_ui32_8 &src, v_ui32_8::PackedType *dst) noexcept;

  void store(const v_ui32_8 &src, v_ui32_8::ValueType *dst) noexcept;
}
