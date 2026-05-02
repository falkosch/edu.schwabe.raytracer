#pragma once

#include "type.h"

#include "vectorization/v_ui32_8/blends.h"
#include "vectorization/v_ui32_8/swizzles.h"

namespace vectorization {
  template <ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8,
            bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4,
            bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  v_ui32_8 swizzledBlend(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
    return blend<SelectX1, SelectX2, SelectX3, SelectX4, SelectX5, SelectX6, SelectX7, SelectX8>(
        swizzle<X1, X2, X3, X4>(a), swizzle<X1, X2, X3, X4>(b)
    );
  }

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  v_ui32_8 swizzledBlendMasked(const v_ui32_8 &a, const v_ui32_8 &b, const v_ui32_8 &mask) noexcept {
    return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
  }
}
