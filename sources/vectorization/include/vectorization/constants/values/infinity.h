#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T Infinity() noexcept;

  template <>
  Float_32 Infinity<Float_32>() noexcept;

  template <>
  Float_64 Infinity<Float_64>() noexcept;

  template <>
  PackedFloat4_128 Infinity<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Infinity<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 Infinity<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Infinity<PackedFloat4_256>() noexcept;
}
