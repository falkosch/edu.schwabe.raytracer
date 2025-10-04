#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskZW() noexcept;

  template <>
  PackedFloat4_128 MaskZW<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat8_256 MaskZW<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskZW<PackedFloat4_256>() noexcept;
}
