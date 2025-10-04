#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  const V MaskZ() noexcept;

  template <>
  const PackedFloat4_128 MaskZ<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat8_256 MaskZ<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 MaskZ<PackedFloat4_256>() noexcept;
}
