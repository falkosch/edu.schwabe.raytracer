#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Float_32 mix(const Float_32 x, const Float_32 y, const Float_32 factor) noexcept;

  const Float_64 mix(const Float_64 x, const Float_64 y, const Float_64 factor) noexcept;

  const PackedFloat4_128 mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const Float_32 factor) noexcept;

  const PackedFloat4_128
  mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const PackedFloat4_128 &factor) noexcept;

  const PackedFloat2_128 mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const Float_64 factor) noexcept;

  const PackedFloat2_128
  mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const PackedFloat2_128 &factor) noexcept;

  const PackedFloat8_256 mix(const PackedFloat8_256 &x, const PackedFloat8_256 &y, const Float_32 factor) noexcept;

  const PackedFloat8_256
  mix(const PackedFloat8_256 &x, const PackedFloat8_256 &y, const PackedFloat8_256 &factor) noexcept;

  const PackedFloat4_256 mix(const PackedFloat4_256 &x, const PackedFloat4_256 &y, const Float_64 factor) noexcept;

  const PackedFloat4_256
  mix(const PackedFloat4_256 &x, const PackedFloat4_256 &y, const PackedFloat4_256 &factor) noexcept;
}
