#include "vectorization/constants/values/one_w.h"

#include "vectorization/constants/values/one.h"
#include "vectorization/constants/values/zero.h"

#include "vectorization/blends/blend_256d.h"
#include "vectorization/blends/blend_256s.h"

namespace vectorization {
  template <>
  PackedFloat4_128 OneW<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(One<PackedFloat4_128>()), VectorSizes::Z * sizeof(Float_32))
    );
  }

  template <>
  PackedFloat4_256 OneW<PackedFloat4_256>() noexcept {
    return blend<false, false, false, true>(Zero<PackedFloat4_256>(), One<PackedFloat4_256>());
  }

  template <>
  PackedFloat8_256 OneW<PackedFloat8_256>() noexcept {
    return blend<false, false, false, true>(Zero<PackedFloat8_256>(), One<PackedFloat8_256>());
  }
}
