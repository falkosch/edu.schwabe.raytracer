#include "vectorization/m_f32_4x4.h"

namespace vectorization {
  const m_f32_4x4 operator*(const m_f32_4x4 &matrix, const m_f32_4x4::ValueType scale) noexcept {
    const auto broadcast = m_f32_4x4::RowVectorType(scale);
    return m_f32_4x4(
        matrix.row0 * broadcast, matrix.row1 * broadcast, matrix.row2 * broadcast, matrix.row3 * broadcast
    );
  }

  const m_f32_4x4 operator*(const m_f32_4x4::ValueType scale, const m_f32_4x4 &matrix) noexcept {
    return matrix * scale;
  }

  const m_f32_4x4 operator+(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept {
    return m_f32_4x4(a.row0 + b.row0, a.row1 + b.row1, a.row2 + b.row2, a.row3 + b.row3);
  }

  const m_f32_4x4 operator*(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept {
    return m_f32_4x4(a.row0 * b, a.row1 * b, a.row2 * b, a.row3 * b);
  }

  const m_f32_4x4::RowVectorType operator*(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &vector) noexcept {
    // #if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    //         const auto vv = _mm256_broadcast_ps(&v.components);
    //         const auto ha_02_13 = _mm256_hadd_ps(
    //             _mm256_mul_ps(vv, _mm256_load_ps(reinterpret_cast<const m_f32_4x4::ValueType * const>(&m.row0))),
    //             _mm256_mul_ps(vv, _mm256_load_ps(reinterpret_cast<const m_f32_4x4::ValueType * const>(&m.row2))));
    //         //_mm256_mul_ps(vv, _mm256_set_m128(m.row1.components, m.row0.components)),
    //         //_mm256_mul_ps(vv, _mm256_set_m128(m.row3.components, m.row2.components)));
    //         const auto r_0213_1302 = _mm256_hadd_ps(
    //             ha_02_13,
    //             _mm256_permute2f128_ps(ha_02_13, ha_02_13, 0x01));
    //         return _mm256_castps256_ps128(_mm256_permute_ps(
    //             r_0213_1302,
    //             _MM_SHUFFLE(VectorIndices::W, VectorIndices::Y, VectorIndices::Z, VectorIndices::X)));
    // #else
    return _mm_hadd_ps(
        _mm_hadd_ps((vector * matrix.row0).components, (vector * matrix.row1).components),
        _mm_hadd_ps((vector * matrix.row2).components, (vector * matrix.row3).components)
    );
    // #endif
  }

  const m_f32_4x4::ColumnVectorType
  operator*(const m_f32_4x4::ColumnVectorType &vector, const m_f32_4x4 &matrix) noexcept {
    return (xxxx(vector) * matrix.row0 + yyyy(vector) * matrix.row1)
           + (zzzz(vector) * matrix.row2 + wwww(vector) * matrix.row3);
  }
}
