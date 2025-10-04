#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskZ() noexcept;

  template <>
  PackedFloat4_128 MaskZ<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskZ<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskZ<PackedFloat4_256>() noexcept;
}
