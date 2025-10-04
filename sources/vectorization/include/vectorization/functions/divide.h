#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_8 divide(Int_8 a, Int_8 b) noexcept;
  UInt_8 divide(UInt_8 a, UInt_8 b) noexcept;

  Int_16 divide(Int_16 a, Int_16 b) noexcept;
  UInt_16 divide(UInt_16 a, UInt_16 b) noexcept;

  Int_32 divide(Int_32 a, Int_32 b) noexcept;
  UInt_32 divide(UInt_32 a, UInt_32 b) noexcept;

  Int_64 divide(Int_64 a, Int_64 b) noexcept;
  UInt_64 divide(UInt_64 a, UInt_64 b) noexcept;

  Float_32 divide(Float_32 a, Float_32 b) noexcept;
  Float_64 divide(Float_64 a, Float_64 b) noexcept;

  PackedFloat4_128 divide(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 divide(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 divide(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 divide(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
