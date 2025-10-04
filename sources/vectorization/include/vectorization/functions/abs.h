#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_8 abs(Int_8 value) noexcept;
  Int_16 abs(Int_16 value) noexcept;
  Int_32 abs(Int_32 value) noexcept;
  Int_64 abs(Int_64 value) noexcept;

  Float_32 abs(Float_32 value) noexcept;
  Float_64 abs(Float_64 value) noexcept;

  PackedFloat4_128 abs(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 abs(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 abs(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 abs(const PackedFloat4_256 &values) noexcept;
}
