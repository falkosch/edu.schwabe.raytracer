#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Float_32 copySign(const Float_32 v) noexcept;

  const Float_64 copySign(const Float_64 v) noexcept;

  const PackedFloat4_128 copySign(const PackedFloat4_128 &v) noexcept;

  const PackedFloat2_128 copySign(const PackedFloat2_128 &v) noexcept;

  const PackedFloat8_256 copySign(const PackedFloat8_256 &v) noexcept;

  const PackedFloat4_256 copySign(const PackedFloat4_256 &v) noexcept;
}
