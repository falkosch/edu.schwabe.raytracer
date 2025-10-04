#pragma once

#include "type.h"

namespace vectorization {

  template <ASizeT Index>
  v_i32_4::ValueType component(const v_i32_4 &v) noexcept;

  template <>
  v_i32_4::ValueType component<VectorIndices::X>(const v_i32_4 &v) noexcept;

  template <>
  v_i32_4::ValueType component<VectorIndices::Y>(const v_i32_4 &v) noexcept;

  template <>
  v_i32_4::ValueType component<VectorIndices::Z>(const v_i32_4 &v) noexcept;

  template <>
  v_i32_4::ValueType component<VectorIndices::W>(const v_i32_4 &v) noexcept;

  v_i32_4::ValueType x(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType y(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType z(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType w(const v_i32_4 &v) noexcept;

  template <ASizeT Index>
  v_i32_4 replaceComponent(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;

  template <>
  v_i32_4 replaceComponent<VectorIndices::X>(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;

  template <>
  v_i32_4 replaceComponent<VectorIndices::Y>(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;

  template <>
  v_i32_4 replaceComponent<VectorIndices::Z>(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;

  template <>
  v_i32_4 replaceComponent<VectorIndices::W>(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;

  v_i32_4 replaceX(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;
  v_i32_4 replaceY(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;
  v_i32_4 replaceZ(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;
  v_i32_4 replaceW(const v_i32_4 &v, v_i32_4::ValueType s) noexcept;
}
