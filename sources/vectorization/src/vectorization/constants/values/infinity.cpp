#include "vectorization/constants/values/infinity.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

namespace vectorization
{
    template <>
    PackedFloat2_128 Infinity<PackedFloat2_128>() noexcept
    {
        // erase fraction part (52 bits) and sign bit
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 53), 1));
    }

    template <>
    PackedFloat4_128 Infinity<PackedFloat4_128>() noexcept
    {
        // erase fraction part (23 bits) and sign bit
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 24), 1));
    }

    template <>
    PackedFloat4_256 Infinity<PackedFloat4_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_pd(_mm256_srli_epi64(_mm256_slli_epi64(MaskAll<PackedInts_256>(), 53), 1));
#else
        const auto v = Infinity<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
#endif
    }

    template <>
    PackedFloat8_256 Infinity<PackedFloat8_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_ps(_mm256_srli_epi32(_mm256_slli_epi32(MaskAll<PackedInts_256>(), 24), 1));
#else
        const auto v = Infinity<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
#endif
    }

    template <>
    Float_32 Infinity<Float_32>() noexcept
    {
        return x(Infinity<PackedFloat4_128>());
    }

    template <>
    Float_64 Infinity<Float_64>() noexcept
    {
        return x(Infinity<PackedFloat2_128>());
    }
}
