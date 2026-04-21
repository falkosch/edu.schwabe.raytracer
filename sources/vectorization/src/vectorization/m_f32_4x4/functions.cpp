#include "vectorization/m_f32_4x4.h"

#include "vectorization/functions/cos.h"
#include "vectorization/functions/reciprocal.h"
#include "vectorization/functions/sin.h"
#include "vectorization/functions/tan.h"

#include <cassert>

namespace vectorization {
  m_f32_4x4 inverse(const m_f32_4x4 &invertible) noexcept {
    const auto SIGN_MASKR =
        blend<false, true, false, true>(Zero<m_f32_4x4::RowVectorType>(), NegativeZero<m_f32_4x4::RowVectorType>());

    const auto r2_XX_r1 = xx_xx(invertible.row2, invertible.row1);
    const auto r2_YY_r1 = yy_yy(invertible.row2, invertible.row1);
    const auto r2_ZZ_r1 = zz_zz(invertible.row2, invertible.row1);
    const auto r2_WW_r1 = ww_ww(invertible.row2, invertible.row1);
    const auto r3_xxxX_r2 = xxxz(xx_xx(invertible.row3, invertible.row2));
    const auto r3_yyyY_r2 = xxxz(yy_yy(invertible.row3, invertible.row2));
    const auto r3_zzzZ_r2 = xxxz(zz_zz(invertible.row3, invertible.row2));
    const auto r3_wwwW_r2 = xxxz(ww_ww(invertible.row3, invertible.row2));

    const auto f0 = r3_wwwW_r2 * r2_ZZ_r1 - r3_zzzZ_r2 * r2_WW_r1;
    const auto f1 = r3_wwwW_r2 * r2_YY_r1 - r3_yyyY_r2 * r2_WW_r1;
    const auto f2 = r3_zzzZ_r2 * r2_YY_r1 - r3_yyyY_r2 * r2_ZZ_r1;
    const auto f3 = r3_wwwW_r2 * r2_XX_r1 - r3_xxxX_r2 * r2_WW_r1;
    const auto f4 = r3_zzzZ_r2 * r2_XX_r1 - r3_xxxX_r2 * r2_ZZ_r1;
    const auto f5 = r3_yyyY_r2 * r2_XX_r1 - r3_xxxX_r2 * r2_YY_r1;

    const auto v0 = xzzz(xx_xx(invertible.row1, invertible.row0));
    const auto v1 = xzzz(yy_yy(invertible.row1, invertible.row0));
    const auto v2 = xzzz(zz_zz(invertible.row1, invertible.row0));
    const auto v3 = xzzz(ww_ww(invertible.row1, invertible.row0));

    const auto i0 = v2 * f1 - v1 * f0 - v3 * f2;
    const auto i1 = v0 * f0 - v2 * f3 + v3 * f4;
    const auto i2 = v1 * f3 - v0 * f1 - v3 * f5;
    const auto i3 = v0 * f2 - v1 * f4 + v2 * f5;

    const auto detv = dotv(invertible.row0, xz_xz(xy_xy(i0, i1), xy_xy(i2, i3)));
    assert(x(detv) != Zero<m_f32_4x4::ValueType>() && "inverse of singular matrix");
    const auto d = reciprocal(detv) ^ SIGN_MASKR;
    return m_f32_4x4(i0 * d, i1 * d, i2 * d, i3 * d);
  }

  m_f32_4x4 transpose(const m_f32_4x4 &matrix) noexcept {
    const auto XY_XY_01 = xy_xy(matrix.row0, matrix.row1);
    const auto ZW_ZW_01 = zw_zw(matrix.row0, matrix.row1);
    const auto XY_XY_23 = xy_xy(matrix.row2, matrix.row3);
    const auto ZW_ZW_23 = zw_zw(matrix.row2, matrix.row3);
    return m_f32_4x4(
        xz_xz(XY_XY_01, XY_XY_23), yw_yw(XY_XY_01, XY_XY_23), xz_xz(ZW_ZW_01, ZW_ZW_23), yw_yw(ZW_ZW_01, ZW_ZW_23)
    );
  }

  m_f32_4x4 translate(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &translation) noexcept {
    const auto transformedTranslation = oneW(translation) * matrix;
    return m_f32_4x4(
        replaceW(matrix.row0, x(transformedTranslation)), replaceW(matrix.row1, y(transformedTranslation)),
        replaceW(matrix.row2, z(transformedTranslation)), replaceW(matrix.row3, w(transformedTranslation))
    );
  }

  m_f32_4x4 scale(const m_f32_4x4 &matrix, const m_f32_4x4::RowVectorType &scale) noexcept {
    const auto s = oneW(scale);
    return m_f32_4x4(matrix.row0 * s, matrix.row1 * s, matrix.row2 * s, matrix.row3 * s);
  }

  m_f32_4x4 rotate(
      const m_f32_4x4 &matrix, const m_f32_4x4::ValueType angleInDegree, const m_f32_4x4::RowVectorType &rotationAxis
  ) noexcept {
    // adapted from glm::detail::tmat4x4<T>.rotate(...)
    const auto angleInRadian = angleInDegree * DegreeToRadian<m_f32_4x4::ValueType>();
    const auto cosinus = cos(angleInRadian);
    const auto sinus = sin(angleInRadian);
    const auto rot = m_f32_4x4::RowVectorType(cosinus, sinus, -sinus);
    assert(dot3(rotationAxis, rotationAxis) > Zero<m_f32_4x4::ValueType>() && "rotate with zero-length axis");
    const auto normal = oneW(normalize3(rotationAxis));
    // Use 2*sin^2(theta/2) instead of 1-cos(theta) to avoid catastrophic cancellation for small angles,
    // since 1-cos(theta) loses precision when cos(theta) is close to 1 (identity: 1-cos(t) = 2*sin^2(t/2)).
    const auto sinHalf = sin(angleInRadian * Half<m_f32_4x4::ValueType>());
    const auto oneMinusCos = Two<m_f32_4x4::ValueType>() * sinHalf * sinHalf;
    const auto temp = m_f32_4x4::RowVectorType(oneMinusCos) * normal;

    // tx/ty/tz are the columns of the 3x3 rotation matrix R.
    const auto tx = xxxx(temp) * normal + rot * wzyx(normal);
    const auto ty = yyyy(temp) * normal + zxyw(rot) * zwxy(normal);
    const auto tz = zzzz(temp) * normal + yzxw(rot) * yxwz(normal);

    // Transpose the 3x3 rotation to get R's rows (rx/ry/rz), replacing two full
    // 4x4 transposes with one partial 3x3 transpose.
    const auto xy_ct = xy_xy(tx, ty);
    const auto zw_ct = zw_zw(tx, ty);
    const auto xy_cz = xy_xy(tz, Zero<m_f32_4x4::RowVectorType>());
    const auto zw_cz = zw_zw(tz, Zero<m_f32_4x4::RowVectorType>());
    const auto rx = xz_xz(xy_ct, xy_cz);
    const auto ry = yw_yw(xy_ct, xy_cz);
    const auto rz = xz_xz(zw_ct, zw_cz);

    // Multiply each matrix row against R's rows via broadcast-multiply-add.
    // rx/ry/rz have W=0, so the sum leaves W=0; blend preserves the original W.
    return m_f32_4x4(
        blend<false, false, false, true>(
            xxxx(matrix.row0) * rx + yyyy(matrix.row0) * ry + zzzz(matrix.row0) * rz, matrix.row0
        ),
        blend<false, false, false, true>(
            xxxx(matrix.row1) * rx + yyyy(matrix.row1) * ry + zzzz(matrix.row1) * rz, matrix.row1
        ),
        blend<false, false, false, true>(
            xxxx(matrix.row2) * rx + yyyy(matrix.row2) * ry + zzzz(matrix.row2) * rz, matrix.row2
        ),
        blend<false, false, false, true>(
            xxxx(matrix.row3) * rx + yyyy(matrix.row3) * ry + zzzz(matrix.row3) * rz, matrix.row3
        )
    );
  }

  // Optimized version of glm:unproject: Doesn't take the projection and
  // view matrix separately but requires precomputed inverse of the
  // view-projection-matrix. Also assumes that in viewport.z is "one over
  // viewport-width" and in viewport.w is "one over viewport-height" stored.
  m_f32_4x4::RowVectorType unproject(
      const m_f32_4x4::RowVectorType &screenPosition, const m_f32_4x4 &inverseViewProjectionMatrix,
      const m_f32_4x4::RowVectorType &viewport
  ) noexcept {
    const auto affineScreenPoint = oneW(screenPosition);

    // normalize x and y of affineScreenPoint using the offset in viewport.xy and the scale in viewport.zw
    const auto normalizedScreenXY = (affineScreenPoint - viewport) * zwxy(viewport);
    const auto transformed = inverseViewProjectionMatrix * xy_zw(normalizedScreenXY, affineScreenPoint);
    assert(w(transformed) != Zero<m_f32_4x4::ValueType>() && "unproject with W=0 (point at infinity)");
    return transformed / wwww(transformed);
  }

  m_f32_4x4 ortho(
      const m_f32_4x4::ValueType left, const m_f32_4x4::ValueType right, const m_f32_4x4::ValueType top,
      const m_f32_4x4::ValueType bottom
  ) noexcept {
    typedef m_f32_4x4::ValueType T;
    typedef m_f32_4x4::RowVectorType RV;

    const auto rRL = reciprocal(right - left);
    const auto rTB = reciprocal(top - bottom);
    return m_f32_4x4(
        RV(rRL + rRL, Zero<T>(), Zero<T>(), Zero<T>()), RV(Zero<T>(), rTB + rTB, Zero<T>(), Zero<T>()),
        RV(Zero<T>(), Zero<T>(), NegativeOne<T>(), Zero<T>()),
        -RV((right + left) * rRL, (top + bottom) * rTB, NegativeZero<T>(), NegativeOne<T>())
    );
  }

  m_f32_4x4 perspectiveFov(
      const m_f32_4x4::ValueType fov, const m_f32_4x4::ValueType width, const m_f32_4x4::ValueType height,
      const m_f32_4x4::ValueType near, const m_f32_4x4::ValueType far
  ) noexcept {
    // Code adapted from glm::perspectiveFov.
    // see: http://schabby.de/projection-matrix/
    typedef m_f32_4x4::ValueType T;
    typedef m_f32_4x4::RowVectorType RV;

    const auto H = -reciprocal(tan(Half<T>() * DegreeToRadian<T>() * fov));
    const auto R = reciprocal(near - far);
    const auto NF = near * far;
    return m_f32_4x4(
        replaceX(Zero<RV>(), H * height / width), replaceY(Zero<RV>(), H),
        RV(Zero<T>(), Zero<T>(), far + near, NF + NF) * R, OneZ<RV>()
    );
  }
}
