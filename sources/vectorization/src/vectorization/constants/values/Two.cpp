#include "vectorization/constants/values/Two.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

#include <limits>

namespace vectorization
{
    template <>
    const Int_8 Two() noexcept {
        return Int_8{ 2 };
    }

    template <>
    const UInt_8 Two() noexcept {
        return UInt_8{ 2 };
    }

    template <>
    const Int_16 Two() noexcept {
        return Int_16{ 2 };
    }

    template <>
    const UInt_16 Two() noexcept {
        return UInt_16{ 2 };
    }

    template <>
    const Int_32 Two() noexcept {
        return Int_32{ 2 };
    }

    template <>
    const UInt_32 Two() noexcept {
        return UInt_32{ 2 };
    }

    template <>
    const Int_64 Two() noexcept {
        return Int_64{ 2 };
    }

    template <>
    const UInt_64 Two() noexcept {
        return UInt_64{ 2 };
    }

    template <>
    const Float_32 Two<Float_32>() noexcept {
        return x(Two<PackedFloat4_128>());
    }

    template <>
    const Float_64 Two<Float_64>() noexcept {
        return x(Two<PackedFloat2_128>());
    }

    template <>
    const PackedFloat4_128 Two<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_srli_epi32(
                _mm_slli_epi32(
                    MaskAll<PackedInts_128>(),
                    sizeof(Int_32) * std::numeric_limits<UInt_8>::digits - 1
                ),
                1
            )
        );
    }

    template <>
    const PackedFloat2_128 Two<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_srli_epi64(
                _mm_slli_epi64(
                    MaskAll<PackedInts_128>(),
                    sizeof(Int_64) * std::numeric_limits<UInt_8>::digits - 1
                ),
                1
            )
        );
    }

    template <>
    const PackedFloat4_256 Two<PackedFloat4_256>() noexcept {
        const auto v = Two<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
    }

    template <>
    const PackedFloat8_256 Two<PackedFloat8_256>() noexcept {
        const auto v = Two<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
    }
}