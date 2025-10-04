#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T Epsilon() noexcept;

  template <>
  Float_32 Epsilon<Float_32>() noexcept;

  template <>
  Float_64 Epsilon<Float_64>() noexcept;

  template <>
  PackedFloat4_128 Epsilon<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Epsilon<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 Epsilon<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Epsilon<PackedFloat4_256>() noexcept;
}
