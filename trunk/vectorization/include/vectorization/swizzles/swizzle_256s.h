#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8>
    // generic swizzle
    inline const PackedFloat8_256 swizzle(const PackedFloat8_256 & v) noexcept {
        static_assert(X1 < VectorSizes::X8, "Index X1 is out of range");
        static_assert(X2 < VectorSizes::X8, "Index Y1 is out of range");
        static_assert(X3 < VectorSizes::X8, "Index Z1 is out of range");
        static_assert(X4 < VectorSizes::X8, "Index W1 is out of range");
        static_assert(X5 < VectorSizes::X8, "Index X2 is out of range");
        static_assert(X6 < VectorSizes::X8, "Index Y2 is out of range");
        static_assert(X7 < VectorSizes::X8, "Index Z2 is out of range");
        static_assert(X8 < VectorSizes::X8, "Index W2 is out of range");
        // TODO
        return _mm256_setzero_pd();
    }

    template <>
    // passthrough swizzle
    const PackedFloat8_256 swizzle<
        VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4,
        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8
    >(const PackedFloat8_256 & v) noexcept;
}