#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Float_32 sqrt(const Float_32 v) noexcept;

  const Float_64 sqrt(const Float_64 v) noexcept;

  const PackedFloat4_128 sqrt(const PackedFloat4_128 &v) noexcept;

  const PackedFloat2_128 sqrt(const PackedFloat2_128 &v) noexcept;

  const PackedFloat8_256 sqrt(const PackedFloat8_256 &v) noexcept;

  const PackedFloat4_256 sqrt(const PackedFloat4_256 &v) noexcept;
}
