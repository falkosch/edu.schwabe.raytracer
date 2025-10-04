#include "vectorization/m_f32_4x4/type.h"

#include <array>
#include <cassert>

namespace vectorization {

  m_f32_4x4::m_f32_4x4() noexcept
      : row0(OneX<m_f32_4x4::RowVectorType>()), row1(OneY<m_f32_4x4::RowVectorType>()),
        row2(OneZ<m_f32_4x4::RowVectorType>()), row3(OneW<m_f32_4x4::RowVectorType>()) {
  }

  m_f32_4x4::m_f32_4x4(
      const m_f32_4x4::RowVectorType &row0In, const m_f32_4x4::RowVectorType &row1In,
      const m_f32_4x4::RowVectorType &row2In, const m_f32_4x4::RowVectorType &row3In
  ) noexcept
      : row0(row0In), row1(row1In), row2(row2In), row3(row3In) {
  }

  m_f32_4x4::m_f32_4x4(const m_f32_4x4::RowVectorType::PackedType &row) noexcept
      : row0(row), row1(row), row2(row), row3(row) {
  }

  m_f32_4x4::m_f32_4x4(const m_f32_4x4::RowVectorType &row) noexcept : row0(row), row1(row), row2(row), row3(row) {
  }

  m_f32_4x4::m_f32_4x4(const std::array<ValueType, SIZE> &data) noexcept
      : row0(&data[VectorIndices::X * COLUMNS]), row1(&data[VectorIndices::Y * COLUMNS]),
        row2(&data[VectorIndices::Z * COLUMNS]), row3(&data[VectorIndices::W * COLUMNS]) {
  }

  m_f32_4x4::m_f32_4x4(const m_f32_4x4::ValueType *const values) noexcept
      : row0(values + VectorIndices::X * COLUMNS), row1(values + VectorIndices::Y * COLUMNS),
        row2(values + VectorIndices::Z * COLUMNS), row3(values + VectorIndices::W * COLUMNS) {
  }

  m_f32_4x4 &m_f32_4x4::operator=(const m_f32_4x4::RowVectorType::PackedType &row) noexcept {
    row0 = row;
    row1 = row;
    row2 = row;
    row3 = row;
    return *this;
  }

  m_f32_4x4::ValueType &m_f32_4x4::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < m_f32_4x4::SIZE);
    return reinterpret_cast<m_f32_4x4::ValueType *const>(this)[index];
  }

  const m_f32_4x4::ValueType &m_f32_4x4::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < m_f32_4x4::SIZE);
    return reinterpret_cast<const m_f32_4x4::ValueType *const>(this)[index];
  }

  m_f32_4x4::ValueType &m_f32_4x4::operator[](const ASizeT index) noexcept {
    assert(index < m_f32_4x4::SIZE);
    return reinterpret_cast<m_f32_4x4::ValueType *const>(this)[index];
  }

  const m_f32_4x4::ValueType &m_f32_4x4::operator[](const ASizeT index) const noexcept {
    assert(index < m_f32_4x4::SIZE);
    return reinterpret_cast<const m_f32_4x4::ValueType *const>(this)[index];
  }
}
