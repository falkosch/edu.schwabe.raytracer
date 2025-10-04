#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 sqrt(Float_32 value) noexcept;
  Float_64 sqrt(Float_64 value) noexcept;

  PackedFloat4_128 sqrt(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 sqrt(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 sqrt(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 sqrt(const PackedFloat4_256 &values) noexcept;
}
