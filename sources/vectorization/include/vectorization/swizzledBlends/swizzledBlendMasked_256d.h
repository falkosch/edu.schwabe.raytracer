#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const PackedFloat4_256
  swizzledBlendMasked(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &mask) noexcept {
    return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
  }

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const PackedFloat4_256
  swizzledBlendMasked(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedInts_256 &mask) noexcept {
    return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
  }
}
