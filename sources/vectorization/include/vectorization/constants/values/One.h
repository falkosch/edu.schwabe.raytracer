#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T One() noexcept;

    template <>
    const bool One<bool>() noexcept;

    template <>
    const Int_8 One<Int_8>() noexcept;

    template <>
    const UInt_8 One<UInt_8>() noexcept;

    template <>
    const Int_16 One<Int_16>() noexcept;

    template <>
    const UInt_16 One<UInt_16>() noexcept;

    template <>
    const Int_32 One<Int_32>() noexcept;

    template <>
    const UInt_32 One<UInt_32>() noexcept;

    template <>
    const Int_64 One<Int_64>() noexcept;

    template <>
    const UInt_64 One<UInt_64>() noexcept;

    template <>
    const Float_32 One<Float_32>() noexcept;

    template <>
    const Float_64 One<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 One<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 One<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 One<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 One<PackedFloat4_256>() noexcept;
}
