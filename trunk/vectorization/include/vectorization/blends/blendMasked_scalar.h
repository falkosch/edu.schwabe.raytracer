#pragma once

#include "../architecture.h"

namespace vectorization
{
    const bool blendMasked(const bool onBitNotSet, const bool onBitSet, const bool mask) noexcept;

    const Int_8 blendMasked(const Int_8 onBitNotSet, const Int_8 onBitSet, const Int_8 mask) noexcept;
    const UInt_8 blendMasked(const UInt_8 onBitNotSet, const UInt_8 onBitSet, const UInt_8 mask) noexcept;

    const Int_16 blendMasked(const Int_16 onBitNotSet, const Int_16 onBitSet, const Int_16 mask) noexcept;
    const UInt_16 blendMasked(const UInt_16 onBitNotSet, const UInt_16 onBitSet, const UInt_16 mask) noexcept;

    const Int_32 blendMasked(const Int_32 onBitNotSet, const Int_32 onBitSet, const Int_32 mask) noexcept;
    const UInt_32 blendMasked(const UInt_32 onBitNotSet, const UInt_32 onBitSet, const UInt_32 mask) noexcept;

    const Int_64 blendMasked(const Int_64 onBitNotSet, const Int_64 onBitSet, const Int_64 mask) noexcept;
    const UInt_64 blendMasked(const UInt_64 onBitNotSet, const UInt_64 onBitSet, const UInt_64 mask) noexcept;

    const Float_32 blendMasked(const Float_32 onBitNotSet, const Float_32 onBitSet, const BoolTypes<Float_32>::Type mask) noexcept;
    const Float_64 blendMasked(const Float_64 onBitNotSet, const Float_64 onBitSet, const BoolTypes<Float_64>::Type mask) noexcept;
}
