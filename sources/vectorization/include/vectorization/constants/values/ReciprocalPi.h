#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T ReciprocalPi() noexcept;

  template <>
  Float_32 ReciprocalPi<Float_32>() noexcept;

  template <>
  Float_64 ReciprocalPi<Float_64>() noexcept;

  template <>
  PackedFloat4_128 ReciprocalPi<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 ReciprocalPi<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 ReciprocalPi<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 ReciprocalPi<PackedFloat4_256>() noexcept;
}
