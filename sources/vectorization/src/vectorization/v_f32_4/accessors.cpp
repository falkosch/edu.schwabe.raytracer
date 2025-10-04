#include "vectorization/v_f32_4/accessors.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 &vector) noexcept {
    return component<VectorIndices::X>(vector.components);
  }

  template <>
  v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 &vector) noexcept {
    return component<VectorIndices::Y>(vector.components);
  }

  template <>
  v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 &vector) noexcept {
    return component<VectorIndices::Z>(vector.components);
  }

  template <>
  v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 &vector) noexcept {
    return component<VectorIndices::W>(vector.components);
  }

  v_f32_4::ValueType x(const v_f32_4 &vector) noexcept {
    return x(vector.components);
  }

  v_f32_4::ValueType y(const v_f32_4 &vector) noexcept {
    return y(vector.components);
  }

  v_f32_4::ValueType z(const v_f32_4 &vector) noexcept {
    return z(vector.components);
  }

  v_f32_4::ValueType w(const v_f32_4 &vector) noexcept {
    return w(vector.components);
  }

  v_f32_4::ValueType x1(const v_f32_4 &vector) noexcept {
    return x1(vector.components);
  }

  v_f32_4::ValueType x2(const v_f32_4 &vector) noexcept {
    return x2(vector.components);
  }

  v_f32_4::ValueType x3(const v_f32_4 &vector) noexcept {
    return x3(vector.components);
  }

  v_f32_4::ValueType x4(const v_f32_4 &vector) noexcept {
    return x4(vector.components);
  }

  template <>
  v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::X>(vector.components, scalar);
  }

  template <>
  v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Y>(vector.components, scalar);
  }

  template <>
  v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Z>(vector.components, scalar);
  }

  template <>
  v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::W>(vector.components, scalar);
  }

  v_f32_4 replaceX(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceX(vector.components, scalar);
  }

  v_f32_4 replaceY(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceY(vector.components, scalar);
  }

  v_f32_4 replaceZ(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceZ(vector.components, scalar);
  }

  v_f32_4 replaceW(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceW(vector.components, scalar);
  }

  v_f32_4 replaceX1(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceX1(vector.components, scalar);
  }

  v_f32_4 replaceX2(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceX2(vector.components, scalar);
  }

  v_f32_4 replaceX3(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceX3(vector.components, scalar);
  }

  v_f32_4 replaceX4(const v_f32_4 &vector, const v_f32_4::ValueType scalar) noexcept {
    return replaceX4(vector.components, scalar);
  }

  v_f32_4 replaceComponent(const v_f32_4 &vector, const v_f32_4 &replacement, const ASizeT index) noexcept {
    return replaceComponent(vector.components, replacement.components, index);
  }

  v_f32_4 replaceComponent(const v_f32_4 &vector, const v_f32_4::ValueType replacement, const ASizeT index) noexcept {
    return replaceComponent(vector.components, replacement, index);
  }
}
