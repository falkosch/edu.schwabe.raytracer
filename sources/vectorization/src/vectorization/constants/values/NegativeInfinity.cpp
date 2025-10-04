#include "vectorization/constants/values/NegativeInfinity.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  const Float_32 NegativeInfinity<Float_32>() noexcept {
    return x(NegativeInfinity<PackedFloat4_128>());
  }

  template <>
  const Float_64 NegativeInfinity<Float_64>() noexcept {
    return x(NegativeInfinity<PackedFloat2_128>());
  }

  template <>
  const PackedFloat2_128 NegativeInfinity<PackedFloat2_128>() noexcept {
    // erase double fraction part (52 bits)
    return _mm_castsi128_pd(_mm_slli_epi64(MaskAll<PackedInts_128>(), 52));
  }

  template <>
  const PackedFloat4_128 NegativeInfinity<PackedFloat4_128>() noexcept {
    // erase fraction part (23 bits)
    return _mm_castsi128_ps(_mm_slli_epi32(MaskAll<PackedInts_128>(), 23));
  }

  template <>
  const PackedFloat4_256 NegativeInfinity<PackedFloat4_256>() noexcept {
    const auto v = NegativeInfinity<PackedFloat2_128>();
    return _mm256_set_m128d(v, v);
  }

  template <>
  const PackedFloat8_256 NegativeInfinity<PackedFloat8_256>() noexcept {
    const auto v = NegativeInfinity<PackedFloat4_128>();
    return _mm256_set_m128(v, v);
  }
}