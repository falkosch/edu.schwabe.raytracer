#include "vectorization/v_ui64_2.h"

namespace vectorization {
  template <>
  v_ui64_2 swizzle<VectorIndices::X, VectorIndices::X>(const v_ui64_2 &vector) noexcept {
    return _mm_unpacklo_epi64(vector.components, vector.components);
  }

  template <>
  v_ui64_2 swizzle<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 &vector) noexcept {
    return vector;
  }

  template <>
  v_ui64_2 swizzle<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 &vector) noexcept {
    return _mm_unpacklo_epi64(_mm_unpackhi_epi64(vector.components, vector.components), vector.components);
  }

  template <>
  v_ui64_2 swizzle<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 &vector) noexcept {
    return _mm_unpackhi_epi64(vector.components, vector.components);
  }

  v_ui64_2 xx(const v_ui64_2 &vector) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X>(vector);
  }

  v_ui64_2 xy(const v_ui64_2 &vector) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y>(vector);
  }

  v_ui64_2 yx(const v_ui64_2 &vector) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X>(vector);
  }

  v_ui64_2 yy(const v_ui64_2 &vector) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y>(vector);
  }
}
