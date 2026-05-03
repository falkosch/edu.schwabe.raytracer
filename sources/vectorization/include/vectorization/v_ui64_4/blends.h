#pragma once

#include "type.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_256i.h"
#endif

namespace vectorization {

  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  v_ui64_4 blend(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blend<SelectX, SelectX, SelectY, SelectY, SelectZ, SelectZ, SelectW, SelectW>(
        a.components, b.components
    );
#else
    return {{_mm_blend_epi16(a.components.lo, b.components.lo, (SelectX ? 0x0F : 0) | (SelectY ? 0xF0 : 0)),
             _mm_blend_epi16(a.components.hi, b.components.hi, (SelectZ ? 0x0F : 0) | (SelectW ? 0xF0 : 0))}};
#endif
  }

  template <>
  v_ui64_4 blend<false, false, false, false>(const v_ui64_4 &a, const v_ui64_4 &) noexcept;

  template <>
  v_ui64_4 blend<true, true, true, true>(const v_ui64_4 &, const v_ui64_4 &b) noexcept;

  v_ui64_4 blendMasked(const v_ui64_4 &a, const v_ui64_4 &b, const v_ui64_4 &mask) noexcept;
}
