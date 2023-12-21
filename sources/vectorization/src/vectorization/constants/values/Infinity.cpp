#include "vectorization/constants/values/Infinity.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  const Float_32 Infinity<Float_32>() noexcept {
    return x(Infinity<PackedFloat4_128>());
  }

  template <>
  const Float_64 Infinity<Float_64>() noexcept {
    return x(Infinity<PackedFloat2_128>());
  }

  template <>
  const PackedFloat2_128 Infinity<PackedFloat2_128>() noexcept {
    // erase fraction part (52 bits) and sign bit
    return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 53), 1));
  }

  template <>
  const PackedFloat4_128 Infinity<PackedFloat4_128>() noexcept {
    // erase fraction part (23 bits) and sign bit
    return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 24), 1));
  }

  template <>
  const PackedFloat4_256 Infinity<PackedFloat4_256>() noexcept {
    const auto v = Infinity<PackedFloat2_128>();
    return _mm256_set_m128d(v, v);
  }

  template <>
  const PackedFloat8_256 Infinity<PackedFloat8_256>() noexcept {
    const auto v = Infinity<PackedFloat4_128>();
    return _mm256_set_m128(v, v);
  }
}