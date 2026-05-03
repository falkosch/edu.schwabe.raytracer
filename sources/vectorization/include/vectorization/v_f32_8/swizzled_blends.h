#pragma once

#include "blends.h"
#include "swizzles.h"

namespace vectorization {
  // 4-param swizzle applies independently to each 128-bit half of b, then blends across all 8 lanes.
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W,
            bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4,
            bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  v_f32_8 swizzledBlend(const v_f32_8 &a, const v_f32_8 &b) noexcept {
    return blend<SelectX1, SelectX2, SelectX3, SelectX4, SelectX5, SelectX6, SelectX7, SelectX8>(
        a, swizzle<X, Y, Z, W>(b)
    );
  }
}
