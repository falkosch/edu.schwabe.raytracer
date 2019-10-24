#include "vectorization/swizzledBlends/swizzledBlend_128s.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y,
        false, true, false, true
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_unpacklo_ps(a, b);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y,
        true, false, true, false
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y,
            false, true, false, true
        >(b, a);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y,
        false, false, true, true
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_movelh_ps(a, b);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y,
        true, true, false, false
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y,
            false, false, true, true
        >(b, a);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W,
        false, true, false, true
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_unpackhi_ps(a, b);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W,
        true, false, true, false
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W,
            false, true, false, true
        >(b, a);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W,
        true, true, false, false
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_movehl_ps(a, b);
    }

    template <>
    const PackedFloat4_128 swizzledBlend<
        VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W,
        false, false, true, true
    >(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return swizzledBlend<
            VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W,
            true, true, false, false
        >(b, a);
    }
}