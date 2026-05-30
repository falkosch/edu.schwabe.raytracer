#include "vectorization/constants/values/negative_zero.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

#include <limits>

namespace vectorization
{
    template <>
    PackedFloat2_128 NegativeZero<PackedFloat2_128>() noexcept
    {
        return _mm_castsi128_pd(
            _mm_slli_epi64(MaskAll<PackedInts_128>(), sizeof(Int_64) * std::numeric_limits<UInt_8>::digits - 1)
        );
    }

    template <>
    PackedFloat4_128 NegativeZero<PackedFloat4_128>() noexcept
    {
        return _mm_castsi128_ps(
            _mm_slli_epi32(MaskAll<PackedInts_128>(), sizeof(Int_32) * std::numeric_limits<UInt_8>::digits - 1)
        );
    }

    template <>
    PackedFloat4_256 NegativeZero<PackedFloat4_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_pd(
            _mm256_slli_epi64(MaskAll<PackedInts_256>(), sizeof(Int_64) * std::numeric_limits<UInt_8>::digits - 1)
        );
#else
        const auto v = NegativeZero<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
#endif
    }

    template <>
    PackedFloat8_256 NegativeZero<PackedFloat8_256>() noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_castsi256_ps(
            _mm256_slli_epi32(MaskAll<PackedInts_256>(), sizeof(Int_32) * std::numeric_limits<UInt_8>::digits - 1)
        );
#else
        const auto v = NegativeZero<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
#endif
    }

    template <>
    Float_32 NegativeZero<Float_32>() noexcept
    {
        return x(NegativeZero<PackedFloat4_128>());
    }

    template <>
    Float_64 NegativeZero<Float_64>() noexcept
    {
        return x(NegativeZero<PackedFloat2_128>());
    }
}
