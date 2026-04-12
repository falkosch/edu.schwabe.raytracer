#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskXYZ() noexcept;

  template <>
  PackedFloat4_128 MaskXYZ<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskXYZ<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskXYZ<PackedFloat4_256>() noexcept;
}
