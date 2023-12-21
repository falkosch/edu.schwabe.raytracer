#include "vectorization/constants/masks/MaskZW.h"

#include "vectorization/constants/masks/MaskAll.h"
#include "vectorization/constants/masks/MaskNone.h"

namespace vectorization {
  template <>
  const PackedFloat4_128 MaskZW<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::Y * sizeof(Float_32)));
  }

  template <>
  const PackedFloat8_256 MaskZW<PackedFloat8_256>() noexcept {
    return _mm256_blend_ps(MaskNone<PackedFloat8_256>(), MaskAll<PackedFloat8_256>(), 0b11001100);
  }

  template <>
  const PackedFloat4_256 MaskZW<PackedFloat4_256>() noexcept {
    return _mm256_blend_pd(MaskNone<PackedFloat4_256>(), MaskAll<PackedFloat4_256>(), 0b1100);
  }
}