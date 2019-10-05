#pragma once

#include "../architecture/meta_bool_types.h"

namespace vectorization
{
    template <typename T>
    inline constexpr T select(const bool mask, const T & onTrue, const T & onFalse) noexcept {
        return *(mask ? &onTrue : &onFalse);
    }

    template <typename T, bool B>
    struct BoolSelector;

    template <typename T>
    struct BoolSelector < T, false >
    {
        inline constexpr T & operator()(const T & onTrue, const T & onFalse) noexcept {
            return onFalse;
        }
    };

    template <typename T>
    struct BoolSelector < T, true >
    {
        inline constexpr T & operator()(const T & onTrue, const T & onFalse) noexcept {
            return onTrue;
        }
    };

    const bool select(const bool mask, const bool onTrue, const bool onFalse) noexcept;

    const Int_8 select(const Int_8 mask, const Int_8 onTrue, const Int_8 onFalse) noexcept;
    const UInt_8 select(const UInt_8 mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept;

    const Int_16 select(const Int_16 mask, const Int_16 onTrue, const Int_16 onFalse) noexcept;
    const UInt_16 select(const UInt_16 mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept;

    const Int_32 select(const Int_32 mask, const Int_32 onTrue, const Int_32 onFalse) noexcept;
    const UInt_32 select(const UInt_32 mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept;

    const Int_64 select(const Int_64 mask, const Int_64 onTrue, const Int_64 onFalse) noexcept;
    const UInt_64 select(const UInt_64 mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept;

    const Float_32 select(const BoolTypes<Float_32>::Type mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;
    const Float_64 select(const BoolTypes<Float_64>::Type mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;

    const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;
    const PackedFloat2_128 select(const PackedFloat2_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;

    const long select(const bool mask, const long onTrue, const long onFalse) noexcept;
    const unsigned long select(const bool mask, const unsigned long onTrue, const unsigned long onFalse) noexcept;

    const Int_8 select(const bool mask, const Int_8 onTrue, const Int_8 onFalse) noexcept;
    const UInt_8 select(const bool mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept;

    const Int_16 select(const bool mask, const Int_16 onTrue, const Int_16 onFalse) noexcept;
    const UInt_16 select(const bool mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept;

    const Int_32 select(const bool mask, const Int_32 onTrue, const Int_32 onFalse) noexcept;
    const UInt_32 select(const bool mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept;

    const Int_64 select(const bool mask, const Int_64 onTrue, const Int_64 onFalse) noexcept;
    const UInt_64 select(const bool mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept;

    const Float_32 select(const bool mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;
    const Float_64 select(const bool mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;

    const PackedInts_128 select(const bool mask, const PackedInts_128 & onTrue, const PackedInts_128 & onFalse) noexcept;
    const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;
    const PackedFloat2_128 select(const bool mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;
}
