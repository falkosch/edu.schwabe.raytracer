#include "vectorization/constants/values/radian_to_degree.h"

#include "vectorization/constants/values/pi.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"

namespace vectorization {
#define RADIAN_TO_DEGREE (180.0 / C_PI)

  const PackedFloat2_128 RadianToDegree_PackedFloat2_128 = _mm_set1_pd(Float_64{RADIAN_TO_DEGREE});
  const PackedFloat4_128 RadianToDegree_PackedFloat4_128 = _mm_set1_ps(Float_32{RADIAN_TO_DEGREE});
  const PackedFloat4_256 RadianToDegree_PackedFloat4_256 = _mm256_set1_pd(Float_64{RADIAN_TO_DEGREE});
  const PackedFloat8_256 RadianToDegree_PackedFloat8_256 = _mm256_set1_ps(Float_32{RADIAN_TO_DEGREE});

  template <>
  Float_32 RadianToDegree<Float_32>() noexcept {
    return x(RadianToDegree_PackedFloat4_128);
  }

  template <>
  Float_64 RadianToDegree<Float_64>() noexcept {
    return x(RadianToDegree_PackedFloat2_128);
  }

  template <>
  PackedFloat2_128 RadianToDegree<PackedFloat2_128>() noexcept {
    return RadianToDegree_PackedFloat2_128;
  }

  template <>
  PackedFloat4_128 RadianToDegree<PackedFloat4_128>() noexcept {
    return RadianToDegree_PackedFloat4_128;
  }

  template <>
  PackedFloat4_256 RadianToDegree<PackedFloat4_256>() noexcept {
    return RadianToDegree_PackedFloat4_256;
  }

  template <>
  PackedFloat8_256 RadianToDegree<PackedFloat8_256>() noexcept {
    return RadianToDegree_PackedFloat8_256;
  }
}
