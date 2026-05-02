#include "vectorization/v_ui32_8.h"

#include "vectorization/constants/masks/mask_all.h"

#include <limits>

namespace vectorization {
  template <>
  v_ui32_8 Zero<v_ui32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Zero<PackedInts_256>();
#else
    const auto z = Zero<v_ui32_8::HalfPackedType>();
    return {{z, z}};
#endif
  }

  template <>
  v_ui32_8 One<v_ui32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srli_epi32(
        MaskAll<PackedInts_256>(), sizeof(v_ui32_8::ValueType) * std::numeric_limits<unsigned char>::digits - 1
    );
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_set1_epi32(1);
#else
    const auto one = _mm_set1_epi32(1);
    return {{one, one}};
#endif
  }

  template <>
  v_ui32_8 Two<v_ui32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_slli_epi32(
        _mm256_srli_epi32(
            MaskAll<PackedInts_256>(), sizeof(v_ui32_8::ValueType) * std::numeric_limits<unsigned char>::digits - 1
        ),
        1
    );
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_set1_epi32(2);
#else
    const auto two = _mm_set1_epi32(2);
    return {{two, two}};
#endif
  }
}
