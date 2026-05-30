#include "vectorization/constants/values/one_half.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

namespace vectorization
{
    template <>
    PackedFloat4_128 OneHalf<PackedFloat4_128>() noexcept
    {
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 24), 2));
    }

    template <>
    PackedFloat2_128 OneHalf<PackedFloat2_128>() noexcept
    {
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 53), 2));
    }

    template <>
    PackedFloat4_256 OneHalf<PackedFloat4_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_pd(_mm256_srli_epi64(_mm256_slli_epi64(MaskAll<PackedInts_256>(), 53), 2));
#else
        const auto v = OneHalf<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
#endif
    }

    template <>
    PackedFloat8_256 OneHalf<PackedFloat8_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_ps(_mm256_srli_epi32(_mm256_slli_epi32(MaskAll<PackedInts_256>(), 24), 2));
#else
        const auto v = OneHalf<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
#endif
    }

    template <>
    Float_32 OneHalf<Float_32>() noexcept
    {
        return x(OneHalf<PackedFloat4_128>());
    }

    template <>
    Float_64 OneHalf<Float_64>() noexcept
    {
        return x(OneHalf<PackedFloat2_128>());
    }
}
