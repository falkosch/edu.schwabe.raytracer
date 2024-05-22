#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // Checks whether 'v' is a NaN
  bool isNaN(Float_32 value) noexcept;
  bool isNaN(Float_64 value) noexcept;

  PackedFloat4_128 isNaN(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 isNaN(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 isNaN(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 isNaN(const PackedFloat4_256 &values) noexcept;

  // Checks whether 'a' is NaN or 'b' is a NaN value
  bool isNaN(Float_32 a, Float_32 b) noexcept;
  bool isNaN(Float_64 a, Float_64 b) noexcept;

  PackedFloat4_128 isNaN(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 isNaN(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  PackedFloat8_256 isNaN(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 isNaN(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
