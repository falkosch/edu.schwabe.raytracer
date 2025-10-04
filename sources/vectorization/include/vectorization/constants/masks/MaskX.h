#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  const V MaskX() noexcept;

  template <>
  const PackedFloat4_128 MaskX<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 MaskX<PackedFloat2_128>() noexcept;

  template <>
  const PackedFloat8_256 MaskX<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 MaskX<PackedFloat4_256>() noexcept;
}
