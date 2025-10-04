#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskY() noexcept;

  template <>
  PackedFloat4_128 MaskY<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 MaskY<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 MaskY<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskY<PackedFloat4_256>() noexcept;
}
