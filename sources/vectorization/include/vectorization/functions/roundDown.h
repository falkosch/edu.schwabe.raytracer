#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_8 roundDown(Int_8 value, Int_8 stepSize) noexcept;
  UInt_8 roundDown(UInt_8 value, UInt_8 stepSize) noexcept;

  Int_16 roundDown(Int_16 value, Int_16 stepSize) noexcept;
  UInt_16 roundDown(UInt_16 value, UInt_16 stepSize) noexcept;

  Int_32 roundDown(Int_32 value, Int_32 stepSize) noexcept;
  UInt_32 roundDown(UInt_32 value, UInt_32 stepSize) noexcept;

  Int_64 roundDown(Int_64 value, Int_64 stepSize) noexcept;
  UInt_64 roundDown(UInt_64 value, UInt_64 stepSize) noexcept;
}
