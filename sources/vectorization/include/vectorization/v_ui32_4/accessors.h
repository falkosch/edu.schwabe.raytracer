#pragma once

#include "type.h"

namespace vectorization {

  template <ASizeT Index>
  v_ui32_4::ValueType component(const v_ui32_4 &vector) noexcept;

  template <>
  v_ui32_4::ValueType component<VectorIndices::X>(const v_ui32_4 &vector) noexcept;

  template <>
  v_ui32_4::ValueType component<VectorIndices::Y>(const v_ui32_4 &vector) noexcept;

  template <>
  v_ui32_4::ValueType component<VectorIndices::Z>(const v_ui32_4 &vector) noexcept;

  template <>
  v_ui32_4::ValueType component<VectorIndices::W>(const v_ui32_4 &vector) noexcept;

  v_ui32_4::ValueType x(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType y(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType z(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType w(const v_ui32_4 &vector) noexcept;

  template <ASizeT Index>
  v_ui32_4 replaceComponent(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;

  template <>
  v_ui32_4 replaceComponent<VectorIndices::X>(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;

  template <>
  v_ui32_4 replaceComponent<VectorIndices::Y>(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;

  template <>
  v_ui32_4 replaceComponent<VectorIndices::Z>(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;

  template <>
  v_ui32_4 replaceComponent<VectorIndices::W>(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;

  v_ui32_4 replaceX(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;
  v_ui32_4 replaceY(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;
  v_ui32_4 replaceZ(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;
  v_ui32_4 replaceW(const v_ui32_4 &vector, v_ui32_4::ValueType scalar) noexcept;
}
