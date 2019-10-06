#include "vectorization/constants/masks/MaskNone.h"

#include "vectorization/constants/values/Zero.h"

namespace vectorization
{
    template <>
    const bool MaskNone<bool>() noexcept {
        return false;
    }

    template <>
    const Int_8 MaskNone<Int_8>() noexcept {
        return { 0 };
    }

    template <>
    const UInt_8 MaskNone<UInt_8>() noexcept {
        return { 0 };
    }

    template <>
    const Int_16 MaskNone<Int_16>() noexcept {
        return { 0 };
    }

    template <>
    const UInt_16 MaskNone<UInt_16>() noexcept {
        return { 0 };
    }

    template <>
    const Int_32 MaskNone<Int_32>() noexcept {
        return { 0 };
    }

    template <>
    const UInt_32 MaskNone<UInt_32>() noexcept {
        return { 0 };
    }

    template <>
    const Int_64 MaskNone<Int_64>() noexcept {
        return { 0 };
    }

    template <>
    const UInt_64 MaskNone<UInt_64>() noexcept {
        return { 0 };
    }

    template <>
    const Float_32 MaskNone<Float_32>() noexcept {
        return _mm_cvtss_f32(MaskNone<PackedFloat4_128>());
    }

    template <>
    const Float_64 MaskNone<Float_64>() noexcept {
        return _mm_cvtsd_f64(MaskNone<PackedFloat2_128>());
    }

    template <>
    const PackedInts_128 MaskNone<PackedInts_128>() noexcept {
        return _mm_setzero_si128();
    }

    template <>
    const PackedFloat4_128 MaskNone<PackedFloat4_128>() noexcept {
        return _mm_setzero_ps();
    }

    template <>
    const PackedFloat2_128 MaskNone<PackedFloat2_128>() noexcept {
        return _mm_setzero_pd();
    }

    template <>
    const PackedInts_256 MaskNone<PackedInts_256>() noexcept {
        return _mm256_setzero_si256();
    }

    template <>
    const PackedFloat8_256 MaskNone<PackedFloat8_256>() noexcept {
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat4_256 MaskNone<PackedFloat4_256>() noexcept {
        return _mm256_setzero_pd();
    }
}