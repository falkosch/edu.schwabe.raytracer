#include "vectorization/constants/values/Sin45.h"

#include "vectorization/accessors.h"

namespace vectorization {
#define SIN_45 0.70710678118654752440084436210485

  const PackedFloat2_128 Sin45_PackedFloat2_128 = _mm_set1_pd(Float_64{SIN_45});
  const PackedFloat4_128 Sin45_PackedFloat4_128 = _mm_set1_ps(Float_32{SIN_45});
  const PackedFloat4_256 Sin45_PackedFloat4_256 = _mm256_set1_pd(Float_64{SIN_45});
  const PackedFloat8_256 Sin45_PackedFloat8_256 = _mm256_set1_ps(Float_32{SIN_45});

  template <>
  const Float_32 Sin45<Float_32>() noexcept {
    return x(Sin45<PackedFloat4_128>());
  }

  template <>
  const Float_64 Sin45<Float_64>() noexcept {
    return x(Sin45<PackedFloat2_128>());
  }

  template <>
  const PackedFloat2_128 Sin45<PackedFloat2_128>() noexcept {
    return Sin45_PackedFloat2_128;
  }

  template <>
  const PackedFloat4_128 Sin45<PackedFloat4_128>() noexcept {
    return Sin45_PackedFloat4_128;
  }

  template <>
  const PackedFloat4_256 Sin45<PackedFloat4_256>() noexcept {
    return Sin45_PackedFloat4_256;
  }

  template <>
  const PackedFloat8_256 Sin45<PackedFloat8_256>() noexcept {
    return Sin45_PackedFloat8_256;
  }
}