#pragma once

#include "vectorization/architecture.h"

#define C_PI 3.1415926535897932384626433832795

namespace vectorization {
  template <typename T>
  T Pi() noexcept;

  template <>
  Float_32 Pi<Float_32>() noexcept;

  template <>
  Float_64 Pi<Float_64>() noexcept;

  template <>
  PackedFloat4_128 Pi<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Pi<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 Pi<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Pi<PackedFloat4_256>() noexcept;
}
