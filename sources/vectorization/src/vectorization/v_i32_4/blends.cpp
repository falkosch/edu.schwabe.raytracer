#include "vectorization/v_i32_4.h"

namespace vectorization {
  template <>
  v_i32_4 blend<false, false, false, false>(const v_i32_4 &a, const v_i32_4 &) noexcept {
    return a;
  }

  template <>
  v_i32_4 blend<true, true, true, true>(const v_i32_4 &, const v_i32_4 &b) noexcept {
    return b;
  }

  v_i32_4 blendMasked(const v_i32_4 &a, const v_i32_4 &b, const v_i32_4 &mask) noexcept {
    return _mm_blendv_epi8(a.components, b.components, mask.components);
  }
}
