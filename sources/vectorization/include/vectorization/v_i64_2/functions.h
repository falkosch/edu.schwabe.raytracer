#pragma once

#include "type.h"

namespace vectorization
{
    // Returns a bitmask of VectorBits indicating components with negative values, one bit per component. A set bit
    // indicates a value less than "0".
    v_i64_2::BoolType isNegative(const v_i64_2& v) noexcept;

    v_i64_2 abs(const v_i64_2& v) noexcept;

    v_i64_2 sqr(const v_i64_2& v) noexcept;

    v_i64_2 min(const v_i64_2& a, const v_i64_2& b) noexcept;
    v_i64_2 minv(const v_i64_2& v) noexcept;
    v_i64_2::ValueType min(const v_i64_2& v) noexcept;

    v_i64_2 max(const v_i64_2& a, const v_i64_2& b) noexcept;
    v_i64_2 maxv(const v_i64_2& v) noexcept;
    v_i64_2::ValueType max(const v_i64_2& v) noexcept;

    v_i64_2 horizontalSumv(const v_i64_2& v) noexcept;
    v_i64_2::ValueType horizontalSum(const v_i64_2& v) noexcept;

    v_i64_2 dotv(const v_i64_2& a, const v_i64_2& b) noexcept;
    v_i64_2::ValueType dot(const v_i64_2& a, const v_i64_2& b) noexcept;

    // Integer lengths and distances are Manhattan lengths: the sum of the absolute component values.
    v_i64_2 lengthv(const v_i64_2& v) noexcept;
    v_i64_2::ValueType length(const v_i64_2& v) noexcept;

    v_i64_2 distancev(const v_i64_2& a, const v_i64_2& b) noexcept;
    v_i64_2::ValueType distance(const v_i64_2& a, const v_i64_2& b) noexcept;

    // The multiply-add family wraps around on overflow like the integer operators.
    // a * b + c
    v_i64_2 multiplyAdd(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept;
    // a * b - c
    v_i64_2 multiplySub(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept;
    // a * b with alternating sub/add of c: {a.x*b.x-c.x, a.y*b.y+c.y}
    v_i64_2 multiplyAddSub(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept;
    // a * b with alternating add/sub of c: {a.x*b.x+c.x, a.y*b.y-c.y}
    v_i64_2 multiplySubAdd(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept;
    // -(a * b) + c
    v_i64_2 negativeMultiplyAdd(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept;
    // -(a * b) - c
    v_i64_2 negativeMultiplySub(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept;

    // Alternately subtracts and adds: {a.x - b.x, a.y + b.y}
    v_i64_2 addSubtract(const v_i64_2& a, const v_i64_2& b) noexcept;

    // Remainder of the truncating division per component, like operator%: the result has the sign of a.
    v_i64_2 modulo(const v_i64_2& a, const v_i64_2& b) noexcept;

    v_i64_2 clamp(const v_i64_2& v, const v_i64_2& lower, const v_i64_2& upper) noexcept;

    // Index of the smallest/largest component; of several equal ones, the highest index.
    ASizeT argmin(const v_i64_2& v) noexcept;
    ASizeT argmax(const v_i64_2& v) noexcept;

    // The tests are bitwise: allTrue needs all bits of the tested components set, anyTrue any bit, allFalse none and
    // anyFalse not all. Comparison masks set all or none of a component's bits.
    bool anyTrue(const v_i64_2& v) noexcept;
    bool anyFalse(const v_i64_2& v) noexcept;
    bool allTrue(const v_i64_2& v) noexcept;
    bool allFalse(const v_i64_2& v) noexcept;
}
