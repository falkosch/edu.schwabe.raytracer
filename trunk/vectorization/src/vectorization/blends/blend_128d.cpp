#include "vectorization/blends/blend_128d.h"

namespace vectorization
{
    template <>
    // passtrough a
    const PackedFloat2_128 blend<false, false>(const PackedFloat2_128 & a, const PackedFloat2_128 &) noexcept {
        return a;
    }

    template <>
    // passthrough b
    const PackedFloat2_128 blend<true, true>(const PackedFloat2_128 &, const PackedFloat2_128 & b) noexcept {
        return b;
    }

    template <>
    // x(a), y(b)
    const PackedFloat2_128 blend<false, true>(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_move_sd(b, a);
    }

    template <>
    // x(b), y(a)
    const PackedFloat2_128 blend<true, false>(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return blend<false, true>(b, a);
    }
}