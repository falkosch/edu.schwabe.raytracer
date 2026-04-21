#include "vectorization/constants/values/degree_to_radian.h"

#include "vectorization/constants/values/pi.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

namespace vectorization {
#define DEGREE_TO_RADIAN (C_PI / 180.0)

  const PackedFloat2_128 DegreeToRadian_PackedFloat2_128 = _mm_set1_pd(Float_64{DEGREE_TO_RADIAN});
  const PackedFloat4_128 DegreeToRadian_PackedFloat4_128 = _mm_set1_ps(Float_32{DEGREE_TO_RADIAN});
  const PackedFloat4_256 DegreeToRadian_PackedFloat4_256 = _mm256_set1_pd(Float_64{DEGREE_TO_RADIAN});
  const PackedFloat8_256 DegreeToRadian_PackedFloat8_256 = _mm256_set1_ps(Float_32{DEGREE_TO_RADIAN});

  template <>
  Float_32 DegreeToRadian<Float_32>() noexcept {
    return x(DegreeToRadian_PackedFloat4_128);
  }

  template <>
  Float_64 DegreeToRadian<Float_64>() noexcept {
    return x(DegreeToRadian_PackedFloat2_128);
  }

  template <>
  PackedFloat2_128 DegreeToRadian<PackedFloat2_128>() noexcept {
    return DegreeToRadian_PackedFloat2_128;
  }

  template <>
  PackedFloat4_128 DegreeToRadian<PackedFloat4_128>() noexcept {
    return DegreeToRadian_PackedFloat4_128;
  }

  template <>
  PackedFloat4_256 DegreeToRadian<PackedFloat4_256>() noexcept {
    return DegreeToRadian_PackedFloat4_256;
  }

  template <>
  PackedFloat8_256 DegreeToRadian<PackedFloat8_256>() noexcept {
    return DegreeToRadian_PackedFloat8_256;
  }
}
