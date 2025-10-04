#include "vectorization/v_ui64_2.h"

namespace vectorization {
  template <>
  v_ui64_2 blend<false, false>(const v_ui64_2 &a, const v_ui64_2 & /*unused*/) noexcept {
    return a;
  }

  template <>
  v_ui64_2 blend<false, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpacklo_epi64(a.components, _mm_unpackhi_epi64(b.components, b.components));
  }

  template <>
  v_ui64_2 blend<true, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpacklo_epi64(b.components, _mm_unpackhi_epi64(a.components, a.components));
  }

  template <>
  v_ui64_2 blend<true, true>(const v_ui64_2 & /*unused*/, const v_ui64_2 &b) noexcept {
    return b;
  }

  v_ui64_2 blendMasked(const v_ui64_2 &a, const v_ui64_2 &b, const v_ui64_2 &mask) noexcept {
    return _mm_blendv_epi8(a.components, b.components, mask.components);
  }
}
