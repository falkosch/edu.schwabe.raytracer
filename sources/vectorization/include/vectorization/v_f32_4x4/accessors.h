#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT Index>
  v_f32_4x4::AOSVectorType aos(const v_f32_4x4 &m) noexcept {
    static_assert(Index < v_f32_4x4::SIZE_AOS, "AOS index is out of range");
    return v_f32_4x4::AOSVectorType(
        component<Index>(m.soaX), component<Index>(m.soaY), component<Index>(m.soaZ), component<Index>(m.soaW)
    );
  }

  template <ASizeT Index>
  void aos(v_f32_4x4 &m, const v_f32_4x4::AOSVectorType &v) noexcept {
    static_assert(Index < v_f32_4x4::SIZE_AOS, "AOS index is out of range");
    component<Index>(m.soaX, component<VectorIndices::X>(v));
    component<Index>(m.soaY, component<VectorIndices::Y>(v));
    component<Index>(m.soaZ, component<VectorIndices::Z>(v));
    component<Index>(m.soaW, component<VectorIndices::W>(v));
  }

  template <ASizeT Index>
  v_f32_4x4::SOAVectorType soa(const v_f32_4x4 &m) noexcept;

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::X>(const v_f32_4x4 &m) noexcept;

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::Y>(const v_f32_4x4 &m) noexcept;

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::Z>(const v_f32_4x4 &m) noexcept;

  template <>
  v_f32_4x4::SOAVectorType soa<VectorIndices::W>(const v_f32_4x4 &m) noexcept;

  template <ASizeT Index>
  void soa(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept;

  template <>
  void soa<VectorIndices::X>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept;

  template <>
  void soa<VectorIndices::Y>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept;

  template <>
  void soa<VectorIndices::Z>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept;

  template <>
  void soa<VectorIndices::W>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept;
}
