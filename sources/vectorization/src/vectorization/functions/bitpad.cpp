#include "vectorization/functions/bitPad.h"

namespace vectorization {
  const Int_8 bitPad(const Int_8 unpadded, const Int_8 bitPadding) noexcept {
    return static_cast<Int_8>((bitPadding + unpadded) & ~bitPadding);
  }

  const UInt_8 bitPad(const UInt_8 unpadded, const UInt_8 bitPadding) noexcept {
    return static_cast<UInt_8>((bitPadding + unpadded) & ~bitPadding);
  }

  const Int_16 bitPad(const Int_16 unpadded, const Int_16 bitPadding) noexcept {
    return static_cast<Int_16>((bitPadding + unpadded) & ~bitPadding);
  }

  const UInt_16 bitPad(const UInt_16 unpadded, const UInt_16 bitPadding) noexcept {
    return static_cast<UInt_16>((bitPadding + unpadded) & ~bitPadding);
  }

  const Int_32 bitPad(const Int_32 unpadded, const Int_32 bitPadding) noexcept {
    return (bitPadding + unpadded) & ~bitPadding;
  }

  const UInt_32 bitPad(const UInt_32 unpadded, const UInt_32 bitPadding) noexcept {
    return (bitPadding + unpadded) & ~bitPadding;
  }

  const Int_64 bitPad(const Int_64 unpadded, const Int_64 bitPadding) noexcept {
    return (bitPadding + unpadded) & ~bitPadding;
  }

  const UInt_64 bitPad(const UInt_64 unpadded, const UInt_64 bitPadding) noexcept {
    return (bitPadding + unpadded) & ~bitPadding;
  }
}