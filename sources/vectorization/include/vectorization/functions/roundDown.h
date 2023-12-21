#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Int_8 roundDown(const Int_8 value, const Int_8 stepSize) noexcept;

  const UInt_8 roundDown(const UInt_8 value, const UInt_8 stepSize) noexcept;

  const Int_16 roundDown(const Int_16 value, const Int_16 stepSize) noexcept;

  const UInt_16 roundDown(const UInt_16 value, const UInt_16 stepSize) noexcept;

  const Int_32 roundDown(const Int_32 value, const Int_32 stepSize) noexcept;

  const UInt_32 roundDown(const UInt_32 value, const UInt_32 stepSize) noexcept;

  const Int_64 roundDown(const Int_64 value, const Int_64 stepSize) noexcept;

  const UInt_64 roundDown(const UInt_64 value, const UInt_64 stepSize) noexcept;
}
