#include "vectorization/swizzles/swizzle_128d.h"

namespace vectorization
{
    template <>
    const PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::Y>(const PackedFloat2_128 & v) noexcept {
        return v;
    }

    template <>
    const PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::X>(const PackedFloat2_128 & v) noexcept {
        return _mm_movedup_pd(v);
    }

    template <>
    const PackedFloat2_128 swizzle<VectorIndices::Y, VectorIndices::Y>(const PackedFloat2_128 & v) noexcept {
        return _mm_unpackhi_pd(v, v);
    }

    const PackedFloat2_128 xx(const PackedFloat2_128 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X>(v);
    }

    const PackedFloat2_128 yx(const PackedFloat2_128 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::X>(v);
    }

    const PackedFloat2_128 yy(const PackedFloat2_128 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Y>(v);
    }
}