#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T NegativeInfinity() noexcept;

  template <>
  Float_32 NegativeInfinity<Float_32>() noexcept;

  template <>
  Float_64 NegativeInfinity<Float_64>() noexcept;

  template <>
  PackedFloat4_128 NegativeInfinity<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 NegativeInfinity<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 NegativeInfinity<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 NegativeInfinity<PackedFloat4_256>() noexcept;
}
