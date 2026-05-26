#pragma once

#include "type.h"

namespace vectorization
{
    v_f32_4 min(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4 minv(const v_f32_4& v) noexcept;
    v_f32_4::ValueType min(const v_f32_4& v) noexcept;
    v_f32_4 min3v(const v_f32_4& v) noexcept;
    v_f32_4::ValueType min3(const v_f32_4& v) noexcept;


    v_f32_4 max(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4 maxv(const v_f32_4& v) noexcept;
    v_f32_4::ValueType max(const v_f32_4& v) noexcept;
    v_f32_4 max3v(const v_f32_4& v) noexcept;
    v_f32_4::ValueType max3(const v_f32_4& v) noexcept;


    v_f32_4 horizontalSumv(const v_f32_4& v) noexcept;
    v_f32_4::ValueType horizontalSum(const v_f32_4& v) noexcept;
    v_f32_4 horizontalSum3v(const v_f32_4& v) noexcept;
    v_f32_4::ValueType horizontalSum3(const v_f32_4& v) noexcept;


    v_f32_4 dotv(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4::ValueType dot(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4 dot3v(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4::ValueType dot3(const v_f32_4& a, const v_f32_4& b) noexcept;


    v_f32_4 lengthv(const v_f32_4& v) noexcept;
    v_f32_4::ValueType length(const v_f32_4& v) noexcept;
    v_f32_4 rlengthv(const v_f32_4& v) noexcept;
    v_f32_4::ValueType rlength(const v_f32_4& v) noexcept;
    v_f32_4 length3v(const v_f32_4& v) noexcept;
    v_f32_4::ValueType length3(const v_f32_4& v) noexcept;
    v_f32_4 rlength3v(const v_f32_4& v) noexcept;
    v_f32_4::ValueType rlength3(const v_f32_4& v) noexcept;


    v_f32_4 normalize(const v_f32_4& v) noexcept;
    v_f32_4 normalize3(const v_f32_4& v) noexcept;


    v_f32_4 distancev(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4::ValueType distance(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4 distance3v(const v_f32_4& a, const v_f32_4& b) noexcept;
    v_f32_4::ValueType distance3(const v_f32_4& a, const v_f32_4& b) noexcept;


    v_f32_4 reflect(const v_f32_4& incident, const v_f32_4& normal) noexcept;
    v_f32_4 reflect(const v_f32_4& incident, const v_f32_4& normal, const v_f32_4& NdotI) noexcept;


    v_f32_4 refractEta(const v_f32_4& incident, const v_f32_4& normal, const v_f32_4& eta) noexcept;
    v_f32_4 refractEta(const v_f32_4& incident, const v_f32_4& normal, const v_f32_4& NdotI,
                       const v_f32_4& eta) noexcept;
    v_f32_4 refract(const v_f32_4& incident, const v_f32_4& normal, const v_f32_4& eta) noexcept;
    v_f32_4 refract(const v_f32_4& incident, const v_f32_4& normal, const v_f32_4& NdotI, const v_f32_4& eta) noexcept;
    v_f32_4 refract(
        const v_f32_4::VectorBoolType& cullingOrientation, const v_f32_4& incident, const v_f32_4& normal,
        const v_f32_4& eta
    ) noexcept;
    v_f32_4 refract(
        const v_f32_4::VectorBoolType& cullingOrientation, const v_f32_4& incident, const v_f32_4& normal,
        const v_f32_4& NdotI, const v_f32_4& eta
    ) noexcept;


    v_f32_4 mix(const v_f32_4& a, const v_f32_4& b, const v_f32_4& factor) noexcept;
    v_f32_4 mix(const v_f32_4& a, const v_f32_4& b, v_f32_4::ValueType factor) noexcept;


    v_f32_4 multiplyAdd(const v_f32_4& a, const v_f32_4& b, const v_f32_4& c) noexcept;
    v_f32_4 multiplySub(const v_f32_4& a, const v_f32_4& b, const v_f32_4& c) noexcept;
    v_f32_4 multiplyAddSub(const v_f32_4& a, const v_f32_4& b, const v_f32_4& c) noexcept;
    v_f32_4 multiplySubAdd(const v_f32_4& a, const v_f32_4& b, const v_f32_4& c) noexcept;
    v_f32_4 negativeMultiplyAdd(const v_f32_4& a, const v_f32_4& b, const v_f32_4& c) noexcept;
    v_f32_4 negativeMultiplySub(const v_f32_4& a, const v_f32_4& b, const v_f32_4& c) noexcept;


    v_f32_4 zeroW(const v_f32_4& v) noexcept;
    v_f32_4 oneW(const v_f32_4& v) noexcept;

    // Returns a bitmask of VectorBits indicating components with negative values. A set bit indicates a value
    // less than "0.0".
    v_f32_4::BoolType isNegative(const v_f32_4& v) noexcept;
    v_f32_4::VectorBoolType isNaN(const v_f32_4& v) noexcept;
    // Checks whether a has Nan or b has NaN values
    v_f32_4::VectorBoolType isNaN(const v_f32_4& a, const v_f32_4& b) noexcept;

    // Alternately subtracts and adds: even-indexed components are subtracted, odd-indexed are added.
    // x(out) = x(a) - x(b);
    // y(out) = y(a) + y(b);
    // z(out) = z(a) - z(b);
    // w(out) = w(a) + w(b);
    v_f32_4 addSubtract(const v_f32_4& a, const v_f32_4& b) noexcept;

    v_f32_4 sqrt(const v_f32_4& v) noexcept;

    v_f32_4 reciprocal(const v_f32_4& v) noexcept;

    v_f32_4 rsqrt(const v_f32_4& v) noexcept;

    v_f32_4 ceil(const v_f32_4& v) noexcept;

    v_f32_4 floor(const v_f32_4& v) noexcept;

    v_f32_4 round(const v_f32_4& v) noexcept;

    v_f32_4 fract(const v_f32_4& v) noexcept;

    v_f32_4 modulo(const v_f32_4& a, const v_f32_4& b) noexcept;

    v_f32_4 abs(const v_f32_4& v) noexcept;

    v_f32_4 cross3(const v_f32_4& a, const v_f32_4& b) noexcept;

    v_f32_4 clamp(const v_f32_4& v, const v_f32_4& lower, const v_f32_4& upper) noexcept;

    v_f32_4 mapOrthogonal(const v_f32_4& v, const v_f32_4& sPlane, const v_f32_4& tPlane) noexcept;

    v_f32_4 mapSpherical(const v_f32_4& v) noexcept;

    ASizeT argmin3(const v_f32_4& v) noexcept;
    ASizeT argmax3(const v_f32_4& v) noexcept;

    v_f32_4 axisAlignedNormal3(const v_f32_4& normal) noexcept;
}
