#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 reciprocal(Float_32 value) noexcept;
  Float_64 reciprocal(Float_64 value) noexcept;

  PackedFloat4_128 reciprocal(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 reciprocal(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 reciprocal(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 reciprocal(const PackedFloat4_256 &values) noexcept;
}
