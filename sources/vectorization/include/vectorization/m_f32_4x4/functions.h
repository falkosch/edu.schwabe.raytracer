#pragma once

#include "type.h"

namespace vectorization {

  m_f32_4x4 inverse(const m_f32_4x4 &invertible) noexcept;

  m_f32_4x4 transpose(const m_f32_4x4 &matrix) noexcept;

  m_f32_4x4 translate(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &translation) noexcept;

  m_f32_4x4 scale(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &scale) noexcept;

  m_f32_4x4 rotate(
      const m_f32_4x4 &matrix, m_f32_4x4::ValueType angleInDegree, const m_f32_4x4::RowVectorType &rotationAxis
  ) noexcept;

  // Optimized version of glm:unproject: Doesn't take the projection and
  // view matrix separately but requires a precomputed inverse of the
  // view-projection-matrix. Also assumes that in viewport.z is "one over
  // viewport-width" and in viewport.w is "one over viewport-height" stored.
  m_f32_4x4::RowVectorType unproject(
      const m_f32_4x4::RowVectorType &screenPosition, const m_f32_4x4 &inverseViewProjectionMatrix,
      const m_f32_4x4::RowVectorType &viewport
  ) noexcept;

  m_f32_4x4 ortho(
      m_f32_4x4::ValueType left, m_f32_4x4::ValueType right, m_f32_4x4::ValueType top, m_f32_4x4::ValueType bottom
  ) noexcept;

  m_f32_4x4 perspectiveFov(
      m_f32_4x4::ValueType fov, m_f32_4x4::ValueType width, m_f32_4x4::ValueType height, m_f32_4x4::ValueType near,
      m_f32_4x4::ValueType far
  ) noexcept;
}
