#pragma once

#include "type.h"

namespace vectorization
{
    v_ui64_2 sqr(const v_ui64_2& v) noexcept;

    v_ui64_2 min(const v_ui64_2& a, const v_ui64_2& b) noexcept;
    v_ui64_2 minv(const v_ui64_2& v) noexcept;
    v_ui64_2::ValueType min(const v_ui64_2& v) noexcept;

    v_ui64_2 max(const v_ui64_2& a, const v_ui64_2& b) noexcept;
    v_ui64_2 maxv(const v_ui64_2& v) noexcept;
    v_ui64_2::ValueType max(const v_ui64_2& v) noexcept;

    v_ui64_2 horizontalSumv(const v_ui64_2& v) noexcept;
    v_ui64_2::ValueType horizontalSum(const v_ui64_2& v) noexcept;

    v_ui64_2 dotv(const v_ui64_2& a, const v_ui64_2& b) noexcept;
    v_ui64_2::ValueType dot(const v_ui64_2& a, const v_ui64_2& b) noexcept;

    // Integer lengths and distances are Manhattan lengths: the sum of the absolute component values.
    v_ui64_2 lengthv(const v_ui64_2& v) noexcept;
    v_ui64_2::ValueType length(const v_ui64_2& v) noexcept;

    v_ui64_2 distancev(const v_ui64_2& a, const v_ui64_2& b) noexcept;
    v_ui64_2::ValueType distance(const v_ui64_2& a, const v_ui64_2& b) noexcept;

    // The multiply-add family wraps around on overflow like the integer operators.
    // a * b + c
    v_ui64_2 multiplyAdd(const v_ui64_2& a, const v_ui64_2& b, const v_ui64_2& c) noexcept;
    // a * b - c
    v_ui64_2 multiplySub(const v_ui64_2& a, const v_ui64_2& b, const v_ui64_2& c) noexcept;
    // a * b with alternating sub/add of c: {a.x*b.x-c.x, a.y*b.y+c.y}
    v_ui64_2 multiplyAddSub(const v_ui64_2& a, const v_ui64_2& b, const v_ui64_2& c) noexcept;
    // a * b with alternating add/sub of c: {a.x*b.x+c.x, a.y*b.y-c.y}
    v_ui64_2 multiplySubAdd(const v_ui64_2& a, const v_ui64_2& b, const v_ui64_2& c) noexcept;
    // -(a * b) + c
    v_ui64_2 negativeMultiplyAdd(const v_ui64_2& a, const v_ui64_2& b, const v_ui64_2& c) noexcept;
    // -(a * b) - c
    v_ui64_2 negativeMultiplySub(const v_ui64_2& a, const v_ui64_2& b, const v_ui64_2& c) noexcept;

    // Alternately subtracts and adds: {a.x - b.x, a.y + b.y}
    v_ui64_2 addSubtract(const v_ui64_2& a, const v_ui64_2& b) noexcept;

    // Remainder of the division per component, like operator%.
    v_ui64_2 modulo(const v_ui64_2& a, const v_ui64_2& b) noexcept;

    v_ui64_2 clamp(const v_ui64_2& v, const v_ui64_2& lower, const v_ui64_2& upper) noexcept;

    // Index of the smallest/largest component; of several equal ones, the highest index.
    ASizeT argmin(const v_ui64_2& v) noexcept;
    ASizeT argmax(const v_ui64_2& v) noexcept;

    // The tests are bitwise: allTrue needs all bits of the tested components set, anyTrue any bit, allFalse none and
    // anyFalse not all. Comparison masks set all or none of a component's bits.
    bool anyTrue(const v_ui64_2& v) noexcept;
    bool anyFalse(const v_ui64_2& v) noexcept;
    bool allTrue(const v_ui64_2& v) noexcept;
    bool allFalse(const v_ui64_2& v) noexcept;
}
