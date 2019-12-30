#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    const Int_8 bitpad(const Int_8 unpadded, const Int_8 bitPadding) noexcept;

    const UInt_8 bitpad(const UInt_8 unpadded, const UInt_8 bitPadding) noexcept;

    const Int_16 bitpad(const Int_16 unpadded, const Int_16 bitPadding) noexcept;

    const UInt_16 bitpad(const UInt_16 unpadded, const UInt_16 bitPadding) noexcept;

    const Int_32 bitpad(const Int_32 unpadded, const Int_32 bitPadding) noexcept;

    const UInt_32 bitpad(const UInt_32 unpadded, const UInt_32 bitPadding) noexcept;

    const Int_64 bitpad(const Int_64 unpadded, const Int_64 bitPadding) noexcept;

    const UInt_64 bitpad(const UInt_64 unpadded, const UInt_64 bitPadding) noexcept;
}
