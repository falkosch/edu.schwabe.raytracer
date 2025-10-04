#include "vectorization/constants/values/OneW.h"

#include "vectorization/constants/values/One.h"
#include "vectorization/constants/values/Zero.h"

#include "vectorization/blends.h"

namespace vectorization {
  template <>
  const PackedFloat4_128 OneW<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(One<PackedFloat4_128>()), VectorSizes::Z * sizeof(Float_32))
    );
  }

  template <>
  const PackedFloat4_256 OneW<PackedFloat4_256>() noexcept {
    return blend<false, false, false, true>(Zero<PackedFloat4_256>(), One<PackedFloat4_256>());
  }

  template <>
  const PackedFloat8_256 OneW<PackedFloat8_256>() noexcept {
    return blend<false, false, false, true>(Zero<PackedFloat8_256>(), One<PackedFloat8_256>());
  }
}