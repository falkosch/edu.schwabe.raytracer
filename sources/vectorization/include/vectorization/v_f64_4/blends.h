#pragma once

#include "type.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_256d.h"
#endif

namespace vectorization {

  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  v_f64_4 blend(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blend<SelectX, SelectY, SelectZ, SelectW>(a.components, b.components);
#else
    return {{_mm_blend_pd(a.components.lo, b.components.lo, (SelectX & 1) | ((SelectY & 1) << 1)),
             _mm_blend_pd(a.components.hi, b.components.hi, (SelectZ & 1) | ((SelectW & 1) << 1))}};
#endif
  }

  template <>
  v_f64_4 blend<false, false, false, false>(const v_f64_4 &a, const v_f64_4 &) noexcept;

  template <>
  v_f64_4 blend<true, true, true, true>(const v_f64_4 &, const v_f64_4 &b) noexcept;

  v_f64_4 blendMasked(const v_f64_4 &onBitNotSet, const v_f64_4 &onBitSet, const v_f64_4 &mask) noexcept;

  v_f64_4
  blendMasked(const v_f64_4 &onBitNotSet, const v_f64_4 &onBitSet, const v_f64_4::VectorBoolType &mask) noexcept;
}
