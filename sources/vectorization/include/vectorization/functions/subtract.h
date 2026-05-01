#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_128 subtract(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 subtract(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 subtract(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 subtract(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 subtractX(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 subtractX(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
}
