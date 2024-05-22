#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 ceil(Float_32 value) noexcept;
  Float_64 ceil(Float_64 value) noexcept;

  PackedFloat4_128 ceil(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 ceil(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 ceil(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 ceil(const PackedFloat4_256 &values) noexcept;
}
