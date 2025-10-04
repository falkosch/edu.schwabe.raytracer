#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 fract(Float_32 value) noexcept;
  Float_64 fract(Float_64 value) noexcept;

  PackedFloat4_128 fract(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 fract(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 fract(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 fract(const PackedFloat4_256 &values) noexcept;
}
