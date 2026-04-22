#include "vectorization/v_ui32_4.h"

#include "vectorization/accessors/component_128i.h"
#include "vectorization/accessors/replace_component_128i.h"

namespace vectorization {
  template <>
  v_ui32_4::ValueType component<VectorIndices::X>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(component<VectorIndices::X>(vector.components));
  }

  template <>
  v_ui32_4::ValueType component<VectorIndices::Y>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(component<VectorIndices::Y>(vector.components));
  }

  template <>
  v_ui32_4::ValueType component<VectorIndices::Z>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(component<VectorIndices::Z>(vector.components));
  }

  template <>
  v_ui32_4::ValueType component<VectorIndices::W>(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(component<VectorIndices::W>(vector.components));
  }

  v_ui32_4::ValueType x(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(x(vector.components));
  }

  v_ui32_4::ValueType y(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(y(vector.components));
  }

  v_ui32_4::ValueType z(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(z(vector.components));
  }

  v_ui32_4::ValueType w(const v_ui32_4 &vector) noexcept {
    return static_cast<v_ui32_4::ValueType>(w(vector.components));
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::X>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::X>(vector.components, static_cast<Int_32>(scalar));
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::Y>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Y>(vector.components, static_cast<Int_32>(scalar));
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::Z>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Z>(vector.components, static_cast<Int_32>(scalar));
  }

  template <>
  v_ui32_4 replaceComponent<VectorIndices::W>(const v_ui32_4 &vector, const v_ui32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::W>(vector.components, static_cast<Int_32>(scalar));
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
