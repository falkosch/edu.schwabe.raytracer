#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  Int_32 movemask(const PackedFloat4_128 &v) noexcept;
  Int_32 movemask(const PackedFloat2_128 &v) noexcept;

  Int_32 movemask(const PackedFloat8_256 &v) noexcept;
  Int_32 movemask(const PackedFloat4_256 &v) noexcept;
}
