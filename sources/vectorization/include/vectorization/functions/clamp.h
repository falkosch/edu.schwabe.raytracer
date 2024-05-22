#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  bool clamp(bool value, bool lower, bool upper) noexcept;

  Int_8 clamp(Int_8 value, Int_8 lower, Int_8 upper) noexcept;
  UInt_8 clamp(UInt_8 value, UInt_8 lower, UInt_8 upper) noexcept;

  Int_16 clamp(Int_16 value, Int_16 lower, Int_16 upper) noexcept;
  UInt_16 clamp(UInt_16 value, UInt_16 lower, UInt_16 upper) noexcept;

  Int_32 clamp(Int_32 value, Int_32 lower, Int_32 upper) noexcept;
  UInt_32 clamp(UInt_32 value, UInt_32 lower, UInt_32 upper) noexcept;

  Int_64 clamp(Int_64 value, Int_64 lower, Int_64 upper) noexcept;
  UInt_64 clamp(UInt_64 value, UInt_64 lower, UInt_64 upper) noexcept;

  Float_32 clamp(Float_32 value, Float_32 lower, Float_32 upper) noexcept;
  Float_64 clamp(Float_64 value, Float_64 lower, Float_64 upper) noexcept;

  PackedFloat4_128
  clamp(const PackedFloat4_128 &values, const PackedFloat4_128 &lower, const PackedFloat4_128 &upper) noexcept;
  PackedFloat2_128
  clamp(const PackedFloat2_128 &values, const PackedFloat2_128 &lower, const PackedFloat2_128 &upper) noexcept;

  PackedFloat8_256
  clamp(const PackedFloat8_256 &values, const PackedFloat8_256 &lower, const PackedFloat8_256 &upper) noexcept;
  PackedFloat4_256
  clamp(const PackedFloat4_256 &values, const PackedFloat4_256 &lower, const PackedFloat4_256 &upper) noexcept;
}
