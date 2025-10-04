#pragma once

#include "type.h"

namespace vectorization {
  m_f32_4x4 operator*(const m_f32_4x4 &matrix, m_f32_4x4::ValueType scale) noexcept;
  m_f32_4x4 operator*(m_f32_4x4::ValueType scale, const m_f32_4x4 &matrix) noexcept;

  m_f32_4x4 operator+(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept;

  // matrix multiplication of two given matrices
  m_f32_4x4 operator*(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept;

  // matrix times vector transformation,
  m_f32_4x4::RowVectorType operator*(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &vector) noexcept;

  // vector time matrix transformation,
  m_f32_4x4::ColumnVectorType operator*(const m_f32_4x4::ColumnVectorType &vector, const m_f32_4x4 &matrix) noexcept;
}
