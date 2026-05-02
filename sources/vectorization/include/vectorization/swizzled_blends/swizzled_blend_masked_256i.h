#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends/blend_masked_256i.h"
#include "vectorization/swizzles/swizzle_256i.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  PackedInts_256 swizzledBlendMasked(const PackedInts_256 &a, const PackedInts_256 &b, const PackedInts_256 &mask) noexcept {
    return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
  }
}
