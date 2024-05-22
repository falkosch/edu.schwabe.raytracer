#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_8 sign(Int_8 value) noexcept;
  Int_16 sign(Int_16 value) noexcept;
  Int_32 sign(Int_32 value) noexcept;
  Int_64 sign(Int_64 value) noexcept;

  Float_32 sign(Float_32 value) noexcept;
  Float_64 sign(Float_64 value) noexcept;

  PackedFloat4_128 sign(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 sign(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 sign(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 sign(const PackedFloat4_256 &values) noexcept;
}
