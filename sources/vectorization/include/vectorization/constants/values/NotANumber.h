#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  const T NotANumber() noexcept;

  template <>
  const Float_32 NotANumber<Float_32>() noexcept;

  template <>
  const Float_64 NotANumber<Float_64>() noexcept;

  template <>
  const PackedFloat4_128 NotANumber<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 NotANumber<PackedFloat2_128>() noexcept;

  template <>
  const PackedFloat8_256 NotANumber<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 NotANumber<PackedFloat4_256>() noexcept;
}
