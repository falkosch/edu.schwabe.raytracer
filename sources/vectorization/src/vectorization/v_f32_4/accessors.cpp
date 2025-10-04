#include "vectorization/v_f32_4/accessors.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  const v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 &v) noexcept {
    return component<VectorIndices::X>(v.components);
  }

  template <>
  const v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 &v) noexcept {
    return component<VectorIndices::Y>(v.components);
  }

  template <>
  const v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 &v) noexcept {
    return component<VectorIndices::Z>(v.components);
  }

  template <>
  const v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 &v) noexcept {
    return component<VectorIndices::W>(v.components);
  }

  const v_f32_4::ValueType x(const v_f32_4 &v) noexcept {
    return x(v.components);
  }

  const v_f32_4::ValueType y(const v_f32_4 &v) noexcept {
    return y(v.components);
  }

  const v_f32_4::ValueType z(const v_f32_4 &v) noexcept {
    return z(v.components);
  }

  const v_f32_4::ValueType w(const v_f32_4 &v) noexcept {
    return w(v.components);
  }

  const v_f32_4::ValueType x1(const v_f32_4 &v) noexcept {
    return x1(v.components);
  }

  const v_f32_4::ValueType x2(const v_f32_4 &v) noexcept {
    return x2(v.components);
  }

  const v_f32_4::ValueType x3(const v_f32_4 &v) noexcept {
    return x3(v.components);
  }

  const v_f32_4::ValueType x4(const v_f32_4 &v) noexcept {
    return x4(v.components);
  }

  template <>
  const v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::X>(v.components, s);
  }

  template <>
  const v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::Y>(v.components, s);
  }

  template <>
  const v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::Z>(v.components, s);
  }

  template <>
  const v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::W>(v.components, s);
  }

  const v_f32_4 replaceX(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceX(v.components, s);
  }

  const v_f32_4 replaceY(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceY(v.components, s);
  }

  const v_f32_4 replaceZ(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceZ(v.components, s);
  }

  const v_f32_4 replaceW(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceW(v.components, s);
  }

  const v_f32_4 replaceX1(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceX1(v.components, s);
  }

  const v_f32_4 replaceX2(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceX2(v.components, s);
  }

  const v_f32_4 replaceX3(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceX3(v.components, s);
  }

  const v_f32_4 replaceX4(const v_f32_4 &v, const v_f32_4::ValueType s) noexcept {
    return replaceX4(v.components, s);
  }

  const v_f32_4 replaceComponent(const v_f32_4 &v, const v_f32_4 &replacement, const ASizeT index) noexcept {
    return replaceComponent(v.components, replacement.components, index);
  }

  const v_f32_4 replaceComponent(const v_f32_4 &v, const v_f32_4::ValueType replacement, const ASizeT index) noexcept {
    return replaceComponent(v.components, replacement, index);
  }
}