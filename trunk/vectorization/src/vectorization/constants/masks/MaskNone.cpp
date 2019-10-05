#include "vectorization/constants/masks/MaskNone.h"

#include "vectorization/constants/values/Zero.h"

namespace vectorization
{
    template <>
    const bool MaskNone<bool>() noexcept {
        return Zero<bool>();
    }

    template <>
    const Int_8 MaskNone<Int_8>() noexcept {
        return Zero<Int_8>();
    }

    template <>
    const UInt_8 MaskNone<UInt_8>() noexcept {
        return Zero<UInt_8>();
    }

    template <>
    const Int_16 MaskNone<Int_16>() noexcept {
        return Zero<Int_16>();
    }

    template <>
    const UInt_16 MaskNone<UInt_16>() noexcept {
        return Zero<UInt_16>();
    }

    template <>
    const Int_32 MaskNone<Int_32>() noexcept {
        return Zero<Int_32>();
    }

    template <>
    const UInt_32 MaskNone<UInt_32>() noexcept {
        return Zero<UInt_32>();
    }

    template <>
    const Int_64 MaskNone<Int_64>() noexcept {
        return Zero<Int_64>();
    }

    template <>
    const UInt_64 MaskNone<UInt_64>() noexcept {
        return Zero<UInt_64>();
    }

    template <>
    const PackedInts_128 MaskNone<PackedInts_128>() noexcept {
        return Zero<PackedInts_128>();
    }

    template <>
    const PackedFloat4_128 MaskNone<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(MaskNone<PackedInts_128>());
    }

    template <>
    const PackedFloat2_128 MaskNone<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(MaskNone<PackedInts_128>());
    }
}