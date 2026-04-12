#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskW() noexcept;

  template <>
  PackedFloat4_128 MaskW<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskW<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskW<PackedFloat4_256>() noexcept;
}
