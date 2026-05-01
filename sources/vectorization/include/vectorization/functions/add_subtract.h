#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // Alternately subtracts and adds: even-indexed components are subtracted, odd-indexed components are added.
  // 128s: {a[0]-b[0], a[1]+b[1], a[2]-b[2], a[3]+b[3]}
  // 128d: {a[0]-b[0], a[1]+b[1]}
  PackedFloat4_128 addSubtract(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 addSubtract(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 addSubtract(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 addSubtract(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
