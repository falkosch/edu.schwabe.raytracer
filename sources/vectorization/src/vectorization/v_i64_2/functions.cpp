#include "vectorization/v_i64_2.h"

#include "vectorization/functions/highest_lane.h"

namespace vectorization
{
    v_i64_2::BoolType isNegative(const v_i64_2& v) noexcept
    {
        return _mm_movemask_pd(_mm_castsi128_pd(v.components));
    }

    v_i64_2 abs(const v_i64_2& v) noexcept
    {
        // _mm_abs_epi64 needs AVX-512
        return blendMasked(v, -v, v < Zero<v_i64_2>());
    }

    v_i64_2 sqr(const v_i64_2& v) noexcept
    {
        return v * v;
    }

    v_i64_2 min(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return blendMasked(a.components, b.components, (a > b).components);
    }

    v_i64_2 minv(const v_i64_2& v) noexcept
    {
        return min(v, yx(v));
    }

    v_i64_2::ValueType min(const v_i64_2& v) noexcept
    {
        return x(minv(v));
    }

    v_i64_2 max(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return blendMasked(b.components, a.components, (a > b).components);
    }

    v_i64_2 maxv(const v_i64_2& v) noexcept
    {
        return max(v, yx(v));
    }

    v_i64_2::ValueType max(const v_i64_2& v) noexcept
    {
        return x(maxv(v));
    }

    v_i64_2 horizontalSumv(const v_i64_2& v) noexcept
    {
        return xx(v) + yy(v);
    }

    v_i64_2::ValueType horizontalSum(const v_i64_2& v) noexcept
    {
        return x(horizontalSumv(v));
    }

    v_i64_2 dotv(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return horizontalSumv(a * b);
    }

    v_i64_2::ValueType dot(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return x(dotv(a, b));
    }

    v_i64_2 lengthv(const v_i64_2& v) noexcept
    {
        return horizontalSumv(abs(v));
    }

    v_i64_2::ValueType length(const v_i64_2& v) noexcept
    {
        return x(lengthv(v));
    }

    v_i64_2 distancev(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return lengthv(a - b);
    }

    v_i64_2::ValueType distance(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return x(distancev(a, b));
    }

    v_i64_2 multiplyAdd(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept
    {
        return a * b + c;
    }

    v_i64_2 multiplySub(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept
    {
        return a * b - c;
    }

    v_i64_2 multiplyAddSub(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept
    {
        return addSubtract(a * b, c);
    }

    v_i64_2 multiplySubAdd(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept
    {
        const v_i64_2 product = a * b;
        return blend<false, true>(product + c, product - c);
    }

    v_i64_2 negativeMultiplyAdd(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept
    {
        return c - a * b;
    }

    v_i64_2 negativeMultiplySub(const v_i64_2& a, const v_i64_2& b, const v_i64_2& c) noexcept
    {
        return -(a * b) - c;
    }

    v_i64_2 addSubtract(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return blend<false, true>(a - b, a + b);
    }

    v_i64_2 modulo(const v_i64_2& a, const v_i64_2& b) noexcept
    {
        return a % b;
    }

    v_i64_2 clamp(const v_i64_2& v, const v_i64_2& lower, const v_i64_2& upper) noexcept
    {
        return max(lower, min(v, upper));
    }

    ASizeT argmin(const v_i64_2& v) noexcept
    {
        return highestLane(static_cast<Int_32>(isNegative(v == minv(v))));
    }

    ASizeT argmax(const v_i64_2& v) noexcept
    {
        return highestLane(static_cast<Int_32>(isNegative(v == maxv(v))));
    }

    bool anyTrue(const v_i64_2& v) noexcept
    {
        return !_mm_testz_si128(v.components, MaskAll<v_i64_2::PackedType>());
    }

    bool anyFalse(const v_i64_2& v) noexcept
    {
        return !_mm_testc_si128(v.components, MaskAll<v_i64_2::PackedType>());
    }

    bool allTrue(const v_i64_2& v) noexcept
    {
        return !!_mm_testc_si128(v.components, MaskAll<v_i64_2::PackedType>());
    }

    bool allFalse(const v_i64_2& v) noexcept
    {
        return !!_mm_testz_si128(v.components, MaskAll<v_i64_2::PackedType>());
    }
}
