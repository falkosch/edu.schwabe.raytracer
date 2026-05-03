#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT Index>
  v_f64_2::ValueType component(const v_f64_2 &vector) noexcept;

  template <>
  v_f64_2::ValueType component<VectorIndices::X>(const v_f64_2 &vector) noexcept;

  template <>
  v_f64_2::ValueType component<VectorIndices::Y>(const v_f64_2 &vector) noexcept;

  v_f64_2::ValueType x(const v_f64_2 &vector) noexcept;
  v_f64_2::ValueType y(const v_f64_2 &vector) noexcept;

  v_f64_2::ValueType x1(const v_f64_2 &vector) noexcept;
  v_f64_2::ValueType x2(const v_f64_2 &vector) noexcept;

  template <ASizeT Index>
  v_f64_2 replaceComponent(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;

  template <>
  v_f64_2 replaceComponent<VectorIndices::X>(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;

  template <>
  v_f64_2 replaceComponent<VectorIndices::Y>(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;

  v_f64_2 replaceX(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;
  v_f64_2 replaceY(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;

  v_f64_2 replaceX1(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;
  v_f64_2 replaceX2(const v_f64_2 &vector, v_f64_2::ValueType scalar) noexcept;

  v_f64_2 replaceComponent(const v_f64_2 &vector, const v_f64_2 &replacement, ASizeT index) noexcept;
  v_f64_2 replaceComponent(const v_f64_2 &vector, v_f64_2::ValueType replacement, ASizeT index) noexcept;
}
