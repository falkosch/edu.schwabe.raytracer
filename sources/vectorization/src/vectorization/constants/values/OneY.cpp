#include "vectorization/constants/values/OneY.h"

#include "vectorization/constants/values/One.h"
#include "vectorization/constants/values/Zero.h"

#include "vectorization/blends.h"

namespace vectorization {
  template <>
  PackedFloat4_128 OneY<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_slli_si128(
        _mm_srli_si128(_mm_castps_si128(One<PackedFloat4_128>()), VectorSizes::Z * sizeof(Float_32)),
        VectorSizes::X * sizeof(Float_32)
    ));
  }

  template <>
  PackedFloat2_128 OneY<PackedFloat2_128>() noexcept {
    return _mm_castsi128_pd(_mm_slli_si128(_mm_castpd_si128(One<PackedFloat2_128>()), VectorSizes::X * sizeof(Float_64))
    );
  }

  template <>
  PackedFloat4_256 OneY<PackedFloat4_256>() noexcept {
    return blend<false, true, false, false>(Zero<PackedFloat4_256>(), One<PackedFloat4_256>());
  }

  template <>
  PackedFloat8_256 OneY<PackedFloat8_256>() noexcept {
    return blend<false, true, false, false>(Zero<PackedFloat8_256>(), One<PackedFloat8_256>());
  }
}
