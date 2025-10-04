#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_8 bitPad(Int_8 value, Int_8 padding) noexcept;
  UInt_8 bitPad(UInt_8 value, UInt_8 padding) noexcept;

  Int_16 bitPad(Int_16 value, Int_16 padding) noexcept;
  UInt_16 bitPad(UInt_16 value, UInt_16 padding) noexcept;

  Int_32 bitPad(Int_32 value, Int_32 padding) noexcept;
  UInt_32 bitPad(UInt_32 value, UInt_32 padding) noexcept;

  Int_64 bitPad(Int_64 value, Int_64 padding) noexcept;
  UInt_64 bitPad(UInt_64 value, UInt_64 padding) noexcept;
}
