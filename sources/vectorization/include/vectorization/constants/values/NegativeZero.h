#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  const T NegativeZero() noexcept;

  template <>
  const Float_32 NegativeZero<Float_32>() noexcept;

  template <>
  const Float_64 NegativeZero<Float_64>() noexcept;

  template <>
  const PackedFloat4_128 NegativeZero<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 NegativeZero<PackedFloat2_128>() noexcept;

  template <>
  const PackedFloat8_256 NegativeZero<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 NegativeZero<PackedFloat4_256>() noexcept;
}
