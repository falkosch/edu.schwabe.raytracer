#include "vectorization/v_i32_8/functions.h"

#include "vectorization/v_i32_8/accessors.h"
#include "vectorization/v_i32_8/blends.h"
#include "vectorization/v_i32_8/constants.h"
#include "vectorization/v_i32_8/operators.h"
#include "vectorization/v_i32_8/swizzles.h"

#include "vectorization/constants/masks/mask_all.h"
#include "vectorization/functions/highest_lane.h"
#include "vectorization/functions/modulo.h"

namespace vectorization
{
    // Swaps the 128-bit halves: {x5, x6, x7, x8, x1, x2, x3, x4}
    inline v_i32_8 swapHalves(const v_i32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm256_permute2f128_si256(v.components, v.components, 0x01);
#else
        return {{v.components.hi, v.components.lo}};
#endif
    }

    v_i32_8 andnot(const v_i32_8& a, const v_i32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_andnot_si256(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto lo = _mm_andnot_si128(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
        const auto hi = _mm_andnot_si128(_mm256_extractf128_si256(a.components, 1),
                                         _mm256_extractf128_si256(b.components, 1));
        return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
        return {
            {
                _mm_andnot_si128(a.components.lo, b.components.lo),
                _mm_andnot_si128(a.components.hi, b.components.hi)
            }
        };
#endif
    }

    v_i32_8::BoolType isNegative(const v_i32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm256_movemask_ps(_mm256_castsi256_ps(v.components));
#else
        return _mm_movemask_ps(_mm_castsi128_ps(v.components.lo))
            | (_mm_movemask_ps(_mm_castsi128_ps(v.components.hi)) << 4);
#endif
    }

    v_i32_8 abs(const v_i32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_abs_epi32(v.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto lo = _mm_abs_epi32(_mm256_castsi256_si128(v.components));
        const auto hi = _mm_abs_epi32(_mm256_extractf128_si256(v.components, 1));
        return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
        return {{_mm_abs_epi32(v.components.lo), _mm_abs_epi32(v.components.hi)}};
#endif
    }

    v_i32_8 sqr(const v_i32_8& v) noexcept
    {
        return v * v;
    }

    v_i32_8 min(const v_i32_8& a, const v_i32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_min_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto lo = _mm_min_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
        const auto hi = _mm_min_epi32(_mm256_extractf128_si256(a.components, 1),
                                      _mm256_extractf128_si256(b.components, 1));
        return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
        return {{_mm_min_epi32(a.components.lo, b.components.lo), _mm_min_epi32(a.components.hi, b.components.hi)}};
#endif
    }

    v_i32_8 max(const v_i32_8& a, const v_i32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        return _mm256_max_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto lo = _mm_max_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
        const auto hi = _mm_max_epi32(_mm256_extractf128_si256(a.components, 1),
                                      _mm256_extractf128_si256(b.components, 1));
        return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
        return {{_mm_max_epi32(a.components.lo, b.components.lo), _mm_max_epi32(a.components.hi, b.components.hi)}};
#endif
    }

    v_i32_8 minv(const v_i32_8& v) noexcept
    {
        // the minimum of each half in all of its lanes, then the minimum of both halves
        const v_i32_8 XZ_YW = min(v, zwzw(v));
        const v_i32_8 halves = xxxx(min(XZ_YW, yyww(XZ_YW)));
        return min(halves, swapHalves(halves));
    }

    v_i32_8::ValueType min(const v_i32_8& v) noexcept
    {
        return x1(minv(v));
    }

    v_i32_8 maxv(const v_i32_8& v) noexcept
    {
        const v_i32_8 XZ_YW = max(v, zwzw(v));
        const v_i32_8 halves = xxxx(max(XZ_YW, yyww(XZ_YW)));
        return max(halves, swapHalves(halves));
    }

    v_i32_8::ValueType max(const v_i32_8& v) noexcept
    {
        return x1(maxv(v));
    }

    v_i32_8 horizontalSumv(const v_i32_8& v) noexcept
    {
        const v_i32_8 XZ_YW = v + zwzw(v);
        const v_i32_8 halves = xxxx(XZ_YW + yyww(XZ_YW));
        return halves + swapHalves(halves);
    }

    v_i32_8::ValueType horizontalSum(const v_i32_8& v) noexcept
    {
        return x1(horizontalSumv(v));
    }

    v_i32_8 dotv(const v_i32_8& a, const v_i32_8& b) noexcept
    {
        return horizontalSumv(a * b);
    }

    v_i32_8::ValueType dot(const v_i32_8& a, const v_i32_8& b) noexcept
    {
        return x1(dotv(a, b));
    }

    v_i32_8 lengthv(const v_i32_8& v) noexcept
    {
        return horizontalSumv(abs(v));
    }

    v_i32_8::ValueType length(const v_i32_8& v) noexcept
    {
        return x1(lengthv(v));
    }

    v_i32_8 distancev(const v_i32_8& a, const v_i32_8& b) noexcept
    {
        return lengthv(a - b);
    }

    v_i32_8::ValueType distance(const v_i32_8& a, const v_i32_8& b) noexcept
    {
        return x1(distancev(a, b));
    }

    v_i32_8 multiplyAdd(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept
    {
        return a * b + c;
    }

    v_i32_8 multiplySub(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept
    {
        return a * b - c;
    }

    v_i32_8 multiplyAddSub(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept
    {
        return addSubtract(a * b, c);
    }

    v_i32_8 multiplySubAdd(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept
    {
        const v_i32_8 product = a * b;
        return blend<false, true, false, true, false, true, false, true>(product + c, product - c);
    }

    v_i32_8 negativeMultiplyAdd(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept
    {
        return c - a * b;
    }

    v_i32_8 negativeMultiplySub(const v_i32_8& a, const v_i32_8& b, const v_i32_8& c) noexcept
    {
        return -(a * b) - c;
    }

    v_i32_8 addSubtract(const v_i32_8& a, const v_i32_8& b) noexcept
    {
        return blend<false, true, false, true, false, true, false, true>(a - b, a + b);
    }

    v_i32_8 modulo(const v_i32_8& a, const v_i32_8& b) noexcept
    {
        // No SIMD integer division
        return v_i32_8(
            modulo(x1(a), x1(b)), modulo(x2(a), x2(b)), modulo(x3(a), x3(b)), modulo(x4(a), x4(b)),
            modulo(x5(a), x5(b)), modulo(x6(a), x6(b)), modulo(x7(a), x7(b)), modulo(x8(a), x8(b))
        );
    }

    v_i32_8 clamp(const v_i32_8& v, const v_i32_8& lower, const v_i32_8& upper) noexcept
    {
        return max(lower, min(v, upper));
    }

    ASizeT argmin(const v_i32_8& v) noexcept
    {
        return highestLane(isNegative(v == minv(v)));
    }

    ASizeT argmax(const v_i32_8& v) noexcept
    {
        return highestLane(isNegative(v == maxv(v)));
    }

    bool anyTrue(const v_i32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return !_mm256_testz_si256(v.components, v.components);
#else
        return !(_mm_testz_si128(v.components.lo, v.components.lo)
            && _mm_testz_si128(v.components.hi, v.components.hi));
#endif
    }

    bool anyFalse(const v_i32_8& v) noexcept
    {
        return !allTrue(v);
    }

    bool allTrue(const v_i32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm256_testc_si256(v.components, MaskAll<v_i32_8::PackedType>());
#else
        const auto ones = MaskAll<v_i32_8::HalfPackedType>();
        return _mm_testc_si128(v.components.lo, ones)
            && _mm_testc_si128(v.components.hi, ones);
#endif
    }

    bool allFalse(const v_i32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm256_testz_si256(v.components, v.components);
#else
        return _mm_testz_si128(v.components.lo, v.components.lo)
            && _mm_testz_si128(v.components.hi, v.components.hi);
#endif
    }
}
