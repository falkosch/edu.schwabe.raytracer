#pragma once

#include "type.h"
#include "vectorization/swizzled_blends.h"

namespace vectorization {
  // generic swizzled blend
  template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY>
  v_f64_2 swizzledBlend(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return swizzledBlend<X, Y, SelectX, SelectY>(a.components, b.components);
  }

  // Special case swizzled blend of two vectors, in which one component of
  // each vector is taken for blending after swizzling.
  template <ASizeT X, ASizeT Y>
  v_f64_2 swizzledBlend1x1(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    static_assert(X < v_f64_2::SIZE, "Index is out of range");
    static_assert(Y < v_f64_2::SIZE, "Index is out of range");
    return swizzledBlend1x1<X, Y>(a.components, b.components);
  }

  v_f64_2 x_x(const v_f64_2 &a, const v_f64_2 &b) noexcept;
  v_f64_2 x_y(const v_f64_2 &a, const v_f64_2 &b) noexcept;
  v_f64_2 y_x(const v_f64_2 &a, const v_f64_2 &b) noexcept;
  v_f64_2 y_y(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  template <ASizeT X, ASizeT Y>
  v_f64_2 swizzledBlendMasked(const v_f64_2 &a, const v_f64_2 &b, const v_f64_2 &mask) noexcept {
    return swizzledBlendMasked<X, Y>(a.components, b.components, mask.components);
  }

  template <ASizeT X, ASizeT Y>
  v_f64_2 swizzledBlendMasked(const v_f64_2 &a, const v_f64_2 &b, const v_f64_2::VectorBoolType &mask) noexcept {
    return swizzledBlendMasked<X, Y>(a.components, b.components, mask.components);
  }
}
