#include "vectorization/v_ui32_4.h"

#include "vectorization/functions/highest_lane.h"

namespace vectorization
{
    // One bit per lane, set where the lane is a set comparison mask
    inline Int_32 laneBits(const v_ui32_4& mask) noexcept
    {
        return _mm_movemask_ps(_mm_castsi128_ps(mask.components));
    }

    v_ui32_4 zeroW(const v_ui32_4& v) noexcept
    {
        // Shift by four bytes left and then back right to erase the w-component
        return _mm_srli_si128(_mm_slli_si128(v.components, sizeof(v_ui32_4::ValueType)),
                              sizeof(v_ui32_4::ValueType));
    }

    v_ui32_4 oneW(const v_ui32_4& v) noexcept
    {
        return blend<false, false, false, true>(v, One<v_ui32_4>());
    }

    v_ui32_4 sqr(const v_ui32_4& v) noexcept
    {
        return v * v;
    }

    v_ui32_4 lengthv(const v_ui32_4& v) noexcept
    {
        return xxxx(v) + yyyy(v) + zzzz(v) + wwww(v);
    }

    v_ui32_4 length3v(const v_ui32_4& v) noexcept
    {
        return xxxx(v) + yyyy(v) + zzzz(v);
    }

    v_ui32_4::ValueType length(const v_ui32_4& v) noexcept
    {
        return x(lengthv(v));
    }

    v_ui32_4::ValueType length3(const v_ui32_4& v) noexcept
    {
        return x(length3v(v));
    }

    // The larger minus the smaller component, so that the difference doesn't wrap around
    v_ui32_4 distancev(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return lengthv(max(a, b) - min(a, b));
    }

    v_ui32_4 distance3v(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return length3v(max(a, b) - min(a, b));
    }

    v_ui32_4::ValueType distance(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return x(distancev(a, b));
    }

    v_ui32_4::ValueType distance3(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return x(distance3v(a, b));
    }

    v_ui32_4 min(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return _mm_min_epu32(a.components, b.components);
    }

    v_ui32_4 minv(const v_ui32_4& v) noexcept
    {
        const v_ui32_4 XZ_YW = min(v, zwzw(v));
        return xxxx(min(XZ_YW, yxwz(XZ_YW)));
    }

    v_ui32_4::ValueType min(const v_ui32_4& v) noexcept
    {
        // gets the min of this (x, y, z, w) and shuffled (z, w, z, w) and then
        // gets the min of XZ_YW (x', y', z', w') and its shuffled (y', x', w', z')
        const v_ui32_4 XZ_YW = min(v, zwzw(v));
        return x(min(XZ_YW, yxwz(XZ_YW)));
    }

    v_ui32_4 min3v(const v_ui32_4& v) noexcept
    {
        return xxxx(min(yxwz(v), min(v, zwzw(v))));
    }

    v_ui32_4::ValueType min3(const v_ui32_4& v) noexcept
    {
        // gets the min of (x, y, z, w), (z, w, z, w) and then of (y, x, w, z)
        return x(min(yxwz(v), min(v, zwzw(v))));
    }

    v_ui32_4 max(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return _mm_max_epu32(a.components, b.components);
    }

    v_ui32_4 maxv(const v_ui32_4& v) noexcept
    {
        const v_ui32_4 XZ_YW = max(v, zwzw(v));
        return xxxx(max(XZ_YW, yxwz(XZ_YW)));
    }

    v_ui32_4::ValueType max(const v_ui32_4& v) noexcept
    {
        const v_ui32_4 XZ_YW = max(v, zwzw(v));
        return x(max(XZ_YW, yxwz(XZ_YW)));
    }

    v_ui32_4 max3v(const v_ui32_4& v) noexcept
    {
        return xxxx(max(yxwz(v), max(v, zwzw(v))));
    }

    v_ui32_4::ValueType max3(const v_ui32_4& v) noexcept
    {
        return x(max(yxwz(v), max(v, zwzw(v))));
    }

    v_ui32_4 horizontalSumv(const v_ui32_4& v) noexcept
    {
        return xxxx(v) + yyyy(v) + zzzz(v) + wwww(v);
    }

    v_ui32_4::ValueType horizontalSum(const v_ui32_4& v) noexcept
    {
        return x(horizontalSumv(v));
    }

    v_ui32_4 horizontalSum3v(const v_ui32_4& v) noexcept
    {
        return xxxx(v) + yyyy(v) + zzzz(v);
    }

    v_ui32_4::ValueType horizontalSum3(const v_ui32_4& v) noexcept
    {
        return x(horizontalSum3v(v));
    }

    v_ui32_4 dotv(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return horizontalSumv(a * b);
    }

    v_ui32_4::ValueType dot(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return x(dotv(a, b));
    }

    v_ui32_4 dot3v(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return horizontalSum3v(a * b);
    }

    v_ui32_4::ValueType dot3(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return x(dot3v(a, b));
    }

    v_ui32_4 multiplyAdd(const v_ui32_4& a, const v_ui32_4& b, const v_ui32_4& c) noexcept
    {
        return a * b + c;
    }

    v_ui32_4 multiplySub(const v_ui32_4& a, const v_ui32_4& b, const v_ui32_4& c) noexcept
    {
        return a * b - c;
    }

    v_ui32_4 multiplyAddSub(const v_ui32_4& a, const v_ui32_4& b, const v_ui32_4& c) noexcept
    {
        return addSubtract(a * b, c);
    }

    v_ui32_4 multiplySubAdd(const v_ui32_4& a, const v_ui32_4& b, const v_ui32_4& c) noexcept
    {
        const v_ui32_4 product = a * b;
        return blend<false, true, false, true>(product + c, product - c);
    }

    v_ui32_4 negativeMultiplyAdd(const v_ui32_4& a, const v_ui32_4& b, const v_ui32_4& c) noexcept
    {
        return c - a * b;
    }

    v_ui32_4 negativeMultiplySub(const v_ui32_4& a, const v_ui32_4& b, const v_ui32_4& c) noexcept
    {
        return Zero<v_ui32_4>() - a * b - c;
    }

    v_ui32_4 addSubtract(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return blend<false, true, false, true>(a - b, a + b);
    }

    v_ui32_4 modulo(const v_ui32_4& a, const v_ui32_4& b) noexcept
    {
        return a % b;
    }

    v_ui32_4 clamp(const v_ui32_4& v, const v_ui32_4& lower, const v_ui32_4& upper) noexcept
    {
        return max(lower, min(v, upper));
    }

    ASizeT argmin(const v_ui32_4& v) noexcept
    {
        return highestLane(laneBits(v == minv(v)));
    }

    ASizeT argmax(const v_ui32_4& v) noexcept
    {
        return highestLane(laneBits(v == maxv(v)));
    }

    ASizeT argmin3(const v_ui32_4& v) noexcept
    {
        return highestLane(laneBits(zeroW(v == min3v(v))));
    }

    ASizeT argmax3(const v_ui32_4& v) noexcept
    {
        return highestLane(laneBits(zeroW(v == max3v(v))));
    }

    bool anyTrue(const v_ui32_4& v) noexcept
    {
        return !_mm_testz_si128(v.components, MaskAll<v_ui32_4::PackedType>());
    }

    bool anyTrue3(const v_ui32_4& v) noexcept
    {
        return !_mm_testz_si128(v.components, MaskXYZ<v_ui32_4>().components);
    }

    bool anyFalse(const v_ui32_4& v) noexcept
    {
        return !_mm_testc_si128(v.components, MaskAll<v_ui32_4::PackedType>());
    }

    bool anyFalse3(const v_ui32_4& v) noexcept
    {
        return !_mm_testc_si128(v.components, MaskXYZ<v_ui32_4>().components);
    }

    bool allTrue(const v_ui32_4& v) noexcept
    {
        return !!_mm_testc_si128(v.components, MaskAll<v_ui32_4::PackedType>());
    }

    bool allTrue3(const v_ui32_4& v) noexcept
    {
        return !!_mm_testc_si128(v.components, MaskXYZ<v_ui32_4>().components);
    }

    bool allFalse(const v_ui32_4& v) noexcept
    {
        return !!_mm_testz_si128(v.components, MaskAll<v_ui32_4::PackedType>());
    }

    bool allFalse3(const v_ui32_4& v) noexcept
    {
        return !!_mm_testz_si128(v.components, MaskXYZ<v_ui32_4>().components);
    }
}
