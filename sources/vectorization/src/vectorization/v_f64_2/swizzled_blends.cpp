#include "vectorization/v_f64_2/swizzled_blends.h"

namespace vectorization {
  v_f64_2 x_x(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(a, b);
  }

  v_f64_2 x_y(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(a, b);
  }

  v_f64_2 y_x(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(a, b);
  }

  v_f64_2 y_y(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(a, b);
  }
}
