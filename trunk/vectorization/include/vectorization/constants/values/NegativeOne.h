#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename T>
    const T NegativeOne() noexcept;

    template <>
    const Int_8 NegativeOne<Int_8>() noexcept;

    template <>
    const Int_16 NegativeOne<Int_16>() noexcept;

    template <>
    const Int_32 NegativeOne<Int_32>() noexcept;

    template <>
    const Int_64 NegativeOne<Int_64>() noexcept;

    template <>
    const Float_32 NegativeOne<Float_32>() noexcept;

    template <>
    const Float_64 NegativeOne<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 NegativeOne<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 NegativeOne<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 NegativeOne<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 NegativeOne<PackedFloat4_256>() noexcept;
}
