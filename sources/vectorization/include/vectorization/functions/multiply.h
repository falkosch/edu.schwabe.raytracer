#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_128 multiply(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 multiply(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 multiply(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 multiply(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 multiplyX(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 multiplyX(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
}
