#include "vectorization/functions/fract.h"

#include "vectorization/functions/floor.h"

namespace vectorization {
  const Float_32 fract(const Float_32 v) noexcept {
    return v - floor(v);
  }

  const Float_64 fract(const Float_64 v) noexcept {
    return v - floor(v);
  }

  const PackedFloat4_128 fract(const PackedFloat4_128 &v) noexcept {
    return _mm_sub_ps(v, floor(v));
  }

  const PackedFloat2_128 fract(const PackedFloat2_128 &v) noexcept {
    return _mm_sub_pd(v, floor(v));
  }
}