#pragma once

#include "../architecture.h"

namespace vectorization
{
    const bool select(const bool mask, const bool onTrue, const bool onFalse) noexcept;

    const Int_8 select(const bool mask, const Int_8 onTrue, const Int_8 onFalse) noexcept;
    const Int_8 select(const Int_8 mask, const Int_8 onTrue, const Int_8 onFalse) noexcept;

    const UInt_8 select(const bool mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept;
    const UInt_8 select(const UInt_8 mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept;

    const Int_16 select(const bool mask, const Int_16 onTrue, const Int_16 onFalse) noexcept;
    const Int_16 select(const Int_16 mask, const Int_16 onTrue, const Int_16 onFalse) noexcept;

    const UInt_16 select(const bool mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept;
    const UInt_16 select(const UInt_16 mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept;

    const Int_32 select(const bool mask, const Int_32 onTrue, const Int_32 onFalse) noexcept;
    const Int_32 select(const Int_32 mask, const Int_32 onTrue, const Int_32 onFalse) noexcept;

    const UInt_32 select(const bool mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept;
    const UInt_32 select(const UInt_32 mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept;

    const Int_64 select(const bool mask, const Int_64 onTrue, const Int_64 onFalse) noexcept;
    const Int_64 select(const Int_64 mask, const Int_64 onTrue, const Int_64 onFalse) noexcept;

    const UInt_64 select(const bool mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept;
    const UInt_64 select(const UInt_64 mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept;

    const Float_32 select(const bool mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;
    const Float_32 select(const BoolTypes<Float_32>::Type mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;

    const Float_64 select(const bool mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;
    const Float_64 select(const BoolTypes<Float_64>::Type mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;
}
