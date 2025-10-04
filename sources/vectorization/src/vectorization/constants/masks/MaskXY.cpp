#include "vectorization/constants/masks/MaskXY.h"

#include "vectorization/constants/masks/MaskAll.h"
#include "vectorization/constants/masks/MaskNone.h"

namespace vectorization {
  template <>
  PackedFloat4_128 MaskXY<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::Y * sizeof(Float_32)));
  }

  template <>
  PackedFloat2_128 MaskXY<PackedFloat2_128>() noexcept {
    return MaskAll<PackedFloat2_128>();
  }

  template <>
  PackedFloat8_256 MaskXY<PackedFloat8_256>() noexcept {
    return _mm256_blend_ps(MaskNone<PackedFloat8_256>(), MaskAll<PackedFloat8_256>(), 0b00110011);
  }

  template <>
  PackedFloat4_256 MaskXY<PackedFloat4_256>() noexcept {
    return _mm256_blend_pd(MaskNone<PackedFloat4_256>(), MaskAll<PackedFloat4_256>(), 0b0011);
  }
}
