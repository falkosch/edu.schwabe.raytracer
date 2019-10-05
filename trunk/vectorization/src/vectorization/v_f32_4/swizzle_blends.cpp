#include "vectorization/v_f32_4/swizzled_blends.h"

namespace vectorization
{
    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept {
        return a;
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, true, true, true>(const v_f32_4 &, const v_f32_4 & b) noexcept {
        return b;
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        // move x(b) into a
        return _mm_move_ss(a.components, b.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        // move x(a) into b
        return _mm_move_ss(b.components, a.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_unpacklo_ps(a.components, b.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_unpacklo_ps(b.components, a.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_movelh_ps(a.components, b.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_movelh_ps(b.components, a.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_unpackhi_ps(a.components, b.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_unpackhi_ps(b.components, a.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_movehl_ps(a.components, b.components);
    }

    template <>
    const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_movehl_ps(b.components, a.components);
    }

    template <>
    const v_f32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_movelh_ps(a.components, b.components);
    }

    template <>
    const v_f32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_movehl_ps(b.components, a.components);
    }

    const v_f32_4 xx_xx(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
    }

    const v_f32_4 xx_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
    }

    const v_f32_4 xy_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
    }

    const v_f32_4 xy_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
    }

    const v_f32_4 xz_xz(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
    }

    const v_f32_4 yx_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(a, b);
    }

    const v_f32_4 yy_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
    }

    const v_f32_4 yw_yw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
    }

    const v_f32_4 zz_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(a, b);
    }

    const v_f32_4 zz_zz(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
    }

    const v_f32_4 zw_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
    }

    const v_f32_4 ww_ww(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend_2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
    }

    template <>
    const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & mask) noexcept {
        return blendMasked(a, b, mask);
    }

    template <>
    const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::VectorBoolType & mask) noexcept {
        return blendMasked(a, b, mask);
    }
}