#pragma once

#include "type.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_256s.h"
#endif

namespace vectorization {

  template <bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4,
            bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  v_f32_8 blend(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blend<SelectX1, SelectX2, SelectX3, SelectX4, SelectX5, SelectX6, SelectX7, SelectX8>(
        a.components, b.components
    );
#else
    return {{_mm_blend_ps(a.components.lo, b.components.lo, (SelectX1 & 1) | ((SelectX2 & 1) << 1) | ((SelectX3 & 1) << 2) | ((SelectX4 & 1) << 3)),
             _mm_blend_ps(a.components.hi, b.components.hi, (SelectX5 & 1) | ((SelectX6 & 1) << 1) | ((SelectX7 & 1) << 2) | ((SelectX8 & 1) << 3))}};
#endif
  }

  template <>
  v_f32_8 blend<false, false, false, false, false, false, false, false>(const v_f32_8 &a, const v_f32_8 &) noexcept;

  template <>
  v_f32_8 blend<true, true, true, true, true, true, true, true>(const v_f32_8 &, const v_f32_8 &b) noexcept;

  v_f32_8 blendMasked(const v_f32_8 &onBitNotSet, const v_f32_8 &onBitSet, const v_f32_8 &mask) noexcept;

  v_f32_8
  blendMasked(const v_f32_8 &onBitNotSet, const v_f32_8 &onBitSet, const v_f32_8::VectorBoolType &mask) noexcept;
}
