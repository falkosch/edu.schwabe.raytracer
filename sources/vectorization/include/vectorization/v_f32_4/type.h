#pragma once

#include "api.h"

#include "vectorization/v_i32_4.h"

#include "vectorization/architecture.h"

namespace vectorization {
  struct v_f32_4 {
    // Metas
    static constexpr ASizeT SIZE = VectorSizes::W;
    typedef Float_32 ValueType;
    typedef v_i32_4::ValueType BoolType;
    typedef v_f32_4 VectorType;
    typedef v_i32_4 VectorBoolType;
    typedef PackedTypes<ValueType, SIZE>::Type PackedType;

    // the actual data
    PackedType components;

    v_f32_4() noexcept;

    v_f32_4(const PackedType &v) noexcept;

    explicit v_f32_4(const ValueType v) noexcept;

    explicit v_f32_4(const ValueType x, const ValueType y) noexcept;

    explicit v_f32_4(const ValueType x, const ValueType y, const ValueType z) noexcept;

    explicit v_f32_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w) noexcept;

    explicit v_f32_4(const PackedType *const v) noexcept;

    explicit v_f32_4(const VectorType *const v) noexcept;

    explicit v_f32_4(const ValueType *const v) noexcept;

    VectorType &operator=(const PackedType &v) noexcept;

    const ValueType &operator[](const ASizeT index) const noexcept;
  };

  void store(const v_f32_4 &src, v_f32_4 *const dst) noexcept;

  void store(const v_f32_4 &src, v_f32_4::PackedType *const dst) noexcept;

  void store(const v_f32_4 &src, v_f32_4::ValueType *const dst) noexcept;
}
