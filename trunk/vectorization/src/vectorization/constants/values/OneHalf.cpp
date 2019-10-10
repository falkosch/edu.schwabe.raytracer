#include "vectorization/constants/values/OneHalf.h"

#include "vectorization/constants/masks.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 OneHalf<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_srli_epi32(
                _mm_slli_epi32(
                    MaskAll<PackedInts_128>(),
                    24
                ),
                2
            )
        );
    }

    template <>
    const PackedFloat2_128 OneHalf<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_srli_epi64(
                _mm_slli_epi64(
                    MaskAll<PackedInts_128>(),
                    53
                ),
                2
            )
        );
    }

    template <>
    const Float_32 OneHalf<Float_32>() noexcept {
        return _mm_cvtss_f32(OneHalf<PackedFloat4_128>());
    }

    template <>
    const Float_64 OneHalf<Float_64>() noexcept {
        return _mm_cvtsd_f64(OneHalf<PackedFloat2_128>());
    }
}