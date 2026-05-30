#include "vectorization/constants/values/half.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

namespace vectorization
{
    template <>
    PackedFloat2_128 Half<PackedFloat2_128>() noexcept
    {
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 55), 2));
    }

    template <>
    PackedFloat4_128 Half<PackedFloat4_128>() noexcept
    {
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 26), 2));
    }

    template <>
    PackedFloat4_256 Half<PackedFloat4_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_pd(_mm256_srli_epi64(_mm256_slli_epi64(MaskAll<PackedInts_256>(), 55), 2));
#else
        const auto v = Half<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
#endif
    }

    template <>
    PackedFloat8_256 Half<PackedFloat8_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_ps(_mm256_srli_epi32(_mm256_slli_epi32(MaskAll<PackedInts_256>(), 26), 2));
#else
        const auto v = Half<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
#endif
    }

    template <>
    Float_32 Half<Float_32>() noexcept
    {
        return x(Half<PackedFloat4_128>());
    }

    template <>
    Float_64 Half<Float_64>() noexcept
    {
        return x(Half<PackedFloat2_128>());
    }
}
