#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskXY() noexcept;

  template <>
  PackedFloat4_128 MaskXY<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 MaskXY<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 MaskXY<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskXY<PackedFloat4_256>() noexcept;
}
