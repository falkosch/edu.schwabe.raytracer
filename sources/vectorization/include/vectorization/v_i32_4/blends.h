#pragma once

#include "type.h"

namespace vectorization {

  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  // generic blend of two vectors
  v_i32_4 blend(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_blend_epi16(a.components, b.components, BLEND_MASK_INT4_32(SelectX, SelectY, SelectZ, SelectW));
  }

  template <>
  // pass through blend
  v_i32_4 blend<false, false, false, false>(const v_i32_4 &a, const v_i32_4 &) noexcept;

  template <>
  // pass through blend other
  v_i32_4 blend<true, true, true, true>(const v_i32_4 &, const v_i32_4 &b) noexcept;

  v_i32_4 blendMasked(const v_i32_4 &a, const v_i32_4 &b, const v_i32_4 &mask) noexcept;
}
