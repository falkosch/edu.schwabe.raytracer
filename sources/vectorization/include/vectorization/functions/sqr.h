#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 sqr(Float_32 value) noexcept;
  Float_64 sqr(Float_64 value) noexcept;

  PackedFloat4_128 sqr(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 sqr(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 sqr(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 sqr(const PackedFloat4_256 &values) noexcept;
}
