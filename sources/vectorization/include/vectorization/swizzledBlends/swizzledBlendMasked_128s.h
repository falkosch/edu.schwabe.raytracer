#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const PackedFloat4_128
  swizzledBlendMasked(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &mask) noexcept {
    return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
  }

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const PackedFloat4_128
  swizzledBlendMasked(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedInts_128 &mask) noexcept {
    return swizzledBlendMasked<X, Y, Z, W>(a, b, _mm_castsi128_ps(mask));
  }
}
