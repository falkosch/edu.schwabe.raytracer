#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT Index>
  v_i64_2::ValueType component(const v_i64_2 &vector) noexcept;

  template <>
  v_i64_2::ValueType component<VectorIndices::X>(const v_i64_2 &vector) noexcept;

  template <>
  v_i64_2::ValueType component<VectorIndices::Y>(const v_i64_2 &vector) noexcept;

  v_i64_2::ValueType x(const v_i64_2 &vector) noexcept;
  v_i64_2::ValueType y(const v_i64_2 &vector) noexcept;

  template <ASizeT Index>
  v_i64_2 replaceComponent(const v_i64_2 &vector, v_i64_2::ValueType scalar) noexcept;

  template <>
  v_i64_2 replaceComponent<VectorIndices::X>(const v_i64_2 &vector, v_i64_2::ValueType scalar) noexcept;

  template <>
  v_i64_2 replaceComponent<VectorIndices::Y>(const v_i64_2 &vector, v_i64_2::ValueType scalar) noexcept;

  v_i64_2 replaceX(const v_i64_2 &vector, v_i64_2::ValueType scalar) noexcept;
  v_i64_2 replaceY(const v_i64_2 &vector, v_i64_2::ValueType scalar) noexcept;
}
