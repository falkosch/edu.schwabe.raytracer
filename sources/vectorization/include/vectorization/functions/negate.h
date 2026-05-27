#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 negate(Float_32 value) noexcept;
  Float_64 negate(Float_64 value) noexcept;

  PackedFloat4_128 negate(const PackedFloat4_128 &v) noexcept;
  PackedFloat2_128 negate(const PackedFloat2_128 &v) noexcept;

  PackedFloat8_256 negate(const PackedFloat8_256 &v) noexcept;
  PackedFloat4_256 negate(const PackedFloat4_256 &v) noexcept;
}
