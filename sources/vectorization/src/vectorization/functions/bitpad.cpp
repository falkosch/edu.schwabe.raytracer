#include "vectorization/functions/bitpad.h"

namespace vectorization
{
    const Int_8 bitpad(const Int_8 unpadded, const Int_8 bitPadding) noexcept {
        return static_cast<Int_8>((bitPadding + unpadded) & ~bitPadding);
    }

    const UInt_8 bitpad(const UInt_8 unpadded, const UInt_8 bitPadding) noexcept {
        return static_cast<UInt_8>((bitPadding + unpadded) & ~bitPadding);
    }

    const Int_16 bitpad(const Int_16 unpadded, const Int_16 bitPadding) noexcept {
        return static_cast<Int_16>((bitPadding + unpadded) & ~bitPadding);
    }

    const UInt_16 bitpad(const UInt_16 unpadded, const UInt_16 bitPadding) noexcept {
        return static_cast<UInt_16>((bitPadding + unpadded) & ~bitPadding);
    }

    const Int_32 bitpad(const Int_32 unpadded, const Int_32 bitPadding) noexcept {
        return (bitPadding + unpadded) & ~bitPadding;
    }

    const UInt_32 bitpad(const UInt_32 unpadded, const UInt_32 bitPadding) noexcept {
        return (bitPadding + unpadded) & ~bitPadding;
    }

    const Int_64 bitpad(const Int_64 unpadded, const Int_64 bitPadding) noexcept {
        return (bitPadding + unpadded) & ~bitPadding;
    }

    const UInt_64 bitpad(const UInt_64 unpadded, const UInt_64 bitPadding) noexcept {
        return (bitPadding + unpadded) & ~bitPadding;
    }
}