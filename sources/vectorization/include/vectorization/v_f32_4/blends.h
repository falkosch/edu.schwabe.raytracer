#pragma once

#include "type.h"

#include "vectorization/blends.h"

namespace vectorization {
  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  // generic blend of two vectors
  inline const v_f32_4 blend(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return blend<SelectX, SelectY, SelectZ, SelectW>(a.components, b.components);
  }

  const v_f32_4 x_yzw(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 blendMasked(const v_f32_4 &onBitNotSet, const v_f32_4 &onBitSet, const v_f32_4 &mask) noexcept;

  const v_f32_4
  blendMasked(const v_f32_4 &onBitNotSet, const v_f32_4 &onBitSet, const v_f32_4::VectorBoolType &mask) noexcept;
}
