#pragma once

#include "type.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_256i.h"
#endif

namespace vectorization {

  template <bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4,
            bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  v_ui32_8 blend(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blend<SelectX1, SelectX2, SelectX3, SelectX4, SelectX5, SelectX6, SelectX7, SelectX8>(
        a.components, b.components
    );
#else
    return {{_mm_blend_epi16(a.components.lo, b.components.lo, BLEND_MASK_INT4_32(SelectX1, SelectX2, SelectX3, SelectX4)),
             _mm_blend_epi16(a.components.hi, b.components.hi, BLEND_MASK_INT4_32(SelectX5, SelectX6, SelectX7, SelectX8))}};
#endif
  }

  template <>
  v_ui32_8 blend<false, false, false, false, false, false, false, false>(const v_ui32_8 &a, const v_ui32_8 &) noexcept;

  template <>
  v_ui32_8 blend<true, true, true, true, true, true, true, true>(const v_ui32_8 &, const v_ui32_8 &b) noexcept;

  v_ui32_8 blendMasked(const v_ui32_8 &a, const v_ui32_8 &b, const v_ui32_8 &mask) noexcept;
}
