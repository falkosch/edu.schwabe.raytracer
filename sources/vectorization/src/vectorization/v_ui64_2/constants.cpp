#include "vectorization/v_ui64_2.h"

#include <limits>

namespace vectorization {
  template <>
  v_ui64_2 MaskAll<v_ui64_2>() noexcept {
    return MaskAll<v_ui64_2::PackedType>();
  }

  template <>
  v_ui64_2 MaskX<v_ui64_2>() noexcept {
    return _mm_srli_si128(MaskAll<v_ui64_2::PackedType>(), VectorSizes::X * sizeof(v_ui64_2::ValueType));
  }

  template <>
  v_ui64_2 MaskY<v_ui64_2>() noexcept {
    return _mm_slli_si128(MaskAll<v_ui64_2::PackedType>(), VectorSizes::X * sizeof(v_ui64_2::ValueType));
  }

  template <>
  v_ui64_2 MaskXY<v_ui64_2>() noexcept {
    return MaskAll<v_ui64_2>();
  }

  template <>
  v_ui64_2 Zero<v_ui64_2>() noexcept {
    return Zero<v_ui64_2::PackedType>();
  }

  template <>
  v_ui64_2 One<v_ui64_2>() noexcept {
    return _mm_srli_epi64(
        MaskAll<v_ui64_2::PackedType>(), sizeof(v_ui64_2::ValueType) * std::numeric_limits<unsigned char>::digits - 1
    );
  }

  template <>
  v_ui64_2 Two<v_ui64_2>() noexcept {
    return _mm_slli_epi64(
        _mm_srli_epi64(
            MaskAll<v_ui64_2::PackedType>(),
            sizeof(v_ui64_2::ValueType) * std::numeric_limits<unsigned char>::digits - 1
        ),
        1
    );
  }
}
