#include "vectorization/swizzles/swizzle_256d.h"

namespace vectorization
{
    template <>
    const PackedFloat4_256 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_256 & v) noexcept {
        return v;
    }

    template <>
    const PackedFloat4_256 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat4_256 & v) noexcept {
        return _mm256_movedup_pd(v);
    }

    template <>
    const PackedFloat4_256 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_256 & v) noexcept {
        return _mm256_permute2f128_pd(v, v, 0b00000000);
    }

    template <>
    const PackedFloat4_256 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat4_256 & v) noexcept {
        return _mm256_unpackhi_pd(v, v);
    }

    template <>
    const PackedFloat4_256 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_256 & v) noexcept {
        return _mm256_permute2f128_pd(v, v, 0b00000001);
    }

    template <>
    const PackedFloat4_256 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_256 & v) noexcept {
        return _mm256_permute2f128_pd(v, v, 0b00010001);
    }

    const PackedFloat4_256 xxzz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 xyxy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 yyww(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v);
    }

    const PackedFloat4_256 zwxy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 zwzw(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
    }
}