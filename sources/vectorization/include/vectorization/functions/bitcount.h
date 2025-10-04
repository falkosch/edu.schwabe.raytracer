#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  ASizeT bitCount(bool value) noexcept;

  ASizeT bitCount(Int_8 value) noexcept;
  ASizeT bitCount(UInt_8 value) noexcept;

  ASizeT bitCount(Int_16 value) noexcept;
  ASizeT bitCount(UInt_16 value) noexcept;

  ASizeT bitCount(Int_32 value) noexcept;
  ASizeT bitCount(UInt_32 value) noexcept;

  ASizeT bitCount(Int_64 value) noexcept;
  ASizeT bitCount(UInt_64 value) noexcept;

  ASizeT bitCount(Float_32 value) noexcept;
  ASizeT bitCount(Float_64 value) noexcept;

  ASizeT bitCount(const PackedInts_128 &values) noexcept;
  ASizeT bitCount(const PackedFloat2_128 &values) noexcept;
  ASizeT bitCount(const PackedFloat4_128 &values) noexcept;

  ASizeT bitCount(const PackedInts_256 &values) noexcept;
  ASizeT bitCount(const PackedFloat8_256 &values) noexcept;
  ASizeT bitCount(const PackedFloat4_256 &values) noexcept;
}
