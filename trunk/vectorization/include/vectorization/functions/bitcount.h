#pragma once

#include "../architecture.h"

namespace vectorization
{
    const ASizeT bitcount(const bool v) noexcept;

    const ASizeT bitcount(const Int_8 v) noexcept;

    const ASizeT bitcount(const UInt_8 v) noexcept;

    const ASizeT bitcount(const Int_16 v) noexcept;

    const ASizeT bitcount(const UInt_16 v) noexcept;

    const ASizeT bitcount(const Int_32 v) noexcept;

    const ASizeT bitcount(const UInt_32 v) noexcept;

    const ASizeT bitcount(const Int_64 v) noexcept;

    const ASizeT bitcount(const UInt_64 v) noexcept;

    const ASizeT bitcount(const Float_32 v) noexcept;

    const ASizeT bitcount(const Float_64 c) noexcept;

    const ASizeT bitcount(const PackedInts_128 & v) noexcept;

    const ASizeT bitcount(const PackedFloat2_128 & v) noexcept;

    const ASizeT bitcount(const PackedFloat4_128 & v) noexcept;

    const ASizeT bitcount(const PackedInts_256 & v) noexcept;

    const ASizeT bitcount(const PackedFloat8_256 & v) noexcept;

    const ASizeT bitcount(const PackedFloat4_256 & v) noexcept;
}
