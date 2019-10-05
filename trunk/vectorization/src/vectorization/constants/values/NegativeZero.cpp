#include "vectorization/constants/values/NegativeZero.h"

#include "vectorization/constants/masks/MaskAll.h"

#include <limits>

namespace vectorization
{
    template <>
    const PackedFloat4_128 NegativeZero<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_slli_epi32(
                MaskAll<PackedInts_128>(),
                sizeof(Int_32) * std::numeric_limits<unsigned char>::digits - 1
            )
        );
    }

    template <>
    const PackedFloat2_128 NegativeZero<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_slli_epi64(
                MaskAll<PackedInts_128>(),
                sizeof(Int_64) * std::numeric_limits<unsigned char>::digits - 1
            )
        );
    }

    template <>
    const Float_32 NegativeZero<Float_32>() noexcept {
        return _mm_cvtss_f32(NegativeZero<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegativeZero<Float_64>() noexcept {
        return _mm_cvtsd_f64(NegativeZero<PackedFloat2_128>());
    }
}