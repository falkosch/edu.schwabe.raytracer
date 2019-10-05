#include "vectorization/v_f32_4/swizzles.h"

namespace vectorization
{
    template <>
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept {
        return v;
    }

    template <>
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_f32_4 & v) noexcept {
        return _mm_unpacklo_ps(v.components, v.components);
    }

    template <>
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const v_f32_4 & v) noexcept {
        return _mm_moveldup_ps(v.components);
    }

    template <>
    const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & v) noexcept {
        return _mm_movelh_ps(v.components, v.components);
    }

    template <>
    const v_f32_4 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept {
        return _mm_movehdup_ps(v.components);
    }

    template <>
    const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept {
        return _mm_unpackhi_ps(v.components, v.components);
    }

    template <>
    const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept {
        return _mm_movehl_ps(v.components, v.components);
    }

    const v_f32_4 xxxx(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
    }

    const v_f32_4 yyyy(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
    }

    const v_f32_4 zzzz(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const v_f32_4 wwww(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
    }

    const v_f32_4 xxxz(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
    }

    const v_f32_4 xxyy(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
    }

    const v_f32_4 xxzz(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const v_f32_4 xyxy(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
    }

    const v_f32_4 xzzz(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
    }

    const v_f32_4 yxxy(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
    }

    const v_f32_4 yxwz(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
    }

    const v_f32_4 yyww(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v);
    }

    const v_f32_4 yzxw(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
    }

    const v_f32_4 yzwx(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
    }

    const v_f32_4 zxyw(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
    }

    const v_f32_4 zzyy(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
    }

    const v_f32_4 zzww(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
    }

    const v_f32_4 zwxy(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
    }

    const v_f32_4 zwzw(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
    }

    const v_f32_4 wxyz(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
    }

    const v_f32_4 wzyx(const v_f32_4 & v) noexcept {
        return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
    }
}