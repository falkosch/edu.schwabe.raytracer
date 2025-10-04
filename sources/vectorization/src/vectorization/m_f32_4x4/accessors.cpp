#include "vectorization/m_f32_4x4.h"

namespace vectorization {
  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::X>(const m_f32_4x4 &matrix) noexcept {
    return matrix.row0;
  }

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::Y>(const m_f32_4x4 &matrix) noexcept {
    return matrix.row1;
  }

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::Z>(const m_f32_4x4 &matrix) noexcept {
    return matrix.row2;
  }

  template <>
  m_f32_4x4::RowVectorType row<VectorIndices::W>(const m_f32_4x4 &matrix) noexcept {
    return matrix.row3;
  }

  template <>
  m_f32_4x4 replaceRow<VectorIndices::X>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept {
    return m_f32_4x4(row, matrix.row1, matrix.row2, matrix.row3);
  }

  template <>
  m_f32_4x4 replaceRow<VectorIndices::Y>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept {
    return m_f32_4x4(matrix.row0, row, matrix.row2, matrix.row3);
  }

  template <>
  m_f32_4x4 replaceRow<VectorIndices::Z>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept {
    return m_f32_4x4(matrix.row0, matrix.row1, row, matrix.row3);
  }

  template <>
  m_f32_4x4 replaceRow<VectorIndices::W>(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &row) noexcept {
    return m_f32_4x4(matrix.row0, matrix.row1, matrix.row2, row);
  }

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::X>(const m_f32_4x4 &matrix) noexcept {
    return xz_xz(xy_xy(matrix.row0, matrix.row1), xy_xy(matrix.row2, matrix.row3));
  }

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::Y>(const m_f32_4x4 &matrix) noexcept {
    return xz_xz(yy_yy(matrix.row0, matrix.row1), yy_yy(matrix.row2, matrix.row3));
  }

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::Z>(const m_f32_4x4 &matrix) noexcept {
    return xz_xz(zw_zw(matrix.row0, matrix.row1), zw_zw(matrix.row2, matrix.row3));
  }

  template <>
  m_f32_4x4::ColumnVectorType column<VectorIndices::W>(const m_f32_4x4 &matrix) noexcept {
    return xz_xz(ww_ww(matrix.row0, matrix.row1), ww_ww(matrix.row2, matrix.row3));
  }

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::X>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept {
    return m_f32_4x4(
        replaceX(matrix.row0, x(column)), replaceX(matrix.row1, y(column)), replaceX(matrix.row2, z(column)),
        replaceX(matrix.row3, w(column))
    );
  }

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::Y>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept {
    return m_f32_4x4(
        replaceY(matrix.row0, x(column)), replaceY(matrix.row1, y(column)), replaceY(matrix.row2, z(column)),
        replaceY(matrix.row3, w(column))
    );
  }

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::Z>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept {
    return m_f32_4x4(
        replaceZ(matrix.row0, x(column)), replaceZ(matrix.row1, y(column)), replaceZ(matrix.row2, z(column)),
        replaceZ(matrix.row3, w(column))
    );
  }

  template <>
  m_f32_4x4
  replaceColumn<VectorIndices::W>(const m_f32_4x4 &matrix, const m_f32_4x4::ColumnVectorType &column) noexcept {
    return m_f32_4x4(
        replaceW(matrix.row0, x(column)), replaceW(matrix.row1, y(column)), replaceW(matrix.row2, z(column)),
        replaceW(matrix.row3, w(column))
    );
  }
}
