#include "vectorization/constants/values/not_a_number.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

#include <limits>

namespace vectorization
{
    // NaNs have all exponent bits set to 1.
    // Sign bit and fraction bits except MSB are usually undetermined.

    // Example: (Signed) Quiet NaN for Float_32 is 1111 1111 11xx xxxx xxxx xxxx xxxx xxxx
    // bits with x are undetermined

    template <>
    PackedFloat4_128 NotANumber<PackedFloat4_128>() noexcept
    {
        return _mm_castsi128_ps(_mm_slli_epi32(_mm_srli_epi32(MaskAll<PackedInts_128>(), 22), 22));
    }

    template <>
    PackedFloat2_128 NotANumber<PackedFloat2_128>() noexcept
    {
        return _mm_castsi128_pd(_mm_slli_epi64(_mm_srli_epi64(MaskAll<PackedInts_128>(), 51), 51));
    }

    template <>
    PackedFloat4_256 NotANumber<PackedFloat4_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_pd(_mm256_slli_epi64(_mm256_srli_epi64(MaskAll<PackedInts_256>(), 51), 51));
#else
        const auto v = NotANumber<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
#endif
    }

    template <>
    PackedFloat8_256 NotANumber<PackedFloat8_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_ps(_mm256_slli_epi32(_mm256_srli_epi32(MaskAll<PackedInts_256>(), 22), 22));
#else
        const auto v = NotANumber<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
#endif
    }

    template <>
    Float_32 NotANumber<Float_32>() noexcept
    {
        return x(NotANumber<PackedFloat4_128>());
    }

    template <>
    Float_64 NotANumber<Float_64>() noexcept
    {
        return x(NotANumber<PackedFloat2_128>());
    }
}
