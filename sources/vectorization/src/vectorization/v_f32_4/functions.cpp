#include "vectorization/v_f32_4/functions.h"

#include "vectorization/v_f32_4/accessors.h"

#include <cassert>
#include "vectorization/functions/broadcast.h"
#include "vectorization/v_f32_4/blends.h"
#include "vectorization/v_f32_4/constants.h"
#include "vectorization/v_f32_4/operators.h"
#include "vectorization/v_f32_4/selects.h"
#include "vectorization/v_f32_4/swizzled_blends.h"
#include "vectorization/v_f32_4/swizzles.h"

#include "vectorization/v_i32_4/accessors.h"
#include "vectorization/v_i32_4/blends.h"
#include "vectorization/v_i32_4/swizzles.h"

#include "vectorization/functions/abs.h"
#include "vectorization/functions/acos.h"
#include "vectorization/functions/add_subtract.h"
#include "vectorization/functions/atan2.h"
#include "vectorization/functions/ceil.h"
#include "vectorization/functions/clamp.h"
#include "vectorization/functions/copysign.h"
#include "vectorization/functions/floor.h"
#include "vectorization/functions/fract.h"
#include "vectorization/functions/is_nan.h"
#include "vectorization/functions/max.h"
#include "vectorization/functions/min.h"
#include "vectorization/functions/mix.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply_add.h"
#include "vectorization/functions/multiply_add_sub.h"
#include "vectorization/functions/multiply_sub.h"
#include "vectorization/functions/multiply_sub_add.h"
#include "vectorization/functions/negative_multiply_add.h"
#include "vectorization/functions/negative_multiply_sub.h"
#include "vectorization/functions/reciprocal.h"
#include "vectorization/functions/round.h"
#include "vectorization/functions/rsqrt.h"
#include "vectorization/functions/sqrt.h"

namespace vectorization {
#pragma region min()
  //{ min()

  v_f32_4 min(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return min(a.components, b.components);
  }

  v_f32_4 minv(const v_f32_4 &v) noexcept {
    const v_f32_4 XZ_YW = min(v, zwzw(v));
    return min(XZ_YW, yyww(XZ_YW));
  }

  v_f32_4::ValueType min(const v_f32_4 &v) noexcept {
    return x(minv(v));
  }

  v_f32_4 min3v(const v_f32_4 &v) noexcept {
    return xxxx(min(min(v, yyww(v)), zwzw(v)));
  }

  v_f32_4::ValueType min3(const v_f32_4 &v) noexcept {
    return x(min3v(v));
  }

  //}
#pragma endregion

#pragma region max()
  //{ max()

  v_f32_4 max(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return max(a.components, b.components);
  }

  v_f32_4 maxv(const v_f32_4 &v) noexcept {
    const v_f32_4 XZ_YW = max(v, zwzw(v));
    return max(XZ_YW, yyww(XZ_YW));
  }

  v_f32_4::ValueType max(const v_f32_4 &v) noexcept {
    return x(maxv(v));
  }

  v_f32_4 max3v(const v_f32_4 &v) noexcept {
    return xxxx(max(max(v, yyww(v)), zwzw(v)));
  }

  v_f32_4::ValueType max3(const v_f32_4 &v) noexcept {
    return x(max3v(v));
  }

  //}
#pragma endregion

#pragma region horizontalSum()
  //{ horizontalSum()

  v_f32_4 horizontalSumv(const v_f32_4 &v) noexcept {
    const v_f32_4 t = v + zwzw(v);
    return t + yyww(t);
  }

  v_f32_4::ValueType horizontalSum(const v_f32_4 &v) noexcept {
    return x(horizontalSumv(v));
  }

  v_f32_4 horizontalSum3v(const v_f32_4 &v) noexcept {
    return horizontalSumv(zeroW(v));
  }

  v_f32_4::ValueType horizontalSum3(const v_f32_4 &v) noexcept {
    return x(horizontalSum3v(v));
  }

  //}
#pragma endregion

#pragma region dot()
  //{ dot()

  v_f32_4 dotv(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_dp_ps(a.components, b.components, 0xFF);
  }

  v_f32_4::ValueType dot(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return x(dotv(a, b));
  }

  v_f32_4 dot3v(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_dp_ps(a.components, b.components, 0x7F);
  }

  v_f32_4::ValueType dot3(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return x(dot3v(a, b));
  }

  //}
#pragma endregion

#pragma region length()
  //{ length()

  v_f32_4 lengthv(const v_f32_4 &v) noexcept {
    return sqrt(dotv(v, v));
  }

  v_f32_4::ValueType length(const v_f32_4 &v) noexcept {
    return x(lengthv(v));
  }

  v_f32_4 rlengthv(const v_f32_4 &v) noexcept {
    return rsqrt(dotv(v, v));
  }

  v_f32_4::ValueType rlength(const v_f32_4 &v) noexcept {
    return x(rlengthv(v));
  }

  v_f32_4 length3v(const v_f32_4 &v) noexcept {
    return sqrt(dot3v(v, v));
  }

  v_f32_4::ValueType length3(const v_f32_4 &v) noexcept {
    return x(length3v(v));
  }

  v_f32_4 rlength3v(const v_f32_4 &v) noexcept {
    return rsqrt(dot3v(v, v));
  }

  v_f32_4::ValueType rlength3(const v_f32_4 &v) noexcept {
    return x(rlength3v(v));
  }

  //}
#pragma endregion

#pragma region normalize()
  //{ normalize()

  v_f32_4 normalize(const v_f32_4 &v) noexcept {
    assert(dot(v, v) > Zero<v_f32_4::ValueType>() && "normalize of zero-length vector");
#ifdef VECTORIZATION_APPROXIMATIONS
    return v * rlengthv(v);
#else
    return v / lengthv(v);
#endif
  }

  v_f32_4 normalize3(const v_f32_4 &v) noexcept {
    assert(dot3(v, v) > Zero<v_f32_4::ValueType>() && "normalize3 of zero-length vector");
#ifdef VECTORIZATION_APPROXIMATIONS
    return zeroW(v * rlength3v(v));
#else
    return zeroW(v / length3v(v));
#endif
  }

  //}
#pragma endregion

#pragma region distance()
  //{ distance()

  v_f32_4 distancev(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return lengthv(b - a);
  }

  v_f32_4::ValueType distance(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return x(distancev(a, b));
  }

  v_f32_4 distance3v(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return length3v(b - a);
  }

  v_f32_4::ValueType distance3(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return x(distance3v(a, b));
  }

  //}
#pragma endregion

#pragma region reflect()
  //{ reflect()

  v_f32_4 reflect(const v_f32_4 &incident, const v_f32_4 &normal) noexcept {
    return reflect(incident, normal, dotv(normal, incident));
  }

  v_f32_4 reflect(const v_f32_4 &incident, const v_f32_4 &normal, const v_f32_4 &NdotI) noexcept {
    return negativeMultiplyAdd(normal, NdotI + NdotI, incident);
  }

  //}
#pragma endregion

#pragma region refract()
  //{ refract()

  v_f32_4
  refractEta(const v_f32_4 &incident, const v_f32_4 &normal, const v_f32_4 &NdotI, const v_f32_4 &eta) noexcept {
    // By Snell's law: https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
    const v_f32_4 etaNdotI = eta * NdotI;
    const v_f32_4 sinSqrPhiT = multiplySub(eta, eta, etaNdotI * etaNdotI);
    const v_f32_4 cosSqrPhiT = One<v_f32_4>() - sinSqrPhiT;

    if (isNegative(cosSqrPhiT)) {
      // Total internal reflection
      return Zero<v_f32_4>();
    }

    return multiplySub(eta, incident, (etaNdotI + sqrt(cosSqrPhiT)) * normal);
  }

  v_f32_4 refractEta(const v_f32_4 &incident, const v_f32_4 &normal, const v_f32_4 &eta) noexcept {
    return refractEta(incident, normal, dotv(normal, incident), eta);
  }

  v_f32_4 refract(const v_f32_4 &incident, const v_f32_4 &normal, const v_f32_4 &eta) noexcept {
    return refract(incident, normal, dotv(normal, incident), eta);
  }

  v_f32_4 refract(const v_f32_4 &incident, const v_f32_4 &normal, const v_f32_4 &NdotI, const v_f32_4 &eta) noexcept {
    if (isNegative(NdotI)) {
      return refractEta(incident, normal, NdotI, xxxx(eta));
    }
    return refractEta(incident, -normal, -NdotI, yyyy(eta));
  }

  v_f32_4 refract(
      const v_f32_4::VectorBoolType &cullingOrientation, const v_f32_4 &incident, const v_f32_4 &normal,
      const v_f32_4 &eta
  ) noexcept {
    return refract(cullingOrientation, incident, normal, dotv(normal, incident), eta);
  }

  v_f32_4 refract(
      const v_f32_4::VectorBoolType &cullingOrientation, const v_f32_4 &incident, const v_f32_4 &normal,
      const v_f32_4 &NdotI, const v_f32_4 &eta
  ) noexcept {
    if (x(cullingOrientation)) // => NdotI < 0
    {
      return refractEta(incident, normal, NdotI, xxxx(eta));
    }
    if (y(cullingOrientation)) // => NdotI > 0
    {
      return refractEta(incident, -normal, -NdotI, yyyy(eta));
    }
    // cullingOrientation is zero => "no culling" set => no implicit ray-orientation set
    return refract(incident, normal, NdotI, eta);
  }

  //}
#pragma endregion

#pragma region mix()
  //{ mix()

  v_f32_4 mix(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &factor) noexcept {
    return mix(a.components, b.components, factor.components);
  }

  v_f32_4 mix(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4::ValueType factor) noexcept {
    return mix(a.components, b.components, broadcast(factor));
  }

  //}
#pragma endregion

#pragma region multiplyAdd() / multiplySub() / negativeMultiplyAdd() / negativeMultiplySub()
  //{ multiplyAdd() / multiplySub() / negativeMultiplyAdd() / negativeMultiplySub()

  v_f32_4 multiplyAdd(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept {
    return vectorization::multiplyAdd(a.components, b.components, c.components);
  }

  v_f32_4 multiplySub(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept {
    return vectorization::multiplySub(a.components, b.components, c.components);
  }

  v_f32_4 multiplyAddSub(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept {
    return vectorization::multiplyAddSub(a.components, b.components, c.components);
  }

  v_f32_4 multiplySubAdd(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept {
    return vectorization::multiplySubAdd(a.components, b.components, c.components);
  }

  v_f32_4 negativeMultiplyAdd(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept {
    return vectorization::negativeMultiplyAdd(a.components, b.components, c.components);
  }

  v_f32_4 negativeMultiplySub(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &c) noexcept {
    return vectorization::negativeMultiplySub(a.components, b.components, c.components);
  }

  //}
#pragma endregion

#pragma region Special vector operations
  //{ Special vector operations

  v_f32_4 zeroW(const v_f32_4 &v) noexcept {
    return _mm_insert_ps(
        v.components, v.components, _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::W)
    );
  }

  v_f32_4 oneW(const v_f32_4 &v) noexcept {
    return _mm_insert_ps(
        v.components, One<v_f32_4::PackedType>(),
        _MM_MK_INSERTPS_NDX(VectorIndices::W, VectorIndices::W, VectorBits::None)
    );
  }

  v_f32_4::BoolType isNegative(const v_f32_4 &v) noexcept {
    return _mm_movemask_ps(v.components);
  }

  v_f32_4::VectorBoolType isNaN(const v_f32_4 &v) noexcept {
    return _mm_castps_si128(isNaN(v.components));
  }

  v_f32_4::VectorBoolType isNaN(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(isNaN(a.components, b.components));
  }

  v_f32_4 addSubtract(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return vectorization::addSubtract(a.components, b.components);
  }

  v_f32_4 sqrt(const v_f32_4 &v) noexcept {
    return sqrt(v.components);
  }

  v_f32_4 reciprocal(const v_f32_4 &v) noexcept {
    return reciprocal(v.components);
  }

  v_f32_4 rsqrt(const v_f32_4 &v) noexcept {
    return rsqrt(v.components);
  }

  v_f32_4 ceil(const v_f32_4 &v) noexcept {
    return ceil(v.components);
  }

  v_f32_4 floor(const v_f32_4 &v) noexcept {
    return floor(v.components);
  }

  v_f32_4 round(const v_f32_4 &v) noexcept {
    return round(v.components);
  }

  v_f32_4 fract(const v_f32_4 &v) noexcept {
    return fract(v.components);
  }

  v_f32_4 modulo(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return modulo(a.components, b.components);
  }

  v_f32_4 abs(const v_f32_4 &v) noexcept {
    return abs(v.components);
  }

  v_f32_4 cross3(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    // http://fastcpp.blogspot.de/2011/04/vector-cross-product-using-sse-code.html
    return multiplySub(yzxw(a), zxyw(b), zxyw(a) * yzxw(b));
  }

  v_f32_4 clamp(const v_f32_4 &v, const v_f32_4 &lower, const v_f32_4 &upper) noexcept {
    return clamp(v.components, lower.components, upper.components);
  }

  v_f32_4 mapOrthogonal(const v_f32_4 &v, const v_f32_4 &sPlane, const v_f32_4 &tPlane) noexcept {
    return x_yzw(dotv(v, sPlane), zw_zw(dotv(v, tPlane), OneW<v_f32_4>()));
  }

  v_f32_4 mapSpherical(const v_f32_4 &v) noexcept {
    return replaceX(
        replaceY(OneW<v_f32_4>(), acos(-y(v)) * ReciprocalPi<v_f32_4::ValueType>()),
        atan2(z(v), x(v)) * RadianToUniform<v_f32_4::ValueType>() + Half<v_f32_4::ValueType>()
    );
  }

  ASizeT argmin3(const v_f32_4 &v) noexcept {
    using VBT = v_f32_4::VectorBoolType;
    using BT = v_f32_4::BoolType;
    const VBT mask = xyxy(v) < zzyy(v);
    const VBT argXY = blendMasked(
        VBT(static_cast<BT>(VectorIndices::Z)),
        VBT(static_cast<BT>(VectorIndices::X), static_cast<BT>(VectorIndices::Y)),
        xyxy(mask) /* x(v) < z(v), y(v) < z(v) */
    );
    return static_cast<ASizeT>(x(blendMasked(yyyy(argXY), argXY, zzzz(mask) /* x(v) < y(v) */)));
  }

  ASizeT argmax3(const v_f32_4 &v) noexcept {
    using VBT = v_f32_4::VectorBoolType;
    using BT = v_f32_4::BoolType;
    const VBT mask = xyxy(v) > zzyy(v);
    const VBT argXY = blendMasked(
        VBT(static_cast<BT>(VectorIndices::Z)),
        VBT(static_cast<BT>(VectorIndices::X), static_cast<BT>(VectorIndices::Y)),
        xyxy(mask) /* x(v) > z(v), y(v) > z(v) */
    );
    return static_cast<ASizeT>(x(blendMasked(yyyy(argXY), argXY, zzzz(mask) /* x(v) > y(v) */)));
  }

  v_f32_4 axisAlignedNormal3(const v_f32_4 &normal) noexcept {
    const v_f32_4 normalSigns = copySign(normal.components);
    const ASizeT maxAxis = argmax3(abs(normal));
    return replaceComponent(Zero<v_f32_4>(), normalSigns, maxAxis);
  }

  //}
#pragma endregion
}
