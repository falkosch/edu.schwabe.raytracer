#pragma once

#include "type.h"

namespace vectorization
{
    v_f64_2 min(const v_f64_2& a, const v_f64_2& b) noexcept;
    v_f64_2 minv(const v_f64_2& v) noexcept;
    v_f64_2::ValueType min(const v_f64_2& v) noexcept;

    v_f64_2 max(const v_f64_2& a, const v_f64_2& b) noexcept;
    v_f64_2 maxv(const v_f64_2& v) noexcept;
    v_f64_2::ValueType max(const v_f64_2& v) noexcept;

    v_f64_2 horizontalSumv(const v_f64_2& v) noexcept;
    v_f64_2::ValueType horizontalSum(const v_f64_2& v) noexcept;

    v_f64_2 dotv(const v_f64_2& a, const v_f64_2& b) noexcept;
    v_f64_2::ValueType dot(const v_f64_2& a, const v_f64_2& b) noexcept;

    v_f64_2 lengthv(const v_f64_2& v) noexcept;
    v_f64_2::ValueType length(const v_f64_2& v) noexcept;
    v_f64_2 rlengthv(const v_f64_2& v) noexcept;
    v_f64_2::ValueType rlength(const v_f64_2& v) noexcept;

    v_f64_2 normalize(const v_f64_2& v) noexcept;

    v_f64_2 distancev(const v_f64_2& a, const v_f64_2& b) noexcept;
    v_f64_2::ValueType distance(const v_f64_2& a, const v_f64_2& b) noexcept;

    v_f64_2 reflect(const v_f64_2& incident, const v_f64_2& normal) noexcept;
    v_f64_2 reflect(const v_f64_2& incident, const v_f64_2& normal, const v_f64_2& NdotI) noexcept;

    // refractEta takes the ratio of refraction indices in both components of eta. refract takes the ratio for
    // entering (NdotI < 0) in x and the one for leaving in y, like v_f32_4.
    v_f64_2 refractEta(const v_f64_2& incident, const v_f64_2& normal, const v_f64_2& eta) noexcept;
    v_f64_2 refractEta(const v_f64_2& incident, const v_f64_2& normal, const v_f64_2& NdotI,
                       const v_f64_2& eta) noexcept;
    v_f64_2 refract(const v_f64_2& incident, const v_f64_2& normal, const v_f64_2& eta) noexcept;
    v_f64_2 refract(const v_f64_2& incident, const v_f64_2& normal, const v_f64_2& NdotI, const v_f64_2& eta) noexcept;
    v_f64_2 refract(
        const v_f64_2::VectorBoolType& cullingOrientation, const v_f64_2& incident, const v_f64_2& normal,
        const v_f64_2& eta
    ) noexcept;
    v_f64_2 refract(
        const v_f64_2::VectorBoolType& cullingOrientation, const v_f64_2& incident, const v_f64_2& normal,
        const v_f64_2& NdotI, const v_f64_2& eta
    ) noexcept;

    v_f64_2 mix(const v_f64_2& a, const v_f64_2& b, const v_f64_2& factor) noexcept;
    v_f64_2 mix(const v_f64_2& a, const v_f64_2& b, v_f64_2::ValueType factor) noexcept;

    v_f64_2 multiplyAdd(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept;
    v_f64_2 multiplySub(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept;
    v_f64_2 multiplyAddSub(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept;
    v_f64_2 multiplySubAdd(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept;
    v_f64_2 negativeMultiplyAdd(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept;
    v_f64_2 negativeMultiplySub(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept;

    // Returns a bitmask of VectorBits indicating components with negative values.
    v_f64_2::BoolType isNegative(const v_f64_2& v) noexcept;
    v_f64_2::VectorBoolType isNaN(const v_f64_2& v) noexcept;
    v_f64_2::VectorBoolType isNaN(const v_f64_2& a, const v_f64_2& b) noexcept;

    // Alternately subtracts and adds: even-indexed components are subtracted, odd-indexed are added.
    // x(out) = x(a) - x(b);
    // y(out) = y(a) + y(b);
    v_f64_2 addSubtract(const v_f64_2& a, const v_f64_2& b) noexcept;

    v_f64_2 sqr(const v_f64_2& v) noexcept;
    v_f64_2 sqrt(const v_f64_2& v) noexcept;
    v_f64_2 reciprocal(const v_f64_2& v) noexcept;
    v_f64_2 rsqrt(const v_f64_2& v) noexcept;

    v_f64_2 ceil(const v_f64_2& v) noexcept;
    v_f64_2 floor(const v_f64_2& v) noexcept;
    v_f64_2 round(const v_f64_2& v) noexcept;
    v_f64_2 fract(const v_f64_2& v) noexcept;

    v_f64_2 modulo(const v_f64_2& a, const v_f64_2& b) noexcept;
    v_f64_2 abs(const v_f64_2& v) noexcept;
    v_f64_2 clamp(const v_f64_2& v, const v_f64_2& lower, const v_f64_2& upper) noexcept;

    // Index of the smallest/largest component; of several equal ones, the highest index. With NaN components the
    // index is unspecified, but a valid one.
    ASizeT argmin(const v_f64_2& v) noexcept;
    ASizeT argmax(const v_f64_2& v) noexcept;

    // A component is true if its sign bit is set, like isNegative tests it and comparison masks set it.
    bool anyTrue(const v_f64_2& v) noexcept;
    bool anyFalse(const v_f64_2& v) noexcept;
    bool allTrue(const v_f64_2& v) noexcept;
    bool allFalse(const v_f64_2& v) noexcept;
}
