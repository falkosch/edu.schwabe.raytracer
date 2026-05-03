#pragma once

#include "type.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y>
  // generic swizzle
  v_f64_2 swizzle(const v_f64_2 &v) noexcept {
    return swizzle<X, Y>(v.components);
  }

  v_f64_2 xx(const v_f64_2 &v) noexcept;

  v_f64_2 yx(const v_f64_2 &v) noexcept;

  v_f64_2 yy(const v_f64_2 &v) noexcept;
}
