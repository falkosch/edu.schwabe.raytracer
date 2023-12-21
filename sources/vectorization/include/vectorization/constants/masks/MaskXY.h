#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  const V MaskXY() noexcept;

  template <>
  const PackedFloat4_128 MaskXY<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 MaskXY<PackedFloat2_128>() noexcept;

  template <>
  const PackedFloat8_256 MaskXY<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 MaskXY<PackedFloat4_256>() noexcept;
}
