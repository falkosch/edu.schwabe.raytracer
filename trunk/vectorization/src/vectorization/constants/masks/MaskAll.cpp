#include "vectorization/constants/masks/MaskAll.h"

#include "vectorization/constants/values/Zero.h"

namespace vectorization
{
    template <>
    const bool MaskAll<bool>() noexcept {
        return true;
    }

    template <>
    const Int_8 MaskAll<Int_8>() noexcept {
        return { -1 };
    }

    template <>
    const UInt_8 MaskAll<UInt_8>() noexcept {
        return static_cast<UInt_8>(-1);
    }

    template <>
    const Int_16 MaskAll<Int_16>() noexcept {
        return { -1 };
    }

    template <>
    const UInt_16 MaskAll<UInt_16>() noexcept {
        return static_cast<UInt_16>(-1);
    }

    template <>
    const Int_32 MaskAll<Int_32>() noexcept {
        return { -1 };
    }

    template <>
    const UInt_32 MaskAll<UInt_32>() noexcept {
        return static_cast<UInt_32>(-1);
    }

    template <>
    const Int_64 MaskAll<Int_64>() noexcept {
        return Int_64{ -1 };
    }

    template <>
    const UInt_64 MaskAll<UInt_64>() noexcept {
        return static_cast<UInt_64>(-1);
    }

    template <>
    const Float_32 MaskAll<Float_32>() noexcept {
        return _mm_cvtss_f32(MaskAll<PackedFloat4_128>());
    }

    template <>
    const Float_64 MaskAll<Float_64>() noexcept {
        return _mm_cvtsd_f64(MaskAll<PackedFloat2_128>());
    }

    template <>
    const PackedInts_128 MaskAll<PackedInts_128>() noexcept {
        return _mm_cmpeq_epi32(Zero<PackedInts_128>(), Zero<PackedInts_128>());
    }

    template <>
    const PackedFloat4_128 MaskAll<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(MaskAll<PackedInts_128>());
    }

    template <>
    const PackedFloat2_128 MaskAll<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(MaskAll<PackedInts_128>());
    }

    template <>
    const PackedInts_256 MaskAll<PackedInts_256>() noexcept {
        return _mm256_set1_epi64x(long long{ -1 });
    }

    template <>
    const PackedFloat8_256 MaskAll<PackedFloat8_256>() noexcept {
        return _mm256_castsi256_ps(MaskAll<PackedInts_256>());
    }

    template <>
    const PackedFloat4_256 MaskAll<PackedFloat4_256>() noexcept {
        return _mm256_castsi256_pd(MaskAll<PackedInts_256>());
    }
}