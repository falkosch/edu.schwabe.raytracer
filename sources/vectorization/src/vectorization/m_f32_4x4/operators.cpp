#include "vectorization/m_f32_4x4.h"

#include "vectorization/functions/horizontal_add.h"

namespace vectorization {
  m_f32_4x4 operator*(const m_f32_4x4 &matrix, const m_f32_4x4::ValueType scale) noexcept {
    const auto broadcast = m_f32_4x4::RowVectorType(scale);
    return m_f32_4x4(
        matrix.row0 * broadcast, matrix.row1 * broadcast, matrix.row2 * broadcast, matrix.row3 * broadcast
    );
  }

  m_f32_4x4 operator*(const m_f32_4x4::ValueType scale, const m_f32_4x4 &matrix) noexcept {
    return matrix * scale;
  }

  m_f32_4x4 operator+(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept {
    return m_f32_4x4(a.row0 + b.row0, a.row1 + b.row1, a.row2 + b.row2, a.row3 + b.row3);
  }

  m_f32_4x4 operator*(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept {
    return m_f32_4x4(a.row0 * b, a.row1 * b, a.row2 * b, a.row3 * b);
  }

  m_f32_4x4::RowVectorType operator*(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &vector) noexcept {
    // dpps variant: 4x dpps (9-13cy) + 3x orps (1cy) = ~11-15cy chain (dpps parallel, then or-tree)
    // const auto d0 = _mm_dp_ps(vector.components, matrix.row0.components, 0xF1);
    // const auto d1 = _mm_dp_ps(vector.components, matrix.row1.components, 0xF2);
    // const auto d2 = _mm_dp_ps(vector.components, matrix.row2.components, 0xF4);
    // const auto d3 = _mm_dp_ps(vector.components, matrix.row3.components, 0xF8);
    // return _mm_or_ps(_mm_or_ps(d0, d1), _mm_or_ps(d2, d3));

    // hadd variant: 4x mulps (5cy) + 3x haddps (3-6cy) = ~11-17cy chain (mul parallel, then hadd-tree)
    return horizontalAdd(
        horizontalAdd((vector * matrix.row0).components, (vector * matrix.row1).components),
        horizontalAdd((vector * matrix.row2).components, (vector * matrix.row3).components)
    );
  }

  m_f32_4x4::ColumnVectorType operator*(const m_f32_4x4::ColumnVectorType &vector, const m_f32_4x4 &matrix) noexcept {
    return xxxx(vector) * matrix.row0 + yyyy(vector) * matrix.row1 + zzzz(vector) * matrix.row2
           + wwww(vector) * matrix.row3;
  }
}
