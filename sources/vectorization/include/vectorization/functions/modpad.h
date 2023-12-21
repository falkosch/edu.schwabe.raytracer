#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Int_8 modPad(const Int_8 unpadded, const Int_8 modPadding) noexcept;

  const UInt_8 modPad(const UInt_8 unpadded, const UInt_8 modPadding) noexcept;

  const Int_16 modPad(const Int_16 unpadded, const Int_16 modPadding) noexcept;

  const UInt_16 modPad(const UInt_16 unpadded, const UInt_16 modPadding) noexcept;

  const Int_32 modPad(const Int_32 unpadded, const Int_32 modPadding) noexcept;

  const UInt_32 modPad(const UInt_32 unpadded, const UInt_32 modPadding) noexcept;

  const Int_64 modPad(const Int_64 unpadded, const Int_64 modPadding) noexcept;

  const UInt_64 modPad(const UInt_64 unpadded, const UInt_64 modPadding) noexcept;
}
