#pragma once

#include "type.h"

namespace vectorization
{
    v_i32_8 andnot(const v_i32_8& a, const v_i32_8& b) noexcept;

    // Returns a bitmask indicating components with negative values, one bit per component (VectorBits::X1 ... X8).
    v_i32_8::BoolType isNegative(const v_i32_8& v) noexcept;

    v_i32_8 abs(const v_i32_8& v) noexcept;

    v_i32_8 sqr(const v_i32_8& v) noexcept;

    v_i32_8 min(const v_i32_8& a, const v_i32_8& b) noexcept;
    v_i32_8 minv(const v_i32_8& v) noexcept;
    v_i32_8::ValueType min(const v_i32_8& v) noexcept;

    v_i32_8 max(const v_i32_8& a, const v_i32_8& b) noexcept;
    v_i32_8 maxv(const v_i32_8& v) noexcept;
    v_i32_8::ValueType max(const v_i32_8& v) noexcept;

    v_i32_8 horizontalSumv(const v_i32_8& v) noexcept;
    v_i32_8::ValueType horizontalSum(const v_i32_8& v) noexcept;

    v_i32_8 dotv(const v_i32_8& a, const v_i32_8& b) noexcept;
    v_i32_8::ValueType dot(const v_i32_8& a, const v_i32_8& b) noexcept;

    // Integer lengths and distances are Manhattan lengths: the sum of the absolute component values.
    v_i32_8 lengthv(const v_i32_8& v) noexcept;
    v_i32_8::ValueType length(const v_i32_8& v) noexcept;

    v_i32_8 distancev(const v_i32_8& a, const v_i32_8& b) noexcept;
    v_i32_8::ValueType distance(const v_i32_8& a, const v_i32_8& b) noexcept;

    // The multiply-add family wraps around on overflow like the integer operators.
    // a * b + c
    v_i32_8 multiplyAdd(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept;
    // a * b - c
    v_i32_8 multiplySub(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept;
    // a * b with alternating sub/add of c: even-indexed components subtract, odd-indexed add
    v_i32_8 multiplyAddSub(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept;
    // a * b with alternating add/sub of c: even-indexed components add, odd-indexed subtract
    v_i32_8 multiplySubAdd(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept;
    // -(a * b) + c
    v_i32_8 negativeMultiplyAdd(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept;
    // -(a * b) - c
    v_i32_8 negativeMultiplySub(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept;

    // Alternately subtracts and adds: even-indexed components are subtracted, odd-indexed are added.
    v_i32_8 addSubtract(const v_i32_8& a, const v_i32_8& b) noexcept;

    // Remainder of the truncating division per component, like operator%: the result has the sign of a.
    v_i32_8 modulo(const v_i32_8& a, const v_i32_8& b) noexcept;

    v_i32_8 clamp(const v_i32_8& v, const v_i32_8& lower, const v_i32_8& upper) noexcept;

    // Index of the smallest/largest component; of several equal ones, the highest index.
    ASizeT argmin(const v_i32_8& v) noexcept;
    ASizeT argmax(const v_i32_8& v) noexcept;

    // The tests are bitwise: allTrue needs all bits of the tested components set, anyTrue any bit, allFalse none and
    // anyFalse not all. Comparison masks set all or none of a component's bits.
    bool anyTrue(const v_i32_8& v) noexcept;
    bool anyFalse(const v_i32_8& v) noexcept;
    bool allTrue(const v_i32_8& v) noexcept;
    bool allFalse(const v_i32_8& v) noexcept;
}
