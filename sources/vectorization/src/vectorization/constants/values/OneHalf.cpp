#include "vectorization/constants/values/OneHalf.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

namespace vectorization
{
    template <>
    const Float_32 OneHalf<Float_32>() noexcept {
        return x(OneHalf<PackedFloat4_128>());
    }

    template <>
    const Float_64 OneHalf<Float_64>() noexcept {
        return x(OneHalf<PackedFloat2_128>());
    }

    template <>
    const PackedFloat4_128 OneHalf<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_srli_epi32(
                _mm_slli_epi32(MaskAll<PackedInts_128>(), 24),
                2
            )
        );
    }

    template <>
    const PackedFloat2_128 OneHalf<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_srli_epi64(
                _mm_slli_epi64(MaskAll<PackedInts_128>(), 53),
                2
            )
        );
    }

    template <>
    const PackedFloat4_256 OneHalf<PackedFloat4_256>() noexcept {
        const auto v = OneHalf<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
    }

    template <>
    const PackedFloat8_256 OneHalf<PackedFloat8_256>() noexcept {
        const auto v = OneHalf<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
    }
}