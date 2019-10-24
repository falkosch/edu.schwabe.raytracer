#include "vectorization/v_f32_4/swizzledBlends.h"

namespace vectorization
{
    const v_f32_4 xx_xx(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
    }

    const v_f32_4 xx_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
    }

    const v_f32_4 xy_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
    }

    const v_f32_4 xy_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
    }

    const v_f32_4 xz_xz(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
    }

    const v_f32_4 yx_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(a, b);
    }

    const v_f32_4 yy_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
    }

    const v_f32_4 yw_yw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
    }

    const v_f32_4 zz_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(a, b);
    }

    const v_f32_4 zz_zz(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
    }

    const v_f32_4 zw_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
    }

    const v_f32_4 ww_ww(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return swizzledBlend2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
    }
}