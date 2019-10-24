#include "vectorization/swizzledBlends/swizzledBlend_128d.h"

namespace vectorization
{
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        false, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 &) noexcept {
        return _mm_movedup_pd(a);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        true, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::X, VectorIndices::X,
            false, false
        >(b, a);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        false, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_unpacklo_pd(a, b);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        true, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::X, VectorIndices::X,
            false, true
        >(b, a);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::Y,
        false, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_unpackhi_pd(a, b);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::Y,
        true, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::Y, VectorIndices::Y,
            false, true
        >(b, a);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        false, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 &) noexcept {
        return _mm_shuffle_pd(a, a, 0b01);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        false, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_shuffle_pd(a, b, 0b01);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        true, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::Y, VectorIndices::X,
            false, true
        >(b, a);
    }

    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        true, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::Y, VectorIndices::X,
            false, false
        >(b, a);
    }
}