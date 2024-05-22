#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_8 modulo(Int_8 a, Int_8 b) noexcept;
  UInt_8 modulo(UInt_8 a, UInt_8 b) noexcept;

  Int_16 modulo(Int_16 a, Int_16 b) noexcept;
  UInt_16 modulo(UInt_16 a, UInt_16 b) noexcept;

  Int_32 modulo(Int_32 a, Int_32 b) noexcept;
  UInt_32 modulo(UInt_32 a, UInt_32 b) noexcept;

  Int_64 modulo(Int_64 a, Int_64 b) noexcept;
  UInt_64 modulo(UInt_64 a, UInt_64 b) noexcept;

  Float_32 modulo(Float_32 a, Float_32 b) noexcept;
  Float_64 modulo(Float_64 a, Float_64 b) noexcept;

  PackedFloat4_128 modulo(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 modulo(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 modulo(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 modulo(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
