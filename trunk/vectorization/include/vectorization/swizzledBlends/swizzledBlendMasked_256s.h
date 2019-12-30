#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization
{
    template <ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8>
    inline const PackedFloat8_256 swizzledBlendMasked(const PackedFloat8_256 & a, const PackedFloat8_256 & b, const PackedFloat8_256 & mask) noexcept {
        return blendMasked(swizzle<X1, X2, X3, X4, X5, X6, X7, X8>(a), swizzle<X1, X2, X3, X4, X5, X6, X7, X8>(b), mask);
    }

    template <ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8>
    inline const PackedFloat8_256 swizzledBlendMasked(const PackedFloat8_256 & a, const PackedFloat8_256 & b, const PackedInts_256 & mask) noexcept {
        return blendMasked(swizzle<X1, X2, X3, X4, X5, X6, X7, X8>(a), swizzle<X1, X2, X3, X4, X5, X6, X7, X8>(b), mask);
    }
}
