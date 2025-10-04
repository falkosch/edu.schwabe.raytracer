#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Float_32 copySign(Float_32 value) noexcept;
  Float_64 copySign(Float_64 value) noexcept;

  PackedFloat4_128 copySign(const PackedFloat4_128 &values) noexcept;
  PackedFloat2_128 copySign(const PackedFloat2_128 &values) noexcept;

  PackedFloat8_256 copySign(const PackedFloat8_256 &values) noexcept;
  PackedFloat4_256 copySign(const PackedFloat4_256 &values) noexcept;
}
