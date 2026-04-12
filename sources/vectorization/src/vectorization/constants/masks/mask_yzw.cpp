#include "vectorization/constants/masks/mask_yzw.h"

#include "vectorization/constants/masks/mask_all.h"
#include "vectorization/constants/masks/mask_none.h"

namespace vectorization {
  template <>
  PackedFloat4_128 MaskYZW<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_32)));
  }

  template <>
  PackedFloat8_256 MaskYZW<PackedFloat8_256>() noexcept {
    return _mm256_blend_ps(MaskNone<PackedFloat8_256>(), MaskAll<PackedFloat8_256>(), 0b11101110);
  }

  template <>
  PackedFloat4_256 MaskYZW<PackedFloat4_256>() noexcept {
    return _mm256_blend_pd(MaskNone<PackedFloat4_256>(), MaskAll<PackedFloat4_256>(), 0b1110);
  }
}
