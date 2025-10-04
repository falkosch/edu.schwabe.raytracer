#include "vectorization/constants/masks/MaskXYZW.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization {
  template <>
  PackedFloat4_128 MaskXYZW<PackedFloat4_128>() noexcept {
    return MaskAll<PackedFloat4_128>();
  }

  template <>
  PackedFloat8_256 MaskXYZW<PackedFloat8_256>() noexcept {
    return MaskAll<PackedFloat8_256>();
  }

  template <>
  PackedFloat4_256 MaskXYZW<PackedFloat4_256>() noexcept {
    return MaskAll<PackedFloat4_256>();
  }
}
