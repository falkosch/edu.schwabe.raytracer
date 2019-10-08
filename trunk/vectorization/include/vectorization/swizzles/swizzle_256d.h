#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // generic swizzle
    inline const PackedFloat4_256 swizzle(const PackedFloat4_256 & v) noexcept {
        static_assert(X < VectorSizes::W, "Index X is out of range");
        static_assert(Y < VectorSizes::W, "Index Y is out of range");
        static_assert(Z < VectorSizes::W, "Index Z is out of range");
        static_assert(W < VectorSizes::W, "Index W is out of range");
        // TODO
        return _mm256_setzero_pd();
    }

    template <>
    // passthrough swizzle
    const PackedFloat4_256 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_256 & v) noexcept;

    template <>
    // duplicate lo swizzle
    const PackedFloat4_256 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat4_256 & v) noexcept;

    template <>
    // permute lanes lo-lo swizzle
    const PackedFloat4_256 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_256 & v) noexcept;

    template <>
    // unpack hi swizzle
    const PackedFloat4_256 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat4_256 & v) noexcept;

    template <>
    // permute lanes hi-lo swizzle
    const PackedFloat4_256 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_256 & v) noexcept;

    template <>
    // permute lanes hi-hi swizzle
    const PackedFloat4_256 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 xxzz(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 xyxy(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 yyww(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 zwxy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 zwzw(const PackedFloat4_256 & v) noexcept;
}