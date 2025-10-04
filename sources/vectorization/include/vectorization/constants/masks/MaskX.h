#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskX() noexcept;

  template <>
  PackedFloat4_128 MaskX<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 MaskX<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 MaskX<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskX<PackedFloat4_256>() noexcept;
}
