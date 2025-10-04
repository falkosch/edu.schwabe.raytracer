#pragma once

#include "type.h"

#include "vectorization/blends.h"

namespace vectorization {
  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  // generic blend of two vectors
  v_ui32_4 blend(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_blend_epi16(a.components, b.components, BLEND_MASK_INT4_32(SelectX, SelectY, SelectZ, SelectW));
  }

  template <>
  // pass through blend
  v_ui32_4 blend<false, false, false, false>(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  template <>
  // pass through blend other
  v_ui32_4 blend<true, true, true, true>(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;
}
