#pragma once

#include "type.h"

namespace vectorization
{
    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // generic swizzle
    inline const v_f32_4 swizzle(const v_f32_4 & v) noexcept {
        static_assert(X < v_f32_4::SIZE, "Index is out of range");
        static_assert(Y < v_f32_4::SIZE, "Index is out of range");
        static_assert(Z < v_f32_4::SIZE, "Index is out of range");
        static_assert(W < v_f32_4::SIZE, "Index is out of range");
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        // seems to create a vex instructions in SSE gen mode
        return _mm_permute_ps(v.components, _MM_SHUFFLE(W, Z, Y, X));
#else
        return _mm_shuffle_ps(v.components, v.components, _MM_SHUFFLE(W, Z, Y, X));
#endif
    }

    template <>
    // passthrough swizzle
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept;

    template <>
    // special unpack low-values swizzle
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_f32_4 & v) noexcept;

    template <>
    // special unpack XZ-values swizzle
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const v_f32_4 & v) noexcept;

    template <>
    // special move low-values swizzle
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & v) noexcept;

    template <>
    // special unpack YW-values swizzle
    const v_f32_4 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept;

    template <>
    // special unpack high-values swizzle
    const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept;

    template <>
    // special move high-values swizzle
    const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept;

    const v_f32_4 xxxx(const v_f32_4 & v) noexcept;

    const v_f32_4 yyyy(const v_f32_4 & v) noexcept;

    const v_f32_4 zzzz(const v_f32_4 & v) noexcept;

    const v_f32_4 wwww(const v_f32_4 & v) noexcept;

    const v_f32_4 xxxz(const v_f32_4 & v) noexcept;

    const v_f32_4 xxyy(const v_f32_4 & v) noexcept;

    const v_f32_4 xxzz(const v_f32_4 & v) noexcept;

    const v_f32_4 xyxy(const v_f32_4 & v) noexcept;

    const v_f32_4 xzzz(const v_f32_4 & v) noexcept;

    const v_f32_4 yxxy(const v_f32_4 & v) noexcept;

    const v_f32_4 yxwz(const v_f32_4 & v) noexcept;

    const v_f32_4 yyww(const v_f32_4 & v) noexcept;

    const v_f32_4 yzxw(const v_f32_4 & v) noexcept;

    const v_f32_4 yzwx(const v_f32_4 & v) noexcept;

    const v_f32_4 zxyw(const v_f32_4 & v) noexcept;

    const v_f32_4 zzyy(const v_f32_4 & v) noexcept;

    const v_f32_4 zzww(const v_f32_4 & v) noexcept;

    const v_f32_4 zwxy(const v_f32_4 & v) noexcept;

    const v_f32_4 zwzw(const v_f32_4 & v) noexcept;

    const v_f32_4 wxyz(const v_f32_4 & v) noexcept;

    const v_f32_4 wzyx(const v_f32_4 & v) noexcept;
}
