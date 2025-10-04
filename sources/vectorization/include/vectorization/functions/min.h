#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  bool min(bool a, bool b) noexcept;

  Int_8 min(Int_8 a, Int_8 b) noexcept;
  UInt_8 min(UInt_8 a, UInt_8 b) noexcept;

  Int_16 min(Int_16 a, Int_16 b) noexcept;
  UInt_16 min(UInt_16 a, UInt_16 b) noexcept;

  Int_32 min(Int_32 a, Int_32 b) noexcept;
  UInt_32 min(UInt_32 a, UInt_32 b) noexcept;

  Int_64 min(Int_64 a, Int_64 b) noexcept;
  UInt_64 min(UInt_64 a, UInt_64 b) noexcept;

  Float_32 min(Float_32 a, Float_32 b) noexcept;
  Float_64 min(Float_64 a, Float_64 b) noexcept;

  PackedFloat4_128 min(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 min(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 min(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 min(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
