#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT Index>
  v_f32_4::ValueType component(const v_f32_4 &vector) noexcept;

  template <>
  v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 &vector) noexcept;

  template <>
  v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 &vector) noexcept;

  template <>
  v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 &vector) noexcept;

  template <>
  v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 &vector) noexcept;

  v_f32_4::ValueType x(const v_f32_4 &vector) noexcept;
  v_f32_4::ValueType y(const v_f32_4 &vector) noexcept;
  v_f32_4::ValueType z(const v_f32_4 &vector) noexcept;
  v_f32_4::ValueType w(const v_f32_4 &vector) noexcept;

  v_f32_4::ValueType x1(const v_f32_4 &vector) noexcept;
  v_f32_4::ValueType x2(const v_f32_4 &vector) noexcept;
  v_f32_4::ValueType x3(const v_f32_4 &vector) noexcept;
  v_f32_4::ValueType x4(const v_f32_4 &vector) noexcept;

  template <ASizeT Index>
  v_f32_4 replaceComponent(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  template <>
  v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  template <>
  v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  template <>
  v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  template <>
  v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  v_f32_4 replaceX(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;
  v_f32_4 replaceY(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;
  v_f32_4 replaceZ(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;
  v_f32_4 replaceW(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  v_f32_4 replaceX1(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;
  v_f32_4 replaceX2(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;
  v_f32_4 replaceX3(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;
  v_f32_4 replaceX4(const v_f32_4 &vector, v_f32_4::ValueType scalar) noexcept;

  v_f32_4 replaceComponent(const v_f32_4 &vector, const v_f32_4 &replacement, ASizeT index) noexcept;
  v_f32_4 replaceComponent(const v_f32_4 &vector, v_f32_4::ValueType replacement, ASizeT index) noexcept;
}
