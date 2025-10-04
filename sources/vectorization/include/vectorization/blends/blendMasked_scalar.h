#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  bool blendMasked(bool onBitNotSet, bool onBitSet, bool mask) noexcept;

  Int_8 blendMasked(Int_8 onBitNotSet, Int_8 onBitSet, Int_8 mask) noexcept;
  UInt_8 blendMasked(UInt_8 onBitNotSet, UInt_8 onBitSet, UInt_8 mask) noexcept;

  Int_16 blendMasked(Int_16 onBitNotSet, Int_16 onBitSet, Int_16 mask) noexcept;
  UInt_16 blendMasked(UInt_16 onBitNotSet, UInt_16 onBitSet, UInt_16 mask) noexcept;

  Int_32 blendMasked(Int_32 onBitNotSet, Int_32 onBitSet, Int_32 mask) noexcept;
  UInt_32 blendMasked(UInt_32 onBitNotSet, UInt_32 onBitSet, UInt_32 mask) noexcept;

  Int_64 blendMasked(Int_64 onBitNotSet, Int_64 onBitSet, Int_64 mask) noexcept;
  UInt_64 blendMasked(UInt_64 onBitNotSet, UInt_64 onBitSet, UInt_64 mask) noexcept;

  Float_32 blendMasked(Float_32 onBitNotSet, Float_32 onBitSet, BoolTypes<Float_32>::Type mask) noexcept;
  Float_64 blendMasked(Float_64 onBitNotSet, Float_64 onBitSet, BoolTypes<Float_64>::Type mask) noexcept;
}
