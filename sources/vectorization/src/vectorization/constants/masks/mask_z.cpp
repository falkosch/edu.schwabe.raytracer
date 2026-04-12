#include "vectorization/constants/masks/mask_z.h"

#include "vectorization/constants/masks/mask_all.h"
#include "vectorization/constants/masks/mask_none.h"
#include "vectorization/constants/masks/mask_w.h"

namespace vectorization {
  template <>
  PackedFloat4_128 MaskZ<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(
        _mm_srli_si128(_mm_castps_si128(MaskW<PackedFloat4_128>()), VectorSizes::X * sizeof(Float_32))
    );
  }

  template <>
  PackedFloat8_256 MaskZ<PackedFloat8_256>() noexcept {
    return _mm256_blend_ps(MaskNone<PackedFloat8_256>(), MaskAll<PackedFloat8_256>(), 0b01000100);
  }

  template <>
  PackedFloat4_256 MaskZ<PackedFloat4_256>() noexcept {
    return _mm256_blend_pd(MaskNone<PackedFloat4_256>(), MaskAll<PackedFloat4_256>(), 0b0100);
  }
}
