#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T Sin45() noexcept;

  template <>
  Float_32 Sin45<Float_32>() noexcept;

  template <>
  Float_64 Sin45<Float_64>() noexcept;

  template <>
  PackedFloat4_128 Sin45<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Sin45<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 Sin45<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Sin45<PackedFloat4_256>() noexcept;
}
