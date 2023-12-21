#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  const T RadianToDegree() noexcept;

  template <>
  const Float_32 RadianToDegree<Float_32>() noexcept;

  template <>
  const Float_64 RadianToDegree<Float_64>() noexcept;

  template <>
  const PackedFloat4_128 RadianToDegree<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 RadianToDegree<PackedFloat2_128>() noexcept;

  template <>
  const PackedFloat8_256 RadianToDegree<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 RadianToDegree<PackedFloat4_256>() noexcept;
}
