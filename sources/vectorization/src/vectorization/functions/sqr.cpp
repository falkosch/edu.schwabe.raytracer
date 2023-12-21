#include "vectorization/functions/sqr.h"

namespace vectorization {
  const Float_32 sqr(const Float_32 v) noexcept {
    return v * v;
  }

  const Float_64 sqr(const Float_64 v) noexcept {
    return v * v;
  }

  const PackedFloat4_128 sqr(const PackedFloat4_128 &v) noexcept {
    return _mm_mul_ps(v, v);
  }

  const PackedFloat2_128 sqr(const PackedFloat2_128 &v) noexcept {
    return _mm_mul_pd(v, v);
  }
}