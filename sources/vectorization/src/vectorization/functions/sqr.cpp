#include "vectorization/functions/sqr.h"

namespace vectorization {
  Float_32 sqr(const Float_32 value) noexcept {
    return value * value;
  }

  Float_64 sqr(const Float_64 value) noexcept {
    return value * value;
  }

  PackedFloat4_128 sqr(const PackedFloat4_128 &values) noexcept {
    return _mm_mul_ps(values, values);
  }

  PackedFloat2_128 sqr(const PackedFloat2_128 &values) noexcept {
    return _mm_mul_pd(values, values);
  }
}
