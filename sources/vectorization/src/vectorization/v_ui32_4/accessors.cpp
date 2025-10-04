#include "vectorization/v_ui32_4.h"

namespace vectorization {
  template <>
  v_ui32_4::ValueType component<VectorIndices::X>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(_mm_cvtsi128_si32(vector.components));
  }

  template <>
  v_ui32_4::ValueType component<VectorIndices::Y>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(
        _mm_cvtsi128_si32(_mm_srli_si128(vector.components, VectorSizes::X * sizeof(v_ui32_4::ValueType)))
    );
  }

  template <>
  v_ui32_4::ValueType component<VectorIndices::Z>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(_mm_cvtsi128_si32(_mm_unpackhi_epi32(vector.components, vector.components))
    );
  }

  template <>
  v_ui32_4::ValueType component<VectorIndices::W>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(
        _mm_cvtsi128_si32(_mm_srli_si128(vector.components, VectorSizes::Z * sizeof(v_ui32_4::ValueType)))
    );
  }

  v_ui32_4::ValueType x(const v_ui32_4 &vector) noexcept {
    return component<VectorIndices::X>(vector);
  }

  v_ui32_4::ValueType y(const v_ui32_4 &vector) noexcept {
    return component<VectorIndices::Y>(vector);
  }

  v_ui32_4::ValueType z(const v_ui32_4 &vector) noexcept {
    return component<VectorIndices::Z>(vector);
  }

  v_ui32_4::ValueType w(const v_ui32_4 &vector) noexcept {
    return component<VectorIndices::W>(vector);
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::X>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return _mm_insert_epi32(vector.components, static_cast<int>(scalar), VectorIndices::X);
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::Y>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return _mm_insert_epi32(vector.components, static_cast<int>(scalar), VectorIndices::Y);
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::Z>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return _mm_insert_epi32(vector.components, static_cast<int>(scalar), VectorIndices::Z);
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::W>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return _mm_insert_epi32(vector.components, static_cast<int>(scalar), VectorIndices::W);
  }

  v_ui32_4 replaceX(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::X>(vector, scalar);
  }

  v_ui32_4 replaceY(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Y>(vector, scalar);
  }

  v_ui32_4 replaceZ(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Z>(vector, scalar);
  }

  v_ui32_4 replaceW(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::W>(vector, scalar);
  }
}
