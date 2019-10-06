#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename V>
    const V MaskAll() noexcept;

    template <>
    const bool MaskAll<bool>() noexcept;

    template <>
    const Int_8 MaskAll<Int_8>() noexcept;

    template <>
    const UInt_8 MaskAll<UInt_8>() noexcept;

    template <>
    const Int_16 MaskAll<Int_16>() noexcept;

    template <>
    const UInt_16 MaskAll<UInt_16>() noexcept;

    template <>
    const Int_32 MaskAll<Int_32>() noexcept;

    template <>
    const UInt_32 MaskAll<UInt_32>() noexcept;

    template <>
    const Int_64 MaskAll<Int_64>() noexcept;

    template <>
    const UInt_64 MaskAll<UInt_64>() noexcept;

    template <>
    const Float_32 MaskAll<Float_32>() noexcept;

    template <>
    const Float_64 MaskAll<Float_64>() noexcept;

    template <>
    const PackedInts_128 MaskAll<PackedInts_128>() noexcept;

    template <>
    const PackedFloat4_128 MaskAll<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 MaskAll<PackedFloat2_128>() noexcept;

    template <>
    const PackedInts_256 MaskAll<PackedInts_256>() noexcept;

    template <>
    const PackedFloat8_256 MaskAll<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 MaskAll<PackedFloat4_256>() noexcept;
}
