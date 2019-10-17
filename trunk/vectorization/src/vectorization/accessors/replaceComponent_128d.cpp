#include "vectorization/accessors/replaceComponent_128d.h"

#include <cassert>

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

    const PackedFloat2_128 replaceComponent(const PackedFloat2_128 & v, const PackedFloat2_128 & replacement, const ASizeT index) noexcept {
        assert(index < VectorSizes::Y);
        auto indexBlendMask = _mm_castsi128_pd(
            _mm_cmpeq_epi64(
                _mm_set1_epi64x(static_cast<int>(index)),
                _mm_set_epi64x(1, 0)
            )
        );
        return _mm_blendv_pd(v, replacement, indexBlendMask);
    }

    const PackedFloat2_128 replaceComponent(const PackedFloat2_128 & v, const Float_64 replacement, const ASizeT index) noexcept {
        return replaceComponent(v, _mm_set1_pd(replacement), index);
    }
}