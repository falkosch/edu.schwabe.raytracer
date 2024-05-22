#include "vectorization/v_ui64_2.h"

namespace vectorization {
  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::X, VectorIndices::X, false, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpacklo_epi64(a.components, b.components);
  }

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::X, VectorIndices::X, true, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpacklo_epi64(b.components, a.components);
  }

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::Y, VectorIndices::Y, false, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpackhi_epi64(a.components, b.components);
  }

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::Y, VectorIndices::Y, true, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpackhi_epi64(b.components, a.components);
  }

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpacklo_epi64(a.components, b.components);
  }

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpacklo_epi64(a.components, _mm_unpackhi_epi64(b.components, b.components));
  }

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpackhi_epi64(a.components, _mm_unpacklo_epi64(b.components, b.components));
  }

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_unpackhi_epi64(a.components, b.components);
  }

  v_ui64_2 x_x(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(a, b);
  }

  v_ui64_2 x_y(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(a, b);
  }

  v_ui64_2 y_x(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(a, b);
  }

  v_ui64_2 y_y(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  template <>
  inline v_ui64_2 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(
      const v_ui64_2 &a, const v_ui64_2 &b, const v_ui64_2 &mask
  ) noexcept {
    return blendMasked(a, b, mask);
  }
}
