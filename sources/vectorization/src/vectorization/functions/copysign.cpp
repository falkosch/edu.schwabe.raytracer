#include "vectorization/functions/copysign.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#include "vectorization/constants/masks.h"
#include "vectorization/constants/values.h"

namespace vectorization {
  PackedFloat4_128 copySign(const PackedFloat4_128 &values) noexcept {
    return _mm_or_ps(One<PackedFloat4_128>(), _mm_and_ps(NegativeZero<PackedFloat4_128>(), values));
  }

  PackedFloat2_128 copySign(const PackedFloat2_128 &values) noexcept {
    return _mm_or_pd(One<PackedFloat2_128>(), _mm_and_pd(NegativeZero<PackedFloat2_128>(), values));
  }

  Float_32 copySign(const Float_32 value) noexcept {
    return x(copySign(_mm_set_ss(value)));
  }

  Float_64 copySign(const Float_64 value) noexcept {
    return x(copySign(_mm_set_sd(value)));
  }
}
