#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  bool max(bool a, bool b) noexcept;

  Int_8 max(Int_8 a, Int_8 b) noexcept;
  UInt_8 max(UInt_8 a, UInt_8 b) noexcept;

  Int_16 max(Int_16 a, Int_16 b) noexcept;
  UInt_16 max(UInt_16 a, UInt_16 b) noexcept;

  Int_32 max(Int_32 a, Int_32 b) noexcept;
  UInt_32 max(UInt_32 a, UInt_32 b) noexcept;

  Int_64 max(Int_64 a, Int_64 b) noexcept;
  UInt_64 max(UInt_64 a, UInt_64 b) noexcept;

  Float_32 max(Float_32 a, Float_32 b) noexcept;
  Float_64 max(Float_64 a, Float_64 b) noexcept;

  PackedFloat4_128 max(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 max(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 max(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 max(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
