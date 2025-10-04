#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  bool select(bool mask, bool onTrue, bool onFalse) noexcept;

  Int_8 select(bool mask, Int_8 onTrue, Int_8 onFalse) noexcept;
  Int_8 select(Int_8 mask, Int_8 onTrue, Int_8 onFalse) noexcept;

  UInt_8 select(bool mask, UInt_8 onTrue, UInt_8 onFalse) noexcept;
  UInt_8 select(UInt_8 mask, UInt_8 onTrue, UInt_8 onFalse) noexcept;

  Int_16 select(bool mask, Int_16 onTrue, Int_16 onFalse) noexcept;
  Int_16 select(Int_16 mask, Int_16 onTrue, Int_16 onFalse) noexcept;

  UInt_16 select(bool mask, UInt_16 onTrue, UInt_16 onFalse) noexcept;
  UInt_16 select(UInt_16 mask, UInt_16 onTrue, UInt_16 onFalse) noexcept;

  Int_32 select(bool mask, Int_32 onTrue, Int_32 onFalse) noexcept;
  Int_32 select(Int_32 mask, Int_32 onTrue, Int_32 onFalse) noexcept;

  UInt_32 select(bool mask, UInt_32 onTrue, UInt_32 onFalse) noexcept;
  UInt_32 select(UInt_32 mask, UInt_32 onTrue, UInt_32 onFalse) noexcept;

  Int_64 select(bool mask, Int_64 onTrue, Int_64 onFalse) noexcept;
  Int_64 select(Int_64 mask, Int_64 onTrue, Int_64 onFalse) noexcept;

  UInt_64 select(bool mask, UInt_64 onTrue, UInt_64 onFalse) noexcept;
  UInt_64 select(UInt_64 mask, UInt_64 onTrue, UInt_64 onFalse) noexcept;

  Float_32 select(bool mask, Float_32 onTrue, Float_32 onFalse) noexcept;
  Float_32 select(BoolTypes<Float_32>::Type mask, Float_32 onTrue, Float_32 onFalse) noexcept;

  Float_64 select(bool mask, Float_64 onTrue, Float_64 onFalse) noexcept;
  Float_64 select(BoolTypes<Float_64>::Type mask, Float_64 onTrue, Float_64 onFalse) noexcept;
}
