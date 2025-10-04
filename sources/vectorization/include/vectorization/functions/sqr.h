#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Float_32 sqr(const Float_32 v) noexcept;

  const Float_64 sqr(const Float_64 v) noexcept;

  const PackedFloat4_128 sqr(const PackedFloat4_128 &v) noexcept;

  const PackedFloat2_128 sqr(const PackedFloat2_128 &v) noexcept;

  const PackedFloat8_256 sqr(const PackedFloat8_256 &v) noexcept;

  const PackedFloat4_256 sqr(const PackedFloat4_256 &v) noexcept;
}
