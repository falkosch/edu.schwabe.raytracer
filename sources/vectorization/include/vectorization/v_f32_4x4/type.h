#pragma once

#include "api.h"

#include "vectorization/v_f32_4.h"

#include "vectorization/architecture.h"

namespace vectorization {
  // Structure of arrays of four v_f32_4 like this:
  // this->soaX = x1, x2, x3, x4
  // this->soaY = y1, y2, y3, y4
  // this->soaZ = z1, z2, z3, z4
  // this->soaW = w1, w2, w3, w4
  struct v_f32_4x4 {
    using SOAVectorType = v_f32_4;
    using AOSVectorType = v_f32_4;
    using PackedType = SOAVectorType::PackedType;
    using ValueType = SOAVectorType::ValueType;
    using BoolType = BoolTypes<ValueType>::Type;

    static constexpr ASizeT SIZE_SOA = SOAVectorType::SIZE;
    static constexpr ASizeT SIZE_AOS = AOSVectorType::SIZE;
    static constexpr ASizeT SIZE = SIZE_SOA * SIZE_AOS;

    SOAVectorType soaX, soaY, soaZ, soaW;

    v_f32_4x4() noexcept;
    explicit v_f32_4x4(
        const SOAVectorType::PackedType &soaXIn, const SOAVectorType::PackedType &soaYIn,
        const SOAVectorType::PackedType &soaZIn, const SOAVectorType::PackedType &soaWIn
    ) noexcept;
    explicit v_f32_4x4(
        const SOAVectorType &soaXIn, const SOAVectorType &soaYIn, const SOAVectorType &soaZIn,
        const SOAVectorType &soaWIn
    ) noexcept;
    // broadcasting aos vector into its soa vectors
    explicit v_f32_4x4(const AOSVectorType &aos) noexcept;
    explicit v_f32_4x4(const ValueType *values) noexcept;
  };
}
