#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 floor(Float_32 value) noexcept;
  Float_64 floor(Float_64 value) noexcept;

  PackedFloat4_128 floor(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 floor(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 floor(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 floor(const PackedFloat4_256 &values) noexcept;
}
