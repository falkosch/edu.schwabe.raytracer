#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  // generic swizzled blend
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  inline PackedFloat4_256 swizzledBlend(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
  }
}
