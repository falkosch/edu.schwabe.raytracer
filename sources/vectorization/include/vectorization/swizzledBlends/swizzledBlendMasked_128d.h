#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y>
  inline PackedFloat2_128
  swizzledBlendMasked(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &mask) noexcept {
    return blendMasked(swizzle<X, Y>(a), swizzle<X, Y>(b), mask);
  }

  template <ASizeT X, ASizeT Y>
  inline PackedFloat2_128
  swizzledBlendMasked(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedInts_128 &mask) noexcept {
    return blendMasked(swizzle<X, Y>(a), swizzle<X, Y>(b), mask);
  }
}
