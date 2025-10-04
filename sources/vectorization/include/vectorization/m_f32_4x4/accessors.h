#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT RowIndex>
  m_f32_4x4::RowVectorType row(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::X>(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::Y>(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::Z>(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::W>(const m_f32_4x4 &matrix) noexcept;

  template <ASizeT RowIndex>
  m_f32_4x4 replaceRow(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept;

  template <>
  m_f32_4x4 replaceRow<VectorIndices::X>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept;

  template <>
  m_f32_4x4 replaceRow<VectorIndices::Y>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept;

  template <>
  m_f32_4x4 replaceRow<VectorIndices::Z>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept;

  template <>
  m_f32_4x4 replaceRow<VectorIndices::W>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept;

  template <ASizeT ColumnIndex>
  m_f32_4x4::ColumnVectorType column(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::X>(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::Y>(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::Z>(const m_f32_4x4 &matrix) noexcept;

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::W>(const m_f32_4x4 &matrix) noexcept;

  template <ASizeT ColumnIndex>
  m_f32_4x4 replaceColumn(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept;

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::X>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept;

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::Y>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept;

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::Z>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept;

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::W>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept;
}
