#include "vectorization/constants/values/NotANumber.h"

#include "vectorization/constants/masks/MaskAll.h"

#include <limits>

namespace vectorization
{
    // NaNs have all exponent bits set to 1
    // sign bit and fraction bits except MSB are usually undetermined

    // Example: (Signed) Quiet NaN for Float_32 is 1111 1111 11xx xxxx xxxx xxxx xxxx xxxx
    // bits with x are undetermined

    template <>
    const PackedFloat4_128 NotANumber<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_slli_epi32(
                _mm_srli_epi32(
                    MaskAll<PackedInts_128>(),
                    22
                ),
                22
            )
        );
    }

    template <>
    const PackedFloat2_128 NotANumber<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_slli_epi64(
                _mm_srli_epi64(
                    MaskAll<PackedInts_128>(),
                    51
                ),
                51
            )
        );
    }

    template <>
    const Float_32 NotANumber<Float_32>() noexcept {
        return _mm_cvtss_f32(NotANumber<PackedFloat4_128>());
    }

    template <>
    const Float_64 NotANumber<Float_64>() noexcept {
        return _mm_cvtsd_f64(NotANumber<PackedFloat2_128>());
    }
}