#include "vectorization/functions/movemask.h"

namespace vectorization {
  Int_32 movemask(const PackedFloat4_128 &v) noexcept {
    return _mm_movemask_ps(v);
  }

  Int_32 movemask(const PackedFloat2_128 &v) noexcept {
    return _mm_movemask_pd(v);
  }

  Int_32 movemask(const PackedFloat8_256 &v) noexcept {
    return _mm256_movemask_ps(v);
  }

  Int_32 movemask(const PackedFloat4_256 &v) noexcept {
    return _mm256_movemask_pd(v);
  }
}
