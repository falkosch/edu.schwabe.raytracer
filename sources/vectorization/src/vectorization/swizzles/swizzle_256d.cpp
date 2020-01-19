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

    const PackedFloat4_256 xxxx(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
    }

    const PackedFloat4_256 xxxz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 xxyy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 xxzz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 xyxy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 xzzz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 yyyy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 yxxy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 yxwz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 yyww(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v);
    }

    const PackedFloat4_256 yzxw(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
    }

    const PackedFloat4_256 yzwx(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
    }

    const PackedFloat4_256 zzzz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 zxyw(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
    }

    const PackedFloat4_256 zzyy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 zzww(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
    }

    const PackedFloat4_256 zwxy(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
    }

    const PackedFloat4_256 zwzw(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
    }

    const PackedFloat4_256 wwww(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
    }

    const PackedFloat4_256 wxyz(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
    }

    const PackedFloat4_256 wzyx(const PackedFloat4_256 & v) noexcept {
        return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
    }
}