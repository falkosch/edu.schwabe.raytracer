#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // -(a * b) - c (fused when FMA available)
  Float_32 negativeMultiplySub(Float_32 a, Float_32 b, Float_32 c) noexcept;
  Float_64 negativeMultiplySub(Float_64 a, Float_64 b, Float_64 c) noexcept;

  PackedFloat4_128
  negativeMultiplySub(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept;
  PackedFloat2_128
  negativeMultiplySub(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept;

  PackedFloat8_256
  negativeMultiplySub(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept;
  PackedFloat4_256
  negativeMultiplySub(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept;

  PackedFloat4_128
  negativeMultiplySubX(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept;
  PackedFloat2_128
  negativeMultiplySubX(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept;
}
