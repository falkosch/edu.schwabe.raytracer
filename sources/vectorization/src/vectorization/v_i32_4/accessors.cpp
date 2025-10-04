#include "vectorization/v_i32_4.h"

namespace vectorization {
  template <>
  v_i32_4::ValueType component<VectorIndices::X>(const v_i32_4 &v) noexcept {
    return _mm_cvtsi128_si32(v.components);
  }

  template <>
  v_i32_4::ValueType component<VectorIndices::Y>(const v_i32_4 &v) noexcept {
    return _mm_cvtsi128_si32(_mm_srli_si128(v.components, VectorSizes::X * sizeof(v_i32_4::ValueType)));
  }

  template <>
  v_i32_4::ValueType component<VectorIndices::Z>(const v_i32_4 &v) noexcept {
    return _mm_cvtsi128_si32(_mm_srli_si128(v.components, VectorSizes::Y * sizeof(v_i32_4::ValueType)));
  }

  template <>
  v_i32_4::ValueType component<VectorIndices::W>(const v_i32_4 &v) noexcept {
    return _mm_cvtsi128_si32(_mm_srli_si128(v.components, VectorSizes::Z * sizeof(v_i32_4::ValueType)));
  }

  v_i32_4::ValueType x(const v_i32_4 &v) noexcept {
    return component<VectorIndices::X>(v);
  }

  v_i32_4::ValueType y(const v_i32_4 &v) noexcept {
    return component<VectorIndices::Y>(v);
  }

  v_i32_4::ValueType z(const v_i32_4 &v) noexcept {
    return component<VectorIndices::Z>(v);
  }

  v_i32_4::ValueType w(const v_i32_4 &v) noexcept {
    return component<VectorIndices::W>(v);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::X>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return _mm_insert_epi32(v.components, s, VectorIndices::X);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::Y>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return _mm_insert_epi32(v.components, s, VectorIndices::Y);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::Z>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return _mm_insert_epi32(v.components, s, VectorIndices::Z);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::W>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return _mm_insert_epi32(v.components, s, VectorIndices::W);
  }

  v_i32_4 replaceX(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::X>(v, s);
  }

  v_i32_4 replaceY(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::Y>(v, s);
  }

  v_i32_4 replaceZ(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::Z>(v, s);
  }

  v_i32_4 replaceW(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::W>(v, s);
  }
}
