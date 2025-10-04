#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskYZ() noexcept;

  template <>
  PackedFloat4_128 MaskYZ<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskYZ<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskYZ<PackedFloat4_256>() noexcept;
}
