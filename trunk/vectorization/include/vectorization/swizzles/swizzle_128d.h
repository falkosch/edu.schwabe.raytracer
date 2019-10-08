#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <ASizeT X, ASizeT Y>
    // generic swizzle
    inline const PackedFloat2_128 swizzle(const PackedFloat2_128 & v) noexcept {
        static_assert(X < VectorSizes::Y, "Index X is out of range");
        static_assert(Y < VectorSizes::Y, "Index Y is out of range");
        constexpr auto swizzleMask = X + (Y << 1);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm_permute_pd(v, swizzleMask);
#else
        return _mm_shuffle_pd(v, v, swizzleMask);
#endif
    }

    template <>
    // passthrough swizzle
    const PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::Y>(const PackedFloat2_128 & v) noexcept;

    template <>
    // duplicate low swizzle
    const PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::X>(const PackedFloat2_128 & v) noexcept;

    template <>
    // unpack high swizzle
    const PackedFloat2_128 swizzle<VectorIndices::Y, VectorIndices::Y>(const PackedFloat2_128 & v) noexcept;

    const PackedFloat2_128 xx(const PackedFloat2_128 & v) noexcept;

    const PackedFloat2_128 yx(const PackedFloat2_128 & v) noexcept;

    const PackedFloat2_128 yy(const PackedFloat2_128 & v) noexcept;
}