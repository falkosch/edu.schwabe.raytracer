#pragma once

#include "type.h"

namespace vectorization {

  template <ASizeT Index>
  v_f32_8::ValueType component(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X1>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X2>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X3>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X4>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X5>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X6>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X7>(const v_f32_8 &v) noexcept;

  template <>
  v_f32_8::ValueType component<VectorIndices::X8>(const v_f32_8 &v) noexcept;

  v_f32_8::ValueType x1(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x2(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x3(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x4(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x5(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x6(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x7(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType x8(const v_f32_8 &v) noexcept;

  template <ASizeT Index>
  v_f32_8 replaceComponent(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X1>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X2>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X3>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X4>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X5>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X6>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X7>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  template <>
  v_f32_8 replaceComponent<VectorIndices::X8>(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;

  v_f32_8 replaceX1(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX2(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX3(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX4(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX5(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX6(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX7(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
  v_f32_8 replaceX8(const v_f32_8 &v, v_f32_8::ValueType s) noexcept;
}
