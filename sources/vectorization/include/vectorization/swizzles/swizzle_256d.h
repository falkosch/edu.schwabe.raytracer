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

        // cross-swap the 128bit lanes first
        const auto swappedV = _mm256_permute2f128_pd(v, v, 0b0001);

        // permute components on both v and swappedV into the expected slots
        // as if we would permute only in PackedFloat2_128
        constexpr auto permuteMask = (X & 1) | ((Y & 1) << 1) | ((Z & 1) << 2) | ((W & 1) << 3);
        const auto swizzledV = _mm256_permute_pd(v, permuteMask);
        const auto swizzledSwappedV = _mm256_permute_pd(swappedV, permuteMask);

        // blend both full 256 bit according to the original indices.
        constexpr auto blendMask = ASizeT{ X >= VectorIndices::Z } | (ASizeT{ Y >= VectorIndices::Z } << 1)
            | (ASizeT{ Z < VectorIndices::Z } << 2) | (ASizeT{ W < VectorIndices::Z } << 3);
        return _mm256_blend_pd(swizzledV, swizzledSwappedV, blendMask);
    }

    template <>
    // passthrough swizzle
    const PackedFloat4_256 swizzle<
        VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W
    >(const PackedFloat4_256 & v) noexcept;

    template <>
    // duplicate lo swizzle
    const PackedFloat4_256 swizzle<
        VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z
    >(const PackedFloat4_256 & v) noexcept;

    template <>
    // permute lanes lo-lo swizzle
    const PackedFloat4_256 swizzle<
        VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y
    >(const PackedFloat4_256 & v) noexcept;

    template <>
    // unpack hi swizzle
    const PackedFloat4_256 swizzle<
        VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W
    >(const PackedFloat4_256 & v) noexcept;

    template <>
    // permute lanes hi-lo swizzle
    const PackedFloat4_256 swizzle<
        VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y
    >(const PackedFloat4_256 & v) noexcept;

    template <>
    // permute lanes hi-hi swizzle
    const PackedFloat4_256 swizzle<
        VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W
    >(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 xxxx(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 xxxz(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 xxyy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 xxzz(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 xyxy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 xzzz(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 yyyy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 yxxy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 yxwz(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 yyww(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 yzxw(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 yzwx(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 zzzz(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 zxyw(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 zzyy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 zzww(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 zwxy(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 zwzw(const PackedFloat4_256 & v) noexcept;

    const PackedFloat4_256 wwww(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 wxyz(const PackedFloat4_256 & v) noexcept;
    const PackedFloat4_256 wzyx(const PackedFloat4_256 & v) noexcept;
}