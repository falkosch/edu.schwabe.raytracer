#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskYZW() noexcept;

  template <>
  PackedFloat4_128 MaskYZW<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskYZW<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskYZW<PackedFloat4_256>() noexcept;
}
