#include "vectorization/functions/negate.h"

#include "vectorization/constants/values.h"

namespace vectorization {
  Float_32 negate(const Float_32 value) noexcept {
    return -value;
  }

  Float_64 negate(const Float_64 value) noexcept {
    return -value;
  }

  PackedFloat4_128 negate(const PackedFloat4_128 &v) noexcept {
    return _mm_xor_ps(NegativeZero<PackedFloat4_128>(), v);
  }

  PackedFloat2_128 negate(const PackedFloat2_128 &v) noexcept {
    return _mm_xor_pd(NegativeZero<PackedFloat2_128>(), v);
  }

  PackedFloat8_256 negate(const PackedFloat8_256 &v) noexcept {
    return _mm256_xor_ps(NegativeZero<PackedFloat8_256>(), v);
  }

  PackedFloat4_256 negate(const PackedFloat4_256 &v) noexcept {
    return _mm256_xor_pd(NegativeZero<PackedFloat4_256>(), v);
  }
}
