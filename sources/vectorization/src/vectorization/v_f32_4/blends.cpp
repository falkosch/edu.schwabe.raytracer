#include "vectorization/v_f32_4/blends.h"

namespace vectorization
{
    const v_f32_4 x_yzw(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return x_yzw(a.components, b.components);
    }

    const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4 & mask) noexcept {
        return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
    }

    const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4::VectorBoolType & mask) noexcept {
        return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
    }
}