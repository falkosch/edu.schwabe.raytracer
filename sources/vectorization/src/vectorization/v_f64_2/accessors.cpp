#include "vectorization/v_f64_2/accessors.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/replace_component_128d.h"

namespace vectorization {
  template <>
  v_f64_2::ValueType component<VectorIndices::X>(const v_f64_2 &vector) noexcept {
    return component<VectorIndices::X>(vector.components);
  }

  template <>
  v_f64_2::ValueType component<VectorIndices::Y>(const v_f64_2 &vector) noexcept {
    return component<VectorIndices::Y>(vector.components);
  }

  v_f64_2::ValueType x(const v_f64_2 &vector) noexcept {
    return x(vector.components);
  }

  v_f64_2::ValueType y(const v_f64_2 &vector) noexcept {
    return y(vector.components);
  }

  v_f64_2::ValueType x1(const v_f64_2 &vector) noexcept {
    return x1(vector.components);
  }

  v_f64_2::ValueType x2(const v_f64_2 &vector) noexcept {
    return x2(vector.components);
  }

  template <>
  v_f64_2 replaceComponent<VectorIndices::X>(const v_f64_2 &vector, const v_f64_2::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::X>(vector.components, scalar);
  }

  template <>
  v_f64_2 replaceComponent<VectorIndices::Y>(const v_f64_2 &vector, const v_f64_2::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Y>(vector.components, scalar);
  }

  v_f64_2 replaceX(const v_f64_2 &vector, const v_f64_2::ValueType scalar) noexcept {
    return replaceX(vector.components, scalar);
  }

  v_f64_2 replaceY(const v_f64_2 &vector, const v_f64_2::ValueType scalar) noexcept {
    return replaceY(vector.components, scalar);
  }

  v_f64_2 replaceX1(const v_f64_2 &vector, const v_f64_2::ValueType scalar) noexcept {
    return replaceX1(vector.components, scalar);
  }

  v_f64_2 replaceX2(const v_f64_2 &vector, const v_f64_2::ValueType scalar) noexcept {
    return replaceX2(vector.components, scalar);
  }

  v_f64_2 replaceComponent(const v_f64_2 &vector, const v_f64_2 &replacement, const ASizeT index) noexcept {
    return replaceComponent(vector.components, replacement.components, index);
  }

  v_f64_2 replaceComponent(const v_f64_2 &vector, const v_f64_2::ValueType replacement, const ASizeT index) noexcept {
    return replaceComponent(vector.components, replacement, index);
  }
}
