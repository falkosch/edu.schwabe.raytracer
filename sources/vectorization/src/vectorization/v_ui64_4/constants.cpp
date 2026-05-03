#include "vectorization/v_ui64_4/constants.h"

namespace vectorization {
  template <>
  v_ui64_4 Zero<v_ui64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Zero<v_ui64_4::PackedType>();
#else
    return {{Zero<v_ui64_4::HalfPackedType>(), Zero<v_ui64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_ui64_4 One<v_ui64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_set1_epi64x(1);
#else
    return {{_mm_set1_epi64x(1), _mm_set1_epi64x(1)}};
#endif
  }
}
