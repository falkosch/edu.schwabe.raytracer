#include "vectorization/m_f32_4x4.h"

#include "vectorization/functions.h"

#include <array>
#include <cassert>

namespace vectorization {
  auto SIGN_MASKR =
      blend<false, true, false, true>(Zero<m_f32_4x4::RowVectorType>(), NegativeZero<m_f32_4x4::RowVectorType>());

#pragma region m_f32_4x4::
  //{ m_f32_4x4::

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

  m_f32_4x4::m_f32_4x4(const std::array<ValueType, SIZE> &m) noexcept
      : row0(&m[VectorIndices::X * COLUMNS]), row1(&m[VectorIndices::Y * COLUMNS]),
        row2(&m[VectorIndices::Z * COLUMNS]), row3(&m[VectorIndices::W * COLUMNS]) {
  }

  m_f32_4x4::m_f32_4x4(const m_f32_4x4::ValueType *const m) noexcept
      : row0(m + VectorIndices::X * COLUMNS), row1(m + VectorIndices::Y * COLUMNS),
        row2(m + VectorIndices::Z * COLUMNS), row3(m + VectorIndices::W * COLUMNS) {
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

  //}
#pragma endregion

#pragma region Constant generators
  //{ Constant generators

  template <>
  const m_f32_4x4 Zero<m_f32_4x4>() noexcept {
    return m_f32_4x4(Zero<m_f32_4x4::PackedType>());
  }

  template <>
  const m_f32_4x4 Identity<m_f32_4x4>() noexcept {
    return m_f32_4x4();
  }

  //}
#pragma endregion

#pragma region Operators
  //{ Operators

  const m_f32_4x4 operator*(const m_f32_4x4 &m, const m_f32_4x4::ValueType v) noexcept {
    auto rv = m_f32_4x4::RowVectorType(v);
    return m_f32_4x4(m.row0 * rv, m.row1 * rv, m.row2 * rv, m.row3 * rv);
  }

  const m_f32_4x4 operator*(const m_f32_4x4::ValueType v, const m_f32_4x4 &m) noexcept {
    return m * v;
  }

  const m_f32_4x4 operator+(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept {
    return m_f32_4x4(a.row0 + b.row0, a.row1 + b.row1, a.row2 + b.row2, a.row3 + b.row3);
  }

  const m_f32_4x4 operator*(const m_f32_4x4 &a, const m_f32_4x4 &b) noexcept {
    return m_f32_4x4(a.row0 * b, a.row1 * b, a.row2 * b, a.row3 * b);
  }

  const m_f32_4x4::RowVectorType operator*(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &v) noexcept {
    // #if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    //         const PackedFloat8_256 vv = _mm256_broadcast_ps(&v.components);
    //         const PackedFloat8_256 ha_02_13 = _mm256_hadd_ps(
    //             _mm256_mul_ps(vv, _mm256_load_ps(reinterpret_cast<const m_f32_4x4::ValueType * const>(&m.row0))),
    //             _mm256_mul_ps(vv, _mm256_load_ps(reinterpret_cast<const m_f32_4x4::ValueType * const>(&m.row2))));
    //         //_mm256_mul_ps(vv, _mm256_set_m128(m.row1.components, m.row0.components)),
    //         //_mm256_mul_ps(vv, _mm256_set_m128(m.row3.components, m.row2.components)));
    //         const PackedFloat8_256 r_0213_1302 = _mm256_hadd_ps(
    //             ha_02_13,
    //             _mm256_permute2f128_ps(ha_02_13, ha_02_13, 0x01));
    //         return _mm256_castps256_ps128(_mm256_permute_ps(
    //             r_0213_1302,
    //             _MM_SHUFFLE(VectorIndices::W, VectorIndices::Y, VectorIndices::Z, VectorIndices::X)));
    // #else
    return _mm_hadd_ps(
        _mm_hadd_ps((v * m.row0).components, (v * m.row1).components),
        _mm_hadd_ps((v * m.row2).components, (v * m.row3).components)
    );
    // #endif
  }

  const m_f32_4x4::ColumnVectorType operator*(const m_f32_4x4::ColumnVectorType &v, const m_f32_4x4 &m) noexcept {
    return (xxxx(v) * m.row0 + yyyy(v) * m.row1) + (zzzz(v) * m.row2 + wwww(v) * m.row3);
  }

  //}
#pragma endregion

#pragma region row()
  //{ row()

  template <>
  const m_f32_4x4::RowVectorType row<VectorIndices::X>(const m_f32_4x4 &m) noexcept {
    return m.row0;
  }

  template <>
  const m_f32_4x4::RowVectorType row<VectorIndices::Y>(const m_f32_4x4 &m) noexcept {
    return m.row1;
  }

  template <>
  const m_f32_4x4::RowVectorType row<VectorIndices::Z>(const m_f32_4x4 &m) noexcept {
    return m.row2;
  }

  template <>
  const m_f32_4x4::RowVectorType row<VectorIndices::W>(const m_f32_4x4 &m) noexcept {
    return m.row3;
  }

  //}
#pragma endregion

#pragma region replaceRow()
  //{ replaceRow()

  template <>
  const m_f32_4x4 replaceRow<VectorIndices::X>(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &v) noexcept {
    return m_f32_4x4(v, m.row1, m.row2, m.row3);
  }

  template <>
  const m_f32_4x4 replaceRow<VectorIndices::Y>(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &v) noexcept {
    return m_f32_4x4(m.row0, v, m.row2, m.row3);
  }

  template <>
  const m_f32_4x4 replaceRow<VectorIndices::Z>(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &v) noexcept {
    return m_f32_4x4(m.row0, m.row1, v, m.row3);
  }

  template <>
  const m_f32_4x4 replaceRow<VectorIndices::W>(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &v) noexcept {
    return m_f32_4x4(m.row0, m.row1, m.row2, v);
  }

  //}
#pragma endregion

#pragma region column()
  //{ column()

  template <>
  const m_f32_4x4::ColumnVectorType column<VectorIndices::X>(const m_f32_4x4 &m) noexcept {
    return m_f32_4x4::ColumnVectorType(
        component<VectorIndices::X>(m.row0), component<VectorIndices::X>(m.row1), component<VectorIndices::X>(m.row2),
        component<VectorIndices::X>(m.row3)
    );
  }

  template <>
  const m_f32_4x4::ColumnVectorType column<VectorIndices::Y>(const m_f32_4x4 &m) noexcept {
    return m_f32_4x4::ColumnVectorType(
        component<VectorIndices::Y>(m.row0), component<VectorIndices::Y>(m.row1), component<VectorIndices::Y>(m.row2),
        component<VectorIndices::Y>(m.row3)
    );
  }

  template <>
  const m_f32_4x4::ColumnVectorType column<VectorIndices::Z>(const m_f32_4x4 &m) noexcept {
    return m_f32_4x4::ColumnVectorType(
        component<VectorIndices::Z>(m.row0), component<VectorIndices::Z>(m.row1), component<VectorIndices::Z>(m.row2),
        component<VectorIndices::Z>(m.row3)
    );
  }

  template <>
  const m_f32_4x4::ColumnVectorType column<VectorIndices::W>(const m_f32_4x4 &m) noexcept {
    return m_f32_4x4::ColumnVectorType(
        component<VectorIndices::W>(m.row0), component<VectorIndices::W>(m.row1), component<VectorIndices::W>(m.row2),
        component<VectorIndices::W>(m.row3)
    );
  }

  //}
#pragma endregion

#pragma region replaceColumn()
  //{ replaceColumn()

  template <>
  const m_f32_4x4 replaceColumn<VectorIndices::X>(const m_f32_4x4 &m, const m_f32_4x4::ColumnVectorType &v) noexcept {
    return m_f32_4x4(replaceX(m.row0, x(v)), replaceX(m.row1, y(v)), replaceX(m.row2, z(v)), replaceX(m.row3, w(v)));
  }

  template <>
  const m_f32_4x4 replaceColumn<VectorIndices::Y>(const m_f32_4x4 &m, const m_f32_4x4::ColumnVectorType &v) noexcept {
    return m_f32_4x4(replaceY(m.row0, x(v)), replaceY(m.row1, y(v)), replaceY(m.row2, z(v)), replaceY(m.row3, w(v)));
  }

  template <>
  const m_f32_4x4 replaceColumn<VectorIndices::Z>(const m_f32_4x4 &m, const m_f32_4x4::ColumnVectorType &v) noexcept {
    return m_f32_4x4(replaceZ(m.row0, x(v)), replaceZ(m.row1, y(v)), replaceZ(m.row2, z(v)), replaceZ(m.row3, w(v)));
  }

  template <>
  const m_f32_4x4 replaceColumn<VectorIndices::W>(const m_f32_4x4 &m, const m_f32_4x4::ColumnVectorType &v) noexcept {
    return m_f32_4x4(replaceW(m.row0, x(v)), replaceW(m.row1, y(v)), replaceW(m.row2, z(v)), replaceW(m.row3, w(v)));
  }

  //}
#pragma endregion

#pragma region inverse()
  //{ inverse()

  const m_f32_4x4 inverse(const m_f32_4x4 &m) noexcept {
    auto r2_XX_r1 = xx_xx(m.row2, m.row1);
    auto r2_YY_r1 = yy_yy(m.row2, m.row1);
    auto r2_ZZ_r1 = zz_zz(m.row2, m.row1);
    auto r2_WW_r1 = ww_ww(m.row2, m.row1);
    auto r3_xxxX_r2 = xxxz(xx_xx(m.row3, m.row2));
    auto r3_yyyY_r2 = xxxz(yy_yy(m.row3, m.row2));
    auto r3_zzzZ_r2 = xxxz(zz_zz(m.row3, m.row2));
    auto r3_wwwW_r2 = xxxz(ww_ww(m.row3, m.row2));

    auto f0 = r3_wwwW_r2 * r2_ZZ_r1 - r3_zzzZ_r2 * r2_WW_r1;
    auto f1 = r3_wwwW_r2 * r2_YY_r1 - r3_yyyY_r2 * r2_WW_r1;
    auto f2 = r3_zzzZ_r2 * r2_YY_r1 - r3_yyyY_r2 * r2_ZZ_r1;
    auto f3 = r3_wwwW_r2 * r2_XX_r1 - r3_xxxX_r2 * r2_WW_r1;
    auto f4 = r3_zzzZ_r2 * r2_XX_r1 - r3_xxxX_r2 * r2_ZZ_r1;
    auto f5 = r3_yyyY_r2 * r2_XX_r1 - r3_xxxX_r2 * r2_YY_r1;

    auto v0 = xzzz(xx_xx(m.row1, m.row0));
    auto v1 = xzzz(yy_yy(m.row1, m.row0));
    auto v2 = xzzz(zz_zz(m.row1, m.row0));
    auto v3 = xzzz(ww_ww(m.row1, m.row0));

    auto i0 = v2 * f1 - v1 * f0 - v3 * f2;
    auto i1 = v0 * f0 - v2 * f3 + v3 * f4;
    auto i2 = v1 * f3 - v0 * f1 - v3 * f5;
    auto i3 = v0 * f2 - v1 * f4 + v2 * f5;

    auto d = reciprocal(dotv(m.row0, xz_xz(xy_xy(i0, i1), xy_xy(i2, i3)))) ^ SIGN_MASKR;
    return m_f32_4x4(i0 * d, i1 * d, i2 * d, i3 * d);
  }

  //}
#pragma endregion

#pragma region transpose()
  //{ transpose()

  const m_f32_4x4 transpose(const m_f32_4x4 &m) noexcept {
    auto XY_XY_01 = xy_xy(m.row0, m.row1);
    auto ZW_ZW_01 = zw_zw(m.row0, m.row1);
    auto XY_XY_23 = xy_xy(m.row2, m.row3);
    auto ZW_ZW_23 = zw_zw(m.row2, m.row3);
    return m_f32_4x4(
        xz_xz(XY_XY_01, XY_XY_23), yw_yw(XY_XY_01, XY_XY_23), xz_xz(ZW_ZW_01, ZW_ZW_23), yw_yw(ZW_ZW_01, ZW_ZW_23)
    );
  }

  //}
#pragma endregion

#pragma region translate()
  //{ translate()

  const m_f32_4x4 translate(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &translation) noexcept {
    auto transformedTranslation = transpose(m) * oneW(translation);
    return m_f32_4x4(
        replaceW(m.row0, x(transformedTranslation)), replaceW(m.row1, y(transformedTranslation)),
        replaceW(m.row2, z(transformedTranslation)), replaceW(m.row3, w(transformedTranslation))
    );
  }

  //}
#pragma endregion

#pragma region scale()
  //{ scale()

  const m_f32_4x4 scale(const m_f32_4x4 &m, const m_f32_4x4::RowVectorType &scale) noexcept {
    auto transposedM = transpose(m);
    return transpose(m_f32_4x4(
        row<VectorIndices::X>(transposedM) * xxxx(scale), row<VectorIndices::Y>(transposedM) * yyyy(scale),
        row<VectorIndices::Z>(transposedM) * zzzz(scale), row<VectorIndices::W>(transposedM)
    ));
  }

  //}
#pragma endregion

#pragma region rotate()
  //{ rotate()

  const m_f32_4x4
  rotate(const m_f32_4x4 &m, const m_f32_4x4::ValueType angle, const m_f32_4x4::RowVectorType &rotationAxis) noexcept {
    // adapted from glm::detail::tmat4x4<T>.rotate(...)
    auto rad = angle * DegreeToRadian<m_f32_4x4::ValueType>();
    auto s = sin(rad);
    auto rot = m_f32_4x4::RowVectorType(cos(rad), s, -s);
    auto axis = oneW(normalize(rotationAxis));
    auto temp = (One<m_f32_4x4::RowVectorType>() - xxxx(rot)) * axis;

    auto X = xxxx(temp) * axis + rot * wzyx(axis);
    auto Y = yyyy(temp) * axis + zxyw(rot) * zwxy(axis);
    auto Z = zzzz(temp) * axis + yzxw(rot) * yxwz(axis);

    auto mt = transpose(m);
    return transpose(m_f32_4x4(
        row<VectorIndices::X>(mt) * xxxx(X) + row<VectorIndices::Y>(mt) * yyyy(X) + row<VectorIndices::Z>(mt) * zzzz(X),
        row<VectorIndices::X>(mt) * xxxx(Y) + row<VectorIndices::Y>(mt) * yyyy(Y) + row<VectorIndices::Z>(mt) * zzzz(Y),
        row<VectorIndices::X>(mt) * xxxx(Z) + row<VectorIndices::Y>(mt) * yyyy(Z) + row<VectorIndices::Z>(mt) * zzzz(Z),
        row<VectorIndices::W>(mt)
    ));
  }

  //}
#pragma endregion

#pragma region unproject()
  //{ unproject()

  const m_f32_4x4::RowVectorType unproject(
      const m_f32_4x4::RowVectorType &screenPosition, const m_f32_4x4 &inverseViewProjectionMatrix,
      const m_f32_4x4::RowVectorType &viewport
  ) noexcept {
    auto affineScreenPoint = oneW(screenPosition);

    // normalize x and y of affineScreenPoint using the offset in viewport.xy and the scale in viewport.zw
    auto normalizedScreenXY = (affineScreenPoint - viewport) * zwxy(viewport);
    auto transformed = inverseViewProjectionMatrix * xy_zw(normalizedScreenXY, affineScreenPoint);
    return transformed / wwww(transformed);
  }

  //}
#pragma endregion

#pragma region ortho()
  //{ ortho()

  const m_f32_4x4 ortho(
      const m_f32_4x4::ValueType left, const m_f32_4x4::ValueType right, const m_f32_4x4::ValueType top,
      const m_f32_4x4::ValueType bottom
  ) noexcept {
    typedef m_f32_4x4::ValueType T;
    typedef m_f32_4x4::RowVectorType RV;

    auto rRL = reciprocal(right - left);
    auto rTB = reciprocal(top - bottom);
    return m_f32_4x4(
        RV(rRL + rRL, Zero<T>(), Zero<T>(), Zero<T>()), RV(Zero<T>(), rTB + rTB, Zero<T>(), Zero<T>()),
        RV(Zero<T>(), Zero<T>(), NegativeOne<T>(), Zero<T>()),
        -RV((right + left) * rRL, (top + bottom) * rTB, NegativeZero<T>(), NegativeOne<T>())
    );
  }

  //}
#pragma endregion

#pragma region perspectiveFov()
  //{ perspectiveFov()

  const m_f32_4x4 perspectiveFov(
      const m_f32_4x4::ValueType fov, const m_f32_4x4::ValueType width, const m_f32_4x4::ValueType height,
      const m_f32_4x4::ValueType near, const m_f32_4x4::ValueType far
  ) noexcept {
    // Code adapted from glm::perspectiveFov.
    // see: http://schabby.de/projection-matrix/
    typedef m_f32_4x4::ValueType T;
    typedef m_f32_4x4::RowVectorType RV;

    auto H = -reciprocal(tan(Half<T>() * DegreeToRadian<T>() * fov));
    auto R = reciprocal(near - far);
    auto NF = near * far;
    return m_f32_4x4(
        replaceX(Zero<RV>(), H * height / width), replaceY(Zero<RV>(), H),
        RV(Zero<T>(), Zero<T>(), far + near, NF + NF) * R, OneZ<RV>()
    );
  }

  //}
#pragma endregion
}