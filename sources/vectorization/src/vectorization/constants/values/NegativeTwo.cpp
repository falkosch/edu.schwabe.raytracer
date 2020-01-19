#include "vectorization/constants/values/NegativeTwo.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

#include <limits>

namespace vectorization
{
    template <>
    const Int_8 NegativeTwo() noexcept {
        return Int_8{ -2 };
    }

    template <>
    const Int_16 NegativeTwo() noexcept {
        return Int_16{ -2 };
    }

    template <>
    const Int_32 NegativeTwo() noexcept {
        return Int_32{ -2 };
    }

    template <>
    const Int_64 NegativeTwo() noexcept {
        return Int_64{ -2 };
    }

    template <>
    const Float_32 NegativeTwo<Float_32>() noexcept {
        return x(NegativeTwo<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegativeTwo<Float_64>() noexcept {
        return x(NegativeTwo<PackedFloat2_128>());
    }

    template <>
    const PackedFloat2_128 NegativeTwo<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_slli_epi64(
                MaskAll<PackedInts_128>(),
                sizeof(Int_64) * std::numeric_limits<UInt_8>::digits - 2)
        );
    }

    template <>
    const PackedFloat4_128 NegativeTwo<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_slli_epi32(
                MaskAll<PackedInts_128>(),
                sizeof(Int_32) * std::numeric_limits<UInt_8>::digits - 2
            )
        );
    }

    template <>
    const PackedFloat4_256 NegativeTwo<PackedFloat4_256>() noexcept {
        const auto v = NegativeTwo<PackedFloat2_128>();
        return _mm256_set_m128d(v, v);
    }

    template <>
    const PackedFloat8_256 NegativeTwo<PackedFloat8_256>() noexcept {
        const auto v = NegativeTwo<PackedFloat4_128>();
        return _mm256_set_m128(v, v);
    }
}