#pragma once

#include "blends.h"
#include "swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W,
            bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  v_i64_4 swizzledBlend(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return blend<SelectX, SelectY, SelectZ, SelectW>(a, swizzle<X, Y, Z, W>(b));
  }
}
