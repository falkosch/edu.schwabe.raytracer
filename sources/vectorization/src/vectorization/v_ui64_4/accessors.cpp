#include "vectorization/v_ui64_4/accessors.h"

namespace vectorization {
  template <>
  v_ui64_4::ValueType component<VectorIndices::X>(const v_ui64_4 &v) noexcept { return v[VectorIndices::X]; }

  template <>
  v_ui64_4::ValueType component<VectorIndices::Y>(const v_ui64_4 &v) noexcept { return v[VectorIndices::Y]; }

  template <>
  v_ui64_4::ValueType component<VectorIndices::Z>(const v_ui64_4 &v) noexcept { return v[VectorIndices::Z]; }

  template <>
  v_ui64_4::ValueType component<VectorIndices::W>(const v_ui64_4 &v) noexcept { return v[VectorIndices::W]; }

  v_ui64_4::ValueType x(const v_ui64_4 &v) noexcept { return component<VectorIndices::X>(v); }
  v_ui64_4::ValueType y(const v_ui64_4 &v) noexcept { return component<VectorIndices::Y>(v); }
  v_ui64_4::ValueType z(const v_ui64_4 &v) noexcept { return component<VectorIndices::Z>(v); }
  v_ui64_4::ValueType w(const v_ui64_4 &v) noexcept { return component<VectorIndices::W>(v); }

  template <>
  v_ui64_4 replaceComponent<VectorIndices::X>(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept {
    v_ui64_4 r = v; r[VectorIndices::X] = s; return r;
  }

  template <>
  v_ui64_4 replaceComponent<VectorIndices::Y>(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept {
    v_ui64_4 r = v; r[VectorIndices::Y] = s; return r;
  }

  template <>
  v_ui64_4 replaceComponent<VectorIndices::Z>(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept {
    v_ui64_4 r = v; r[VectorIndices::Z] = s; return r;
  }

  template <>
  v_ui64_4 replaceComponent<VectorIndices::W>(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept {
    v_ui64_4 r = v; r[VectorIndices::W] = s; return r;
  }

  v_ui64_4 replaceX(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::X>(v, s); }
  v_ui64_4 replaceY(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::Y>(v, s); }
  v_ui64_4 replaceZ(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::Z>(v, s); }
  v_ui64_4 replaceW(const v_ui64_4 &v, const v_ui64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::W>(v, s); }
}
