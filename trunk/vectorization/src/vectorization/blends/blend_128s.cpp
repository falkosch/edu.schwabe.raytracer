#include "vectorization/blends/blend_128s.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 blend<false, false, false, false>(const PackedFloat4_128 & a, const PackedFloat4_128 &) noexcept {
        return a;
    }

    template <>
    const PackedFloat4_128 blend<true, true, true, true>(const PackedFloat4_128 &, const PackedFloat4_128 & b) noexcept {
        return b;
    }

    template <>
    const PackedFloat4_128 blend<false, false, true, true>(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_shuffle_ps(a, b, _MM_SHUFFLE(VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X));
    }

    template <>
    const PackedFloat4_128 blend<false, true, true, true>(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_move_ss(b, a);
    }

    template <>
    const PackedFloat4_128 blend<true, false, false, false>(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return blend<false, true, true, true>(b, a);
    }

    template <>
    const PackedFloat4_128 blend<true, true, false, false>(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return blend<false, false, true, true>(b, a);
    }
}