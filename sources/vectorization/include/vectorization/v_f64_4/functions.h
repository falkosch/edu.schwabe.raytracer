#pragma once

#include "type.h"

namespace vectorization
{
    v_f64_4 min(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 minv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType min(const v_f64_4& v) noexcept;
    v_f64_4 min3v(const v_f64_4& v) noexcept;
    v_f64_4::ValueType min3(const v_f64_4& v) noexcept;

    v_f64_4 max(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 maxv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType max(const v_f64_4& v) noexcept;
    v_f64_4 max3v(const v_f64_4& v) noexcept;
    v_f64_4::ValueType max3(const v_f64_4& v) noexcept;

    v_f64_4 horizontalSumv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType horizontalSum(const v_f64_4& v) noexcept;
    v_f64_4 horizontalSum3v(const v_f64_4& v) noexcept;
    v_f64_4::ValueType horizontalSum3(const v_f64_4& v) noexcept;

    v_f64_4 dotv(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4::ValueType dot(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 dot3v(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4::ValueType dot3(const v_f64_4& a, const v_f64_4& b) noexcept;

    v_f64_4 lengthv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType length(const v_f64_4& v) noexcept;
    v_f64_4 rlengthv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType rlength(const v_f64_4& v) noexcept;
    v_f64_4 length3v(const v_f64_4& v) noexcept;
    v_f64_4::ValueType length3(const v_f64_4& v) noexcept;
    v_f64_4 rlength3v(const v_f64_4& v) noexcept;
    v_f64_4::ValueType rlength3(const v_f64_4& v) noexcept;

    v_f64_4 normalize(const v_f64_4& v) noexcept;
    v_f64_4 normalize3(const v_f64_4& v) noexcept;

    v_f64_4 distancev(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4::ValueType distance(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 distance3v(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4::ValueType distance3(const v_f64_4& a, const v_f64_4& b) noexcept;

    v_f64_4 reflect(const v_f64_4& incident, const v_f64_4& normal) noexcept;
    v_f64_4 reflect(const v_f64_4& incident, const v_f64_4& normal, const v_f64_4& NdotI) noexcept;

    // refractEta takes the ratio of refraction indices in all components of eta. refract takes the ratio for
    // entering (NdotI < 0) in x and the one for leaving in y, like v_f32_4.
    v_f64_4 refractEta(const v_f64_4& incident, const v_f64_4& normal, const v_f64_4& eta) noexcept;
    v_f64_4 refractEta(const v_f64_4& incident, const v_f64_4& normal, const v_f64_4& NdotI,
                       const v_f64_4& eta) noexcept;
    v_f64_4 refract(const v_f64_4& incident, const v_f64_4& normal, const v_f64_4& eta) noexcept;
    v_f64_4 refract(const v_f64_4& incident, const v_f64_4& normal, const v_f64_4& NdotI, const v_f64_4& eta) noexcept;
    v_f64_4 refract(
        const v_f64_4::VectorBoolType& cullingOrientation, const v_f64_4& incident, const v_f64_4& normal,
        const v_f64_4& eta
    ) noexcept;
    v_f64_4 refract(
        const v_f64_4::VectorBoolType& cullingOrientation, const v_f64_4& incident, const v_f64_4& normal,
        const v_f64_4& NdotI, const v_f64_4& eta
    ) noexcept;

    v_f64_4 mix(const v_f64_4& a, const v_f64_4& b, const v_f64_4& factor) noexcept;
    v_f64_4 mix(const v_f64_4& a, const v_f64_4& b, v_f64_4::ValueType factor) noexcept;

    v_f64_4 multiplyAdd(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 multiplySub(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 multiplyAddSub(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 multiplySubAdd(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 negativeMultiplyAdd(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 negativeMultiplySub(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;

    v_f64_4 zeroW(const v_f64_4& v) noexcept;
    v_f64_4 oneW(const v_f64_4& v) noexcept;

    v_f64_4::BoolType isNegative(const v_f64_4& v) noexcept;
    v_f64_4::VectorBoolType isNaN(const v_f64_4& v) noexcept;
    v_f64_4::VectorBoolType isNaN(const v_f64_4& a, const v_f64_4& b) noexcept;

    v_f64_4 addSubtract(const v_f64_4& a, const v_f64_4& b) noexcept;

    v_f64_4 sqr(const v_f64_4& v) noexcept;
    v_f64_4 sqrt(const v_f64_4& v) noexcept;
    v_f64_4 reciprocal(const v_f64_4& v) noexcept;
    v_f64_4 rsqrt(const v_f64_4& v) noexcept;

    v_f64_4 ceil(const v_f64_4& v) noexcept;
    v_f64_4 floor(const v_f64_4& v) noexcept;
    v_f64_4 round(const v_f64_4& v) noexcept;
    v_f64_4 fract(const v_f64_4& v) noexcept;

    v_f64_4 modulo(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 abs(const v_f64_4& v) noexcept;
    v_f64_4 clamp(const v_f64_4& v, const v_f64_4& lower, const v_f64_4& upper) noexcept;

    v_f64_4 cross3(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 mapOrthogonal(const v_f64_4& v, const v_f64_4& sPlane, const v_f64_4& tPlane) noexcept;
    v_f64_4 mapSpherical(const v_f64_4& v) noexcept;

    // Index of the smallest/largest component; of several equal ones, the highest index. With NaN components the
    // index is unspecified, but a valid one.
    ASizeT argmin(const v_f64_4& v) noexcept;
    ASizeT argmax(const v_f64_4& v) noexcept;
    ASizeT argmin3(const v_f64_4& v) noexcept;
    ASizeT argmax3(const v_f64_4& v) noexcept;

    // A component is true if its sign bit is set, like isNegative tests it and comparison masks set it.
    bool anyTrue(const v_f64_4& v) noexcept;
    bool anyTrue3(const v_f64_4& v) noexcept;
    bool anyFalse(const v_f64_4& v) noexcept;
    bool anyFalse3(const v_f64_4& v) noexcept;
    bool allTrue(const v_f64_4& v) noexcept;
    bool allTrue3(const v_f64_4& v) noexcept;
    bool allFalse(const v_f64_4& v) noexcept;
    bool allFalse3(const v_f64_4& v) noexcept;
}
