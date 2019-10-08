#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // generic swizzle
    inline const PackedFloat4_128 swizzle(const PackedFloat4_128 & v) noexcept {
        static_assert(X < VectorSizes::W, "Index X is out of range");
        static_assert(Y < VectorSizes::W, "Index Y is out of range");
        static_assert(Z < VectorSizes::W, "Index Z is out of range");
        static_assert(W < VectorSizes::W, "Index W is out of range");
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm_permute_ps(v, _MM_SHUFFLE(W, Z, Y, X));
#else
        return _mm_shuffle_ps(v, v, _MM_SHUFFLE(W, Z, Y, X));
#endif
    }

    template <>
    // passthrough swizzle
    const PackedFloat4_128 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_128 & v) noexcept;

    template <>
    // unpack low-values swizzle
    const PackedFloat4_128 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const PackedFloat4_128 & v) noexcept;

    template <>
    // duplicate XZ-values swizzle
    const PackedFloat4_128 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat4_128 & v) noexcept;

    template <>
    // move low-values swizzle
    const PackedFloat4_128 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_128 & v) noexcept;

    template <>
    // duplicate YW-values swizzle
    const PackedFloat4_128 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat4_128 & v) noexcept;

    template <>
    // unpack high-values swizzle
    const PackedFloat4_128 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const PackedFloat4_128 & v) noexcept;

    template <>
    // move high-values swizzle
    const PackedFloat4_128 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_128 & v) noexcept;

    const PackedFloat4_128 xxxx(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 yyyy(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 zzzz(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 wwww(const PackedFloat4_128 & v) noexcept;

    const PackedFloat4_128 xxxz(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 xxyy(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 xxzz(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 xyxy(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 xzzz(const PackedFloat4_128 & v) noexcept;

    const PackedFloat4_128 yxxy(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 yxwz(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 yyww(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 yzxw(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 yzwx(const PackedFloat4_128 & v) noexcept;

    const PackedFloat4_128 zxyw(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 zzyy(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 zzww(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 zwxy(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 zwzw(const PackedFloat4_128 & v) noexcept;

    const PackedFloat4_128 wxyz(const PackedFloat4_128 & v) noexcept;
    const PackedFloat4_128 wzyx(const PackedFloat4_128 & v) noexcept;
}