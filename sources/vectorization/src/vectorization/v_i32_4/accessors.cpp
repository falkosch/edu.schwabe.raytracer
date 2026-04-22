#include "vectorization/v_i32_4.h"

#include "vectorization/accessors/component_128i.h"
#include "vectorization/accessors/replace_component_128i.h"

namespace vectorization {
  template <>
  v_i32_4::ValueType component<VectorIndices::X>(const v_i32_4 &v) noexcept {
    return component<VectorIndices::X>(v.components);
  }

  template <>
  v_i32_4::ValueType component<VectorIndices::Y>(const v_i32_4 &v) noexcept {
    return component<VectorIndices::Y>(v.components);
  }

  template <>
  v_i32_4::ValueType component<VectorIndices::Z>(const v_i32_4 &v) noexcept {
    return component<VectorIndices::Z>(v.components);
  }

  template <>
  v_i32_4::ValueType component<VectorIndices::W>(const v_i32_4 &v) noexcept {
    return component<VectorIndices::W>(v.components);
  }

  v_i32_4::ValueType x(const v_i32_4 &v) noexcept {
    return x(v.components);
  }

  v_i32_4::ValueType y(const v_i32_4 &v) noexcept {
    return y(v.components);
  }

  v_i32_4::ValueType z(const v_i32_4 &v) noexcept {
    return z(v.components);
  }

  v_i32_4::ValueType w(const v_i32_4 &v) noexcept {
    return w(v.components);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::X>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::X>(v.components, s);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::Y>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::Y>(v.components, s);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::Z>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::Z>(v.components, s);
  }

  template <>
  v_i32_4 replaceComponent<VectorIndices::W>(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceComponent<VectorIndices::W>(v.components, s);
  }

  v_i32_4 replaceX(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceX(v.components, s);
  }

  v_i32_4 replaceY(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceY(v.components, s);
  }

  v_i32_4 replaceZ(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceZ(v.components, s);
  }

  v_i32_4 replaceW(const v_i32_4 &v, const v_i32_4::ValueType s) noexcept {
    return replaceW(v.components, s);
  }
}
