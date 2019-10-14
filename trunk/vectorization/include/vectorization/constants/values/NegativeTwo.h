#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename T>
    const T NegativeTwo() noexcept;

    template <>
    const Int_8 NegativeTwo<Int_8>() noexcept;

    template <>
    const Int_16 NegativeTwo<Int_16>() noexcept;

    template <>
    const Int_32 NegativeTwo<Int_32>() noexcept;

    template <>
    const Int_64 NegativeTwo<Int_64>() noexcept;

    template <>
    const Float_32 NegativeTwo<Float_32>() noexcept;

    template <>
    const Float_64 NegativeTwo<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 NegativeTwo<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 NegativeTwo<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 NegativeTwo<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 NegativeTwo<PackedFloat4_256>() noexcept;
}
