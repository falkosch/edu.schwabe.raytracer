#include "vectorization/constants/values/NegativeZero.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

#include <limits>

namespace vectorization {
  template <>
  const Float_32 NegativeZero<Float_32>() noexcept {
    return x(NegativeZero<PackedFloat4_128>());
  }

  template <>
  const Float_64 NegativeZero<Float_64>() noexcept {
    return x(NegativeZero<PackedFloat2_128>());
  }

  template <>
  const PackedFloat2_128 NegativeZero<PackedFloat2_128>() noexcept {
    return _mm_castsi128_pd(
        _mm_slli_epi64(MaskAll<PackedInts_128>(), sizeof(Int_64) * std::numeric_limits<UInt_8>::digits - 1)
    );
  }

  template <>
  const PackedFloat4_128 NegativeZero<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(
        _mm_slli_epi32(MaskAll<PackedInts_128>(), sizeof(Int_32) * std::numeric_limits<UInt_8>::digits - 1)
    );
  }

  template <>
  const PackedFloat4_256 NegativeZero<PackedFloat4_256>() noexcept {
    const auto v = NegativeZero<PackedFloat2_128>();
    return _mm256_set_m128d(v, v);
  }

  template <>
  const PackedFloat8_256 NegativeZero<PackedFloat8_256>() noexcept {
    const auto v = NegativeZero<PackedFloat4_128>();
    return _mm256_set_m128(v, v);
  }
}