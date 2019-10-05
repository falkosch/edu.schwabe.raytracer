#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{
    const Int_8 divide(const Int_8 a, const Int_8 b) noexcept;

    const UInt_8 divide(const UInt_8 a, const UInt_8 b) noexcept;

    const Int_16 divide(const Int_16 a, const Int_16 b) noexcept;

    const UInt_16 divide(const UInt_16 a, const UInt_16 b) noexcept;

    const Int_32 divide(const Int_32 a, const Int_32 b) noexcept;

    const UInt_32 divide(const UInt_32 a, const UInt_32 b) noexcept;

    const Int_64 divide(const Int_64 a, const Int_64 b) noexcept;

    const UInt_64 divide(const UInt_64 a, const UInt_64 b) noexcept;

    const Float_32 divide(const Float_32 a, const Float_32 b) noexcept;

    const Float_64 divide(const Float_64 a, const Float_64 b) noexcept;

    const PackedFloat4_128 divide(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

    const PackedFloat2_128 divide(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    const PackedFloat8_256 divide(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

    const PackedFloat4_256 divide(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
}
