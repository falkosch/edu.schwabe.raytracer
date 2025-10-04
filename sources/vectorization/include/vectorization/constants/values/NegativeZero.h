#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T NegativeZero() noexcept;

  template <>
  Float_32 NegativeZero<Float_32>() noexcept;

  template <>
  Float_64 NegativeZero<Float_64>() noexcept;

  template <>
  PackedFloat4_128 NegativeZero<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 NegativeZero<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 NegativeZero<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 NegativeZero<PackedFloat4_256>() noexcept;
}
