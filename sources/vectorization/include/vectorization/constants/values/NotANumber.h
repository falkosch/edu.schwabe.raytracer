#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T NotANumber() noexcept;

  template <>
  Float_32 NotANumber<Float_32>() noexcept;

  template <>
  Float_64 NotANumber<Float_64>() noexcept;

  template <>
  PackedFloat4_128 NotANumber<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 NotANumber<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 NotANumber<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 NotANumber<PackedFloat4_256>() noexcept;
}
