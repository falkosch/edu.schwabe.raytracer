#pragma once

#include "../architecture.h"

namespace vectorization
{
    const Int_8 modulo(const Int_8 a, const Int_8 b) noexcept;

    const UInt_8 modulo(const UInt_8 a, const UInt_8 b) noexcept;

    const Int_16 modulo(const Int_16 a, const Int_16 b) noexcept;

    const UInt_16 modulo(const UInt_16 a, const UInt_16 b) noexcept;

    const Int_32 modulo(const Int_32 a, const Int_32 b) noexcept;

    const UInt_32 modulo(const UInt_32 a, const UInt_32 b) noexcept;

    const Int_64 modulo(const Int_64 a, const Int_64 b) noexcept;

    const UInt_64 modulo(const UInt_64 a, const UInt_64 b) noexcept;

    const Float_32 modulo(const Float_32 a, const Float_32 b) noexcept;

    const Float_64 modulo(const Float_64 a, const Float_64 b) noexcept;

    const PackedFloat4_128 modulo(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

    const PackedFloat2_128 modulo(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    const PackedFloat8_256 modulo(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

    const PackedFloat4_256 modulo(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
}
