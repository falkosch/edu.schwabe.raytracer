#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T SelfOcclusionEpsilon() noexcept;

  template <>
  Float_32 SelfOcclusionEpsilon<Float_32>() noexcept;

  template <>
  Float_64 SelfOcclusionEpsilon<Float_64>() noexcept;

  template <>
  PackedFloat4_128 SelfOcclusionEpsilon<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 SelfOcclusionEpsilon<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 SelfOcclusionEpsilon<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 SelfOcclusionEpsilon<PackedFloat4_256>() noexcept;
}
