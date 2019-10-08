#include "vectorization/swizzles/swizzle_256s.h"

namespace vectorization
{
    template <>
    const PackedFloat8_256 swizzle<
        VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4,
        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8
    >(const PackedFloat8_256 & v) noexcept {
        return v;
    }
}