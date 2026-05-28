#pragma once

#include "type.h"

namespace vectorization
{
    v_f64_4 min(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 minv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType min(const v_f64_4& v) noexcept;

    v_f64_4 max(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4 maxv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType max(const v_f64_4& v) noexcept;

    v_f64_4 horizontalSumv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType horizontalSum(const v_f64_4& v) noexcept;

    v_f64_4 dotv(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4::ValueType dot(const v_f64_4& a, const v_f64_4& b) noexcept;

    v_f64_4 lengthv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType length(const v_f64_4& v) noexcept;
    v_f64_4 rlengthv(const v_f64_4& v) noexcept;
    v_f64_4::ValueType rlength(const v_f64_4& v) noexcept;

    v_f64_4 normalize(const v_f64_4& v) noexcept;

    v_f64_4 distancev(const v_f64_4& a, const v_f64_4& b) noexcept;
    v_f64_4::ValueType distance(const v_f64_4& a, const v_f64_4& b) noexcept;

    v_f64_4 mix(const v_f64_4& a, const v_f64_4& b, const v_f64_4& factor) noexcept;
    v_f64_4 mix(const v_f64_4& a, const v_f64_4& b, v_f64_4::ValueType factor) noexcept;

    v_f64_4 multiplyAdd(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 multiplySub(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 multiplyAddSub(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 multiplySubAdd(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 negativeMultiplyAdd(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;
    v_f64_4 negativeMultiplySub(const v_f64_4& a, const v_f64_4& b, const v_f64_4& c) noexcept;

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

    bool anyTrue(const v_f64_4& v) noexcept;
    bool anyFalse(const v_f64_4& v) noexcept;
    bool allTrue(const v_f64_4& v) noexcept;
    bool allFalse(const v_f64_4& v) noexcept;
}
