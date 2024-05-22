#include "vectorization/functions/bitPad.h"

namespace vectorization {
  Int_8 bitPad(const Int_8 value, const Int_8 padding) noexcept {
    return static_cast<Int_8>((padding + value) & ~padding);
  }

  UInt_8 bitPad(const UInt_8 value, const UInt_8 padding) noexcept {
    return static_cast<UInt_8>((padding + value) & ~padding);
  }

  Int_16 bitPad(const Int_16 value, const Int_16 padding) noexcept {
    return static_cast<Int_16>((padding + value) & ~padding);
  }

  UInt_16 bitPad(const UInt_16 value, const UInt_16 padding) noexcept {
    return static_cast<UInt_16>((padding + value) & ~padding);
  }

  Int_32 bitPad(const Int_32 value, const Int_32 padding) noexcept {
    return (padding + value) & ~padding;
  }

  UInt_32 bitPad(const UInt_32 value, const UInt_32 padding) noexcept {
    return (padding + value) & ~padding;
  }

  Int_64 bitPad(const Int_64 value, const Int_64 padding) noexcept {
    return (padding + value) & ~padding;
  }

  UInt_64 bitPad(const UInt_64 value, const UInt_64 padding) noexcept {
    return (padding + value) & ~padding;
  }
}
