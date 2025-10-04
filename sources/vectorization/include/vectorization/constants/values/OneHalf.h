#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T OneHalf() noexcept;

  template <>
  Float_32 OneHalf<Float_32>() noexcept;

  template <>
  Float_64 OneHalf<Float_64>() noexcept;

  template <>
  PackedFloat4_128 OneHalf<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 OneHalf<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 OneHalf<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 OneHalf<PackedFloat4_256>() noexcept;
}
