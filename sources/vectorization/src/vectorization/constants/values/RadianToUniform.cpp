#include "vectorization/constants/values/RadianToUniform.h"

#include "vectorization/constants/values/Pi.h"

#include "vectorization/accessors.h"

namespace vectorization {
#define RADIAN_TO_UNIFORM (0.5 / C_PI)

  const PackedFloat2_128 RadianToUniform_PackedFloat2_128 = _mm_set1_pd(Float_64{RADIAN_TO_UNIFORM});
  const PackedFloat4_128 RadianToUniform_PackedFloat4_128 = _mm_set1_ps(Float_32{RADIAN_TO_UNIFORM});
  const PackedFloat4_256 RadianToUniform_PackedFloat4_256 = _mm256_set1_pd(Float_64{RADIAN_TO_UNIFORM});
  const PackedFloat8_256 RadianToUniform_PackedFloat8_256 = _mm256_set1_ps(Float_32{RADIAN_TO_UNIFORM});

  template <>
  Float_32 RadianToUniform<Float_32>() noexcept {
    return x(RadianToUniform<PackedFloat4_128>());
  }

  template <>
  Float_64 RadianToUniform<Float_64>() noexcept {
    return x(RadianToUniform<PackedFloat2_128>());
  }

  template <>
  PackedFloat2_128 RadianToUniform<PackedFloat2_128>() noexcept {
    return RadianToUniform_PackedFloat2_128;
  }

  template <>
  PackedFloat4_128 RadianToUniform<PackedFloat4_128>() noexcept {
    return RadianToUniform_PackedFloat4_128;
  }

  template <>
  PackedFloat4_256 RadianToUniform<PackedFloat4_256>() noexcept {
    return RadianToUniform_PackedFloat4_256;
  }

  template <>
  PackedFloat8_256 RadianToUniform<PackedFloat8_256>() noexcept {
    return RadianToUniform_PackedFloat8_256;
  }
}
