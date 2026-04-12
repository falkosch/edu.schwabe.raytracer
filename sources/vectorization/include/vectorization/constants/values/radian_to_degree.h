#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T RadianToDegree() noexcept;

  template <>
  Float_32 RadianToDegree<Float_32>() noexcept;

  template <>
  Float_64 RadianToDegree<Float_64>() noexcept;

  template <>
  PackedFloat4_128 RadianToDegree<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 RadianToDegree<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 RadianToDegree<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 RadianToDegree<PackedFloat4_256>() noexcept;
}
