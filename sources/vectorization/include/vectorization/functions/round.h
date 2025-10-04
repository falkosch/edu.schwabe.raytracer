#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 round(Float_32 value) noexcept;
  Float_64 round(Float_64 value) noexcept;

  PackedFloat4_128 round(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 round(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 round(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 round(const PackedFloat4_256 &values) noexcept;
}
