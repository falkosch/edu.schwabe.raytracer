#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T DegreeToRadian() noexcept;

  template <>
  Float_32 DegreeToRadian<Float_32>() noexcept;

  template <>
  Float_64 DegreeToRadian<Float_64>() noexcept;

  template <>
  PackedFloat4_128 DegreeToRadian<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 DegreeToRadian<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 DegreeToRadian<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 DegreeToRadian<PackedFloat4_256>() noexcept;
}
