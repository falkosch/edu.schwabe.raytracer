#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T RadianToUniform() noexcept;

  template <>
  Float_32 RadianToUniform<Float_32>() noexcept;

  template <>
  Float_64 RadianToUniform<Float_64>() noexcept;

  template <>
  PackedFloat4_128 RadianToUniform<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 RadianToUniform<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 RadianToUniform<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 RadianToUniform<PackedFloat4_256>() noexcept;
}
