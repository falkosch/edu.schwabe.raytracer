#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 mix(Float_32 x, Float_32 y, Float_32 factor) noexcept;
  Float_64 mix(Float_64 x, Float_64 y, Float_64 factor) noexcept;

  PackedFloat4_128 mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, Float_32 factor) noexcept;
  PackedFloat2_128 mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, Float_64 factor) noexcept;

  PackedFloat8_256 mix(const PackedFloat8_256 &x, const PackedFloat8_256 &y, Float_32 factor) noexcept;
  PackedFloat4_256 mix(const PackedFloat4_256 &x, const PackedFloat4_256 &y, Float_64 factor) noexcept;

  PackedFloat4_128 mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const PackedFloat4_128 &factor) noexcept;
  PackedFloat2_128 mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const PackedFloat2_128 &factor) noexcept;

  PackedFloat8_256 mix(const PackedFloat8_256 &x, const PackedFloat8_256 &y, const PackedFloat8_256 &factor) noexcept;
  PackedFloat4_256 mix(const PackedFloat4_256 &x, const PackedFloat4_256 &y, const PackedFloat4_256 &factor) noexcept;
}
