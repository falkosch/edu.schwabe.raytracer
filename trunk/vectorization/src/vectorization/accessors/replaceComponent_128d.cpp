#include "vectorization/accessors/replaceComponent_128d.h"

namespace vectorization
{
    template <>
    const PackedFloat2_128 replaceComponent<VectorIndices::X>(const PackedFloat2_128 & v, const Float_64 s) noexcept {
        return _mm_shuffle_pd(_mm_set_sd(s), v, 0b10);
    }

    template <>
    const PackedFloat2_128 replaceComponent<VectorIndices::Y>(const PackedFloat2_128 & v, const Float_64 s) noexcept {
        return _mm_shuffle_pd(v, _mm_set_sd(s), 0b00);
    }

    const PackedFloat2_128 replaceX(const PackedFloat2_128 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X>(v, s);
    }

    const PackedFloat2_128 replaceY(const PackedFloat2_128 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::Y>(v, s);
    }

    const PackedFloat2_128 replaceX1(const PackedFloat2_128 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X1>(v, s);
    }

    const PackedFloat2_128 replaceX2(const PackedFloat2_128 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X2>(v, s);
    }
}