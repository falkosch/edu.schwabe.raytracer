#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  // generic swizzled blend
  template <
      ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8, bool SelectX1,
      bool SelectX2, bool SelectX3, bool SelectX4, bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  inline PackedFloat8_256 swizzledBlend(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return blend<SelectX1, SelectX2, SelectX3, SelectX4, SelectX5, SelectX6, SelectX7, SelectX8>(
        swizzle<X1, X2, X3, X4, X5, X6, X7, X8>(a), swizzle<X1, X2, X3, X4, X5, X6, X7, X8>(b)
    );
  }
}
