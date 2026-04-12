#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskXYZW() noexcept;

  template <>
  PackedFloat4_128 MaskXYZW<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskXYZW<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskXYZW<PackedFloat4_256>() noexcept;
}
