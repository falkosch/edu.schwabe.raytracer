#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 rsqrt(Float_32 value) noexcept;
  Float_64 rsqrt(Float_64 value) noexcept;

  PackedFloat4_128 rsqrt(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 rsqrt(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 rsqrt(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 rsqrt(const PackedFloat4_256 &values) noexcept;
}
