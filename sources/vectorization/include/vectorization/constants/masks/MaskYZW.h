#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  const V MaskYZW() noexcept;

  template <>
  const PackedFloat4_128 MaskYZW<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat8_256 MaskYZW<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 MaskYZW<PackedFloat4_256>() noexcept;
}
