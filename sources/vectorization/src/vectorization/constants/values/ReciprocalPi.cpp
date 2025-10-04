#include "vectorization/constants/values/ReciprocalPi.h"

#include "vectorization/constants/values/Pi.h"

#include "vectorization/accessors.h"

namespace vectorization {
#define RECIPROCAL_PI (1.0 / C_PI)

  const PackedFloat2_128 ReciprocalPi_PackedFloat2_128 = _mm_set1_pd(Float_64{RECIPROCAL_PI});
  const PackedFloat4_128 ReciprocalPi_PackedFloat4_128 = _mm_set1_ps(Float_32{RECIPROCAL_PI});
  const PackedFloat4_256 ReciprocalPi_PackedFloat4_256 = _mm256_set1_pd(Float_64{RECIPROCAL_PI});
  const PackedFloat8_256 ReciprocalPi_PackedFloat8_256 = _mm256_set1_ps(Float_32{RECIPROCAL_PI});

  template <>
  Float_32 ReciprocalPi<Float_32>() noexcept {
    return x(ReciprocalPi<PackedFloat4_128>());
  }

  template <>
  Float_64 ReciprocalPi<Float_64>() noexcept {
    return x(ReciprocalPi<PackedFloat2_128>());
  }

  template <>
  PackedFloat2_128 ReciprocalPi<PackedFloat2_128>() noexcept {
    return ReciprocalPi_PackedFloat2_128;
  }

  template <>
  PackedFloat4_128 ReciprocalPi<PackedFloat4_128>() noexcept {
    return ReciprocalPi_PackedFloat4_128;
  }

  template <>
  PackedFloat4_256 ReciprocalPi<PackedFloat4_256>() noexcept {
    return ReciprocalPi_PackedFloat4_256;
  }

  template <>
  PackedFloat8_256 ReciprocalPi<PackedFloat8_256>() noexcept {
    return ReciprocalPi_PackedFloat8_256;
  }
}
