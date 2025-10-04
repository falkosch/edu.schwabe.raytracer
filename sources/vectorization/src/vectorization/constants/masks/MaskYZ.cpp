#include "vectorization/constants/masks/MaskYZ.h"

#include "vectorization/constants/masks/MaskAll.h"
#include "vectorization/constants/masks/MaskNone.h"
#include "vectorization/constants/masks/MaskXY.h"

namespace vectorization {
  template <>
  const PackedFloat4_128 MaskYZ<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(
        _mm_slli_si128(_mm_castps_si128(MaskXY<PackedFloat4_128>()), VectorSizes::X * sizeof(Float_32))
    );
  }

  template <>
  const PackedFloat8_256 MaskYZ<PackedFloat8_256>() noexcept {
    return _mm256_blend_ps(MaskNone<PackedFloat8_256>(), MaskAll<PackedFloat8_256>(), 0b01100110);
  }

  template <>
  const PackedFloat4_256 MaskYZ<PackedFloat4_256>() noexcept {
    return _mm256_blend_pd(MaskNone<PackedFloat4_256>(), MaskAll<PackedFloat4_256>(), 0b0110);
  }
}