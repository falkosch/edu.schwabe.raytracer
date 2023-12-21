#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  const V MaskYZ() noexcept;

  template <>
  const PackedFloat4_128 MaskYZ<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat8_256 MaskYZ<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 MaskYZ<PackedFloat4_256>() noexcept;
}
