#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Int_8 bitPad(const Int_8 unpadded, const Int_8 bitPadding) noexcept;

  const UInt_8 bitPad(const UInt_8 unpadded, const UInt_8 bitPadding) noexcept;

  const Int_16 bitPad(const Int_16 unpadded, const Int_16 bitPadding) noexcept;

  const UInt_16 bitPad(const UInt_16 unpadded, const UInt_16 bitPadding) noexcept;

  const Int_32 bitPad(const Int_32 unpadded, const Int_32 bitPadding) noexcept;

  const UInt_32 bitPad(const UInt_32 unpadded, const UInt_32 bitPadding) noexcept;

  const Int_64 bitPad(const Int_64 unpadded, const Int_64 bitPadding) noexcept;

  const UInt_64 bitPad(const UInt_64 unpadded, const UInt_64 bitPadding) noexcept;
}
