#pragma once

#include "type.h"

namespace vectorization
{
    template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
    // generic blend of two vectors
    inline const v_f32_4 blend(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_blend_ps(a.components, b.components, BLEND_MASK_FLOAT4_32(SelectX, SelectY, SelectZ, SelectW));
    }

    template <>
    // passtrough blend
    const v_f32_4 blend<false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept;

    template <>
    // passthrough blend other
    const v_f32_4 blend<true, true, true, true>(const v_f32_4 &, const v_f32_4 & b) noexcept;

    template <>
    // blend using shuffle
    const v_f32_4 blend<false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

    template <>
    // move x(a) into b
    const v_f32_4 blend<false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

    template <>
    // move x(b) into a
    const v_f32_4 blend<true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

    const v_f32_4 x_yzw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

    const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4 & mask) noexcept;

    const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4::VectorBoolType & mask) noexcept;
}
