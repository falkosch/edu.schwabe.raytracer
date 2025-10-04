#include "vectorization/v_f32_4x4.h"

namespace vectorization {
  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::X>(const v_f32_4x4 &m) noexcept {
    return m.soaX;
  }

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::Y>(const v_f32_4x4 &m) noexcept {
    return m.soaY;
  }

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::Z>(const v_f32_4x4 &m) noexcept {
    return m.soaZ;
  }

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::W>(const v_f32_4x4 &m) noexcept {
    return m.soaW;
  }

  template <>
  void soa<VectorIndices::X>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaX = v;
  }

  template <>
  void soa<VectorIndices::Y>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaY = v;
  }

  template <>
  void soa<VectorIndices::Z>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaZ = v;
  }

  template <>
  void soa<VectorIndices::W>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaW = v;
  }
}
