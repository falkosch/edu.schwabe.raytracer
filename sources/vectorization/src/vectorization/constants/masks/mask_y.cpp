#include "vectorization/constants/masks/mask_y.h"

#include "vectorization/constants/masks/mask_all.h"
#include "vectorization/constants/masks/mask_none.h"
#include "vectorization/constants/masks/mask_x.h"

namespace vectorization {
  template <>
  PackedFloat4_128 MaskY<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(
        _mm_slli_si128(_mm_castps_si128(MaskX<PackedFloat4_128>()), VectorSizes::X * sizeof(Float_32))
    );
  }

  template <>
  PackedFloat2_128 MaskY<PackedFloat2_128>() noexcept {
    return _mm_castsi128_pd(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_64)));
  }

  template <>
  PackedFloat8_256 MaskY<PackedFloat8_256>() noexcept {
    return _mm256_blend_ps(MaskNone<PackedFloat8_256>(), MaskAll<PackedFloat8_256>(), 0b00100010);
  }

  template <>
  PackedFloat4_256 MaskY<PackedFloat4_256>() noexcept {
    return _mm256_blend_pd(MaskNone<PackedFloat4_256>(), MaskAll<PackedFloat4_256>(), 0b0010);
  }
}
