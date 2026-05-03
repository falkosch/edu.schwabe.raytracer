#include "vectorization/v_i64_2.h"

namespace vectorization {
  template <>
  v_i64_2 MaskAll<v_i64_2>() noexcept {
    return MaskAll<v_i64_2::PackedType>();
  }

  template <>
  v_i64_2 MaskX<v_i64_2>() noexcept {
    return _mm_srli_si128(MaskAll<v_i64_2::PackedType>(), VectorSizes::X * sizeof(v_i64_2::ValueType));
  }

  template <>
  v_i64_2 MaskY<v_i64_2>() noexcept {
    return _mm_slli_si128(MaskAll<v_i64_2::PackedType>(), VectorSizes::X * sizeof(v_i64_2::ValueType));
  }

  template <>
  v_i64_2 MaskXY<v_i64_2>() noexcept {
    return MaskAll<v_i64_2>();
  }

  template <>
  v_i64_2 Zero<v_i64_2>() noexcept {
    return Zero<v_i64_2::PackedType>();
  }

  template <>
  v_i64_2 One<v_i64_2>() noexcept {
    return _mm_set1_epi64x(1);
  }

  template <>
  v_i64_2 NegativeOne<v_i64_2>() noexcept {
    return _mm_set1_epi64x(-1);
  }
}
