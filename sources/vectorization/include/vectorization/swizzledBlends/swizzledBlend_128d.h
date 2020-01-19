#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization
{
    // generic swizzled blend
    template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY>
    inline const PackedFloat2_128 swizzledBlend(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return blend<SelectX, SelectY>(swizzle<X, Y>(a), swizzle<X, Y>(b));
    }

    // x(a), x(a)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        false, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // x(b), x(b)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        true, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // x(a), x(b)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        false, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // x(b), x(a)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::X, VectorIndices::X,
        true, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // y(a), y(b)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::Y,
        false, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // y(b), y(a)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::Y,
        true, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // y(a), x(a)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        false, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // y(a), x(b)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        false, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // y(b), x(a)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        true, false
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    // y(b), x(b)
    template <>
    const PackedFloat2_128 swizzledBlend<
        VectorIndices::Y, VectorIndices::X,
        true, true
    >(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;
}
