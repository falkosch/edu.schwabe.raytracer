#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  const V MaskXYZ() noexcept;

  template <>
  const PackedFloat4_128 MaskXYZ<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat8_256 MaskXYZ<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 MaskXYZ<PackedFloat4_256>() noexcept;
}
