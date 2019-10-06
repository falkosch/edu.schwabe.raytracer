#include "vectorization/v_f32_4/functions.h"

#include "vectorization/v_f32_4/accessors.h"
#include "vectorization/v_f32_4/blends.h"
#include "vectorization/v_f32_4/constants.h"
#include "vectorization/v_f32_4/operators.h"
#include "vectorization/v_f32_4/swizzles.h"
#include "vectorization/v_f32_4/swizzled_blends.h"

namespace vectorization
{
#pragma region min()
    //{ min()

    const v_f32_4 min(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return min(a.components, b.components);
    }

    const v_f32_4 minv(const v_f32_4 & v) noexcept {
        const v_f32_4 XZ_YW = min(v, zwzw(v));
        return min(XZ_YW, yyww(XZ_YW));
    }

    const v_f32_4::ValueType min(const v_f32_4 & v) noexcept {
        return x(minv(v));
    }

    const v_f32_4 min3v(const v_f32_4 & v) noexcept {
        return xxxx(min(min(v, yyww(v)), zwzw(v)));
    }

    const v_f32_4::ValueType min3(const v_f32_4 & v) noexcept {
        return x(min3v(v));
    }

    //}
#pragma endregion

#pragma region max()
    //{ max()

    const v_f32_4 max(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return max(a.components, b.components);
    }

    const v_f32_4 maxv(const v_f32_4 & v) noexcept {
        const v_f32_4 XZ_YW = max(v, zwzw(v));
        return max(XZ_YW, yyww(XZ_YW));
    }

    const v_f32_4::ValueType max(const v_f32_4 & v) noexcept {
        return x(maxv(v));
    }

    const v_f32_4 max3v(const v_f32_4 & v) noexcept {
        return xxxx(max(max(v, yyww(v)), zwzw(v)));
    }

    const v_f32_4::ValueType max3(const v_f32_4 & v) noexcept {
        return x(max3v(v));
    }

    //}
#pragma endregion

#pragma region horizontalSum()
    //{ horizontalSum()

    const v_f32_4 horizontalSumv(const v_f32_4 & v) noexcept {
        const v_f32_4::PackedType t = _mm_hadd_ps(v.components, v.components);
        return _mm_hadd_ps(t, t);
    }

    const v_f32_4::ValueType horizontalSum(const v_f32_4 & v) noexcept {
        return x(horizontalSumv(v));
    }

    const v_f32_4 horizontalSum3v(const v_f32_4 & v) noexcept {
        return horizontalSumv(zeroW(v));
    }

    const v_f32_4::ValueType horizontalSum3(const v_f32_4 & v) noexcept {
        return x(horizontalSum3v(v));
    }

    //}
#pragma endregion

#pragma region dot()
    //{ dot()

    const v_f32_4 dotv(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_dp_ps(a.components, b.components, 0xFF);
    }

    const v_f32_4::ValueType dot(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return x(dotv(a, b));
    }

    const v_f32_4 dot3v(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_dp_ps(a.components, b.components, 0x7F);
    }

    const v_f32_4::ValueType dot3(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return x(dot3v(a, b));
    }

    //}
#pragma endregion

#pragma region length()
    //{ length()

    const v_f32_4 lengthv(const v_f32_4 & v) noexcept {
        return sqrt(dotv(v, v));
    }

    const v_f32_4::ValueType length(const v_f32_4 & v) noexcept {
        return x(lengthv(v));
    }

    const v_f32_4 rlengthv(const v_f32_4 & v) noexcept {
        return rsqrt(dotv(v, v));
    }

    const v_f32_4::ValueType rlength(const v_f32_4 & v) noexcept {
        return x(rlengthv(v));
    }

    const v_f32_4 length3v(const v_f32_4 & v) noexcept {
        return sqrt(dot3v(v, v));
    }

    const v_f32_4::ValueType length3(const v_f32_4 & v) noexcept {
        return x(length3v(v));
    }

    const v_f32_4 rlength3v(const v_f32_4 & v) noexcept {
        return rsqrt(dot3v(v, v));
    }

    const v_f32_4::ValueType rlength3(const v_f32_4 & v) noexcept {
        return x(rlength3v(v));
    }

    //}
#pragma endregion

#pragma region normalize()
    //{ normalize()

    const v_f32_4 normalize(const v_f32_4 & v) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
        return v * rlengthv(v);
#else
        return v / lengthv(v);
#endif
    }

    const v_f32_4 normalize3(const v_f32_4 & v) noexcept {
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

    const v_f32_4 distancev(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return lengthv(b - a);
    }

    const v_f32_4::ValueType distance(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return x(distancev(a, b));
    }

    const v_f32_4 distance3v(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return length3v(b - a);
    }

    const v_f32_4::ValueType distance3(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return x(distance3v(a, b));
    }

    //}
#pragma endregion

#pragma region reflect()
    //{ reflect()

    const v_f32_4 reflect(const v_f32_4 & incident, const v_f32_4 & normal) noexcept {
        return reflect(incident, normal, dotv(normal, incident));
    }

    const v_f32_4 reflect(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI) noexcept {
        const v_f32_4 t = normal * NdotI;
        return incident - (t + t);
    }

    //}
#pragma endregion

#pragma region refract()
    //{ refract()

    const v_f32_4 refractEta(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept {
        // By snells law:
        // sin²(phi_t) = (eta_i/eta_t)² * sin²(phi_i)
        // = (eta_i/eta_t)² * (1 - cos²(phi_i)) = (eta_i/eta_t)² * (1 - (-I.N)²)
        // = (eta_i/eta_t)² - ((eta_i/eta_t) * (I.N))²
        // = 1 - cos²(phi_t)
        const v_f32_4 etaNdotI = eta * NdotI;
        const v_f32_4 sinSqrPhiT = eta * eta - etaNdotI * etaNdotI;
        // cos²(phi_t) = 1 - sin²(phi_t)
        const v_f32_4 cosSqrPhiT = One<v_f32_4>() - sinSqrPhiT;
        // TIR if 0 < cosSqrPhiT
        if (isNegative(cosSqrPhiT)) return Zero<v_f32_4>();
        // (eta_i/eta_t) * I + ((eta_i/eta_t) * cos(phi_i) - sqrt(cos²(phi_t))) * N
        // = (eta_i/eta_t) * I + ((eta_i/eta_t) * (-I.N) - sqrt(cos²(phi_t))) * N
        // = (eta_i/eta_t) * I - ((eta_i/eta_t) * (I.N) + sqrt(cos²(phi_t))) * N
        return eta * incident - (etaNdotI + sqrt(cosSqrPhiT)) * normal;
    }

    const v_f32_4 refractEta(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept {
        return refractEta(incident, normal, dotv(normal, incident), eta);
    }

    const v_f32_4 refract(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept {
        return refract(incident, normal, dotv(normal, incident), eta);
    }

    const v_f32_4 refract(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept {
        if (isNegative(NdotI)) {
            return refractEta(incident, normal, NdotI, xxxx(eta));
        }
        return refractEta(incident, -normal, -NdotI, yyyy(eta));
    }

    const v_f32_4 refract(const v_f32_4::VectorBoolType & cullingOrientation, const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept {
        return refract(cullingOrientation, incident, normal, dotv(normal, incident), eta);
    }

    const v_f32_4 refract(const v_f32_4::VectorBoolType & cullingOrientation, const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept {
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

    const v_f32_4 mix(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & factor) noexcept {
        return mix(a.components, b.components, factor.components);
    }

    const v_f32_4 mix(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::ValueType factor) noexcept {
        return mix(a.components, b.components, _mm_set_ps1(factor));
    }

    //}
#pragma endregion

#pragma region Special vector operations
    //{ Special vector operations

    const v_f32_4 zeroW(const v_f32_4 & v) noexcept {
        return _mm_insert_ps(v.components, v.components, _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::W));
    }

    const v_f32_4 oneW(const v_f32_4 & v) noexcept {
        return _mm_insert_ps(v.components, One<v_f32_4::PackedType>(), _MM_MK_INSERTPS_NDX(VectorIndices::W, VectorIndices::W, VectorBits::None));
    }

    const v_f32_4::BoolType isNegative(const v_f32_4 & v) noexcept {
        return _mm_movemask_ps(v.components);
    }

    const v_f32_4::VectorBoolType isNaN(const v_f32_4 & v) noexcept {
        return _mm_castps_si128(isNaN(v.components));
    }

    const v_f32_4::VectorBoolType isNaN(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_castps_si128(isNaN(a.components, b.components));
    }

    const v_f32_4 subadd(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return _mm_addsub_ps(a.components, b.components);
    }

    const v_f32_4 sqrt(const v_f32_4 & v) noexcept {
        return sqrt(v.components);
    }

    const v_f32_4 reciprocal(const v_f32_4 & v) noexcept {
        return reciprocal(v.components);
    }

    const v_f32_4 rsqrt(const v_f32_4 & v) noexcept {
        return rsqrt(v.components);
    }

    const v_f32_4 ceil(const v_f32_4 & v) noexcept {
        return ceil(v.components);
    }

    const v_f32_4 floor(const v_f32_4 & v) noexcept {
        return floor(v.components);
    }

    const v_f32_4 round(const v_f32_4 & v) noexcept {
        return round(v.components);
    }

    const v_f32_4 fract(const v_f32_4 & v) noexcept {
        return fract(v.components);
    }

    const v_f32_4 modulo(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        return modulo(a.components, b.components);
    }

    const v_f32_4 abs(const v_f32_4 & v) noexcept {
        return abs(v.components);
    }

    const v_f32_4 cross3(const v_f32_4 & a, const v_f32_4 & b) noexcept {
        // http://fastcpp.blogspot.de/2011/04/vector-cross-product-using-sse-code.html
        return yzxw(a) * zxyw(b) - zxyw(a) * yzxw(b);
    }

    const v_f32_4 clamp(const v_f32_4 & v, const v_f32_4 & lower, const v_f32_4 & upper) noexcept {
        return clamp(v.components, lower.components, upper.components);
    }

    const v_f32_4 mapOrthogonal(const v_f32_4 & v, const v_f32_4 & sPlane, const v_f32_4 & tPlane) noexcept {
        return x_yzw(
            dotv(v, sPlane),
            zw_zw(dotv(v, tPlane), OneW<v_f32_4>())
        );
    }

    const v_f32_4 mapSpherical(const v_f32_4 & v) noexcept {
        return replaceX(
            replaceY(OneW<v_f32_4>(), acos(-y(v)) * ReciprocalPi<v_f32_4::ValueType>()),
            atan2(z(v), x(v)) * RadianToUniform<v_f32_4::ValueType>() + Half<v_f32_4::ValueType>()
        );
    }

    const ASizeT argmin3(const v_f32_4 & v) noexcept {
        typedef v_f32_4::VectorBoolType VBT;
        typedef v_f32_4::BoolType BT;
        const VBT mask = xyxy(v) < zzyy(v);
        const VBT argXY = blendMasked(
            VBT(static_cast<BT>(VectorIndices::Z)),
            VBT(static_cast<BT>(VectorIndices::X), static_cast<BT>(VectorIndices::Y)),
            xyxy(mask) /* x(v) < z(v), y(v) < z(v) */);
        return static_cast<ASizeT>(x(blendMasked(yyyy(argXY), argXY, zzzz(mask) /* x(v) < y(v) */)));
    }

    const ASizeT argmax3(const v_f32_4 & v) noexcept {
        typedef v_f32_4::VectorBoolType VBT;
        typedef v_f32_4::BoolType BT;
        const VBT mask = xyxy(v) > zzyy(v);
        const VBT argXY = blendMasked(
            VBT(static_cast<BT>(VectorIndices::Z)),
            VBT(static_cast<BT>(VectorIndices::X), static_cast<BT>(VectorIndices::Y)),
            xyxy(mask) /* x(v) > z(v), y(v) > z(v) */);
        return static_cast<ASizeT>(x(blendMasked(yyyy(argXY), argXY, zzzz(mask) /* x(v) > y(v) */)));
    }

    const v_f32_4 axisAlignedNormal3(const v_f32_4 & normal) noexcept {
        const ASizeT maxAxis = argmax3(abs(normal));
        const v_f32_4 normalSigns = copysign(normal.components);
        v_f32_4 alignedNormal = Zero<v_f32_4>();
        alignedNormal[maxAxis] = normalSigns[maxAxis];
        return alignedNormal;
    }

    //}
#pragma endregion
    }