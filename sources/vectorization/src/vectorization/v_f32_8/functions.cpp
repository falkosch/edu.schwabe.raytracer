#include "vectorization/v_f32_8/functions.h"

#include "vectorization/v_f32_8/accessors.h"
#include "vectorization/v_f32_8/blends.h"
#include "vectorization/v_f32_8/constants.h"
#include "vectorization/v_f32_8/operators.h"
#include "vectorization/v_f32_8/swizzles.h"

#include <cassert>

#include "vectorization/functions/abs.h"
#include "vectorization/functions/add_subtract.h"
#include "vectorization/functions/ceil.h"
#include "vectorization/functions/clamp.h"
#include "vectorization/functions/divide.h"
#include "vectorization/functions/floor.h"
#include "vectorization/functions/fract.h"
#include "vectorization/functions/is_nan.h"
#include "vectorization/functions/max.h"
#include "vectorization/functions/min.h"
#include "vectorization/functions/mix.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply_add.h"
#include "vectorization/functions/multiply_add_sub.h"
#include "vectorization/functions/multiply_sub.h"
#include "vectorization/functions/multiply_sub_add.h"
#include "vectorization/functions/negative_multiply_add.h"
#include "vectorization/functions/negative_multiply_sub.h"
#include "vectorization/functions/reciprocal.h"
#include "vectorization/functions/round.h"
#include "vectorization/functions/rsqrt.h"
#include "vectorization/functions/sqr.h"
#include "vectorization/functions/sqrt.h"

#include "vectorization/functions/movemask.h"

namespace vectorization
{
    v_f32_8 min(const v_f32_8& a, const v_f32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return min(a.components, b.components);
#else
        return {{min(a.components.lo, b.components.lo), min(a.components.hi, b.components.hi)}};
#endif
    }

    v_f32_8 minv(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto swapped = _mm256_permute2f128_ps(v.components, v.components, 0x01);
        const auto minHalves = _mm256_min_ps(v.components, swapped);
        const auto zw = _mm256_shuffle_ps(minHalves, minHalves, _MM_SHUFFLE(2, 3, 2, 3));
        const auto minZW = _mm256_min_ps(minHalves, zw);
        const auto y = _mm256_shuffle_ps(minZW, minZW, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm256_permute_ps(_mm256_min_ps(minZW, y), _MM_SHUFFLE(0, 0, 0, 0));
#else
        const auto minHalves = _mm_min_ps(v.components.lo, v.components.hi);
        const auto zw = _mm_shuffle_ps(minHalves, minHalves, _MM_SHUFFLE(2, 3, 2, 3));
        const auto minZW = _mm_min_ps(minHalves, zw);
        const auto y = _mm_shuffle_ps(minZW, minZW, _MM_SHUFFLE(1, 1, 1, 1));
        const auto scalar = _mm_min_ps(minZW, y);
        const auto result = _mm_shuffle_ps(scalar, scalar, 0);
        return {{result, result}};
#endif
    }

    v_f32_8::ValueType min(const v_f32_8& v) noexcept
    {
        return x1(minv(v));
    }

    v_f32_8 maxv(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto swapped = _mm256_permute2f128_ps(v.components, v.components, 0x01);
        const auto maxHalves = _mm256_max_ps(v.components, swapped);
        const auto zw = _mm256_shuffle_ps(maxHalves, maxHalves, _MM_SHUFFLE(2, 3, 2, 3));
        const auto maxZW = _mm256_max_ps(maxHalves, zw);
        const auto y = _mm256_shuffle_ps(maxZW, maxZW, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm256_permute_ps(_mm256_max_ps(maxZW, y), _MM_SHUFFLE(0, 0, 0, 0));
#else
        const auto maxHalves = _mm_max_ps(v.components.lo, v.components.hi);
        const auto zw = _mm_shuffle_ps(maxHalves, maxHalves, _MM_SHUFFLE(2, 3, 2, 3));
        const auto maxZW = _mm_max_ps(maxHalves, zw);
        const auto y = _mm_shuffle_ps(maxZW, maxZW, _MM_SHUFFLE(1, 1, 1, 1));
        const auto scalar = _mm_max_ps(maxZW, y);
        const auto result = _mm_shuffle_ps(scalar, scalar, 0);
        return {{result, result}};
#endif
    }

    v_f32_8 max(const v_f32_8& a, const v_f32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return max(a.components, b.components);
#else
        return {{max(a.components.lo, b.components.lo), max(a.components.hi, b.components.hi)}};
#endif
    }

    v_f32_8::ValueType max(const v_f32_8& v) noexcept
    {
        return x1(maxv(v));
    }

    v_f32_8 horizontalSumv(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        const auto swapped = _mm256_permute2f128_ps(v.components, v.components, 0x01);
        const auto sumHalves = _mm256_add_ps(v.components, swapped);
        const auto zw = _mm256_shuffle_ps(sumHalves, sumHalves, _MM_SHUFFLE(2, 3, 2, 3));
        const auto sumZW = _mm256_add_ps(sumHalves, zw);
        const auto y = _mm256_shuffle_ps(sumZW, sumZW, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm256_permute_ps(_mm256_add_ps(sumZW, y), _MM_SHUFFLE(0, 0, 0, 0));
#else
        const auto sumHalves = _mm_add_ps(v.components.lo, v.components.hi);
        const auto zw = _mm_shuffle_ps(sumHalves, sumHalves, _MM_SHUFFLE(2, 3, 2, 3));
        const auto sumZW = _mm_add_ps(sumHalves, zw);
        const auto y = _mm_shuffle_ps(sumZW, sumZW, _MM_SHUFFLE(1, 1, 1, 1));
        const auto scalar = _mm_add_ps(sumZW, y);
        const auto result = _mm_shuffle_ps(scalar, scalar, 0);
        return {{result, result}};
#endif
    }

    v_f32_8::ValueType horizontalSum(const v_f32_8& v) noexcept
    {
        return x1(horizontalSumv(v));
    }

    v_f32_8 dotv(const v_f32_8& a, const v_f32_8& b) noexcept
    {
        return horizontalSumv(a * b);
    }

    v_f32_8::ValueType dot(const v_f32_8& a, const v_f32_8& b) noexcept
    {
        return x1(dotv(a, b));
    }

    v_f32_8 lengthv(const v_f32_8& v) noexcept
    {
        return sqrt(dotv(v, v));
    }

    v_f32_8::ValueType length(const v_f32_8& v) noexcept
    {
        return x1(lengthv(v));
    }

    v_f32_8 rlengthv(const v_f32_8& v) noexcept
    {
        return rsqrt(dotv(v, v));
    }

    v_f32_8::ValueType rlength(const v_f32_8& v) noexcept
    {
        return x1(rlengthv(v));
    }

    v_f32_8 normalize(const v_f32_8& v) noexcept
    {
        assert(dot(v, v) > Zero<v_f32_8::ValueType>() && "normalize of zero-length vector");
#ifdef VECTORIZATION_APPROXIMATIONS
        return v * rlengthv(v);
#else
        return v / lengthv(v);
#endif
    }

    v_f32_8 distancev(const v_f32_8& a, const v_f32_8& b) noexcept
    {
        return lengthv(a - b);
    }

    v_f32_8::ValueType distance(const v_f32_8& a, const v_f32_8& b) noexcept
    {
        return x1(distancev(a, b));
    }

    v_f32_8 mix(const v_f32_8& a, const v_f32_8& b, const v_f32_8& factor) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return mix(a.components, b.components, factor.components);
#else
        return {
            {
                mix(a.components.lo, b.components.lo, factor.components.lo),
                mix(a.components.hi, b.components.hi, factor.components.hi)
            }
        };
#endif
    }

    v_f32_8 mix(const v_f32_8& a, const v_f32_8& b, const v_f32_8::ValueType factor) noexcept
    {
        return mix(a, b, v_f32_8(factor));
    }

    v_f32_8 multiplyAdd(const v_f32_8& a, const v_f32_8& b, const v_f32_8& c) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return multiplyAdd(a.components, b.components, c.components);
#else
        return {
            {
                multiplyAdd(a.components.lo, b.components.lo, c.components.lo),
                multiplyAdd(a.components.hi, b.components.hi, c.components.hi)
            }
        };
#endif
    }

    v_f32_8 multiplySub(const v_f32_8& a, const v_f32_8& b, const v_f32_8& c) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return multiplySub(a.components, b.components, c.components);
#else
        return {
            {
                multiplySub(a.components.lo, b.components.lo, c.components.lo),
                multiplySub(a.components.hi, b.components.hi, c.components.hi)
            }
        };
#endif
    }

    v_f32_8 multiplyAddSub(const v_f32_8& a, const v_f32_8& b, const v_f32_8& c) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return multiplyAddSub(a.components, b.components, c.components);
#else
        return {
            {
                multiplyAddSub(a.components.lo, b.components.lo, c.components.lo),
                multiplyAddSub(a.components.hi, b.components.hi, c.components.hi)
            }
        };
#endif
    }

    v_f32_8 multiplySubAdd(const v_f32_8& a, const v_f32_8& b, const v_f32_8& c) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return multiplySubAdd(a.components, b.components, c.components);
#else
        return {
            {
                multiplySubAdd(a.components.lo, b.components.lo, c.components.lo),
                multiplySubAdd(a.components.hi, b.components.hi, c.components.hi)
            }
        };
#endif
    }

    v_f32_8 negativeMultiplyAdd(const v_f32_8& a, const v_f32_8& b, const v_f32_8& c) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return negativeMultiplyAdd(a.components, b.components, c.components);
#else
        return {
            {
                negativeMultiplyAdd(a.components.lo, b.components.lo, c.components.lo),
                negativeMultiplyAdd(a.components.hi, b.components.hi, c.components.hi)
            }
        };
#endif
    }

    v_f32_8 negativeMultiplySub(const v_f32_8& a, const v_f32_8& b, const v_f32_8& c) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return negativeMultiplySub(a.components, b.components, c.components);
#else
        return {
            {
                negativeMultiplySub(a.components.lo, b.components.lo, c.components.lo),
                negativeMultiplySub(a.components.hi, b.components.hi, c.components.hi)
            }
        };
#endif
    }

    v_f32_8::BoolType isNegative(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return movemask(v.components);
#else
        return movemask(v.components.lo)
            | (movemask(v.components.hi) << 4);
#endif
    }

    v_f32_8::VectorBoolType isNaN(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm256_castps_si256(isNaN(v.components));
#else
        return {{_mm_castps_si128(isNaN(v.components.lo)), _mm_castps_si128(isNaN(v.components.hi))}};
#endif
    }

    v_f32_8::VectorBoolType isNaN(const v_f32_8& a, const v_f32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return _mm256_castps_si256(isNaN(a.components, b.components));
#else
        return {
            {
                _mm_castps_si128(isNaN(a.components.lo, b.components.lo)),
                _mm_castps_si128(isNaN(a.components.hi, b.components.hi))
            }
        };
#endif
    }

    v_f32_8 addSubtract(const v_f32_8& a, const v_f32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return addSubtract(a.components, b.components);
#else
        return {
            {
                addSubtract(a.components.lo, b.components.lo),
                addSubtract(a.components.hi, b.components.hi)
            }
        };
#endif
    }

    v_f32_8 sqrt(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return sqrt(v.components);
#else
        return {{sqrt(v.components.lo), sqrt(v.components.hi)}};
#endif
    }

    v_f32_8 sqr(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return sqr(v.components);
#else
        return {{sqr(v.components.lo), sqr(v.components.hi)}};
#endif
    }

    v_f32_8 reciprocal(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return reciprocal(v.components);
#else
        return {{reciprocal(v.components.lo), reciprocal(v.components.hi)}};
#endif
    }

    v_f32_8 rsqrt(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return rsqrt(v.components);
#else
        return {{rsqrt(v.components.lo), rsqrt(v.components.hi)}};
#endif
    }

    v_f32_8 ceil(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return ceil(v.components);
#else
        return {{ceil(v.components.lo), ceil(v.components.hi)}};
#endif
    }

    v_f32_8 floor(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return floor(v.components);
#else
        return {{floor(v.components.lo), floor(v.components.hi)}};
#endif
    }

    v_f32_8 round(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return round(v.components);
#else
        return {{round(v.components.lo), round(v.components.hi)}};
#endif
    }

    v_f32_8 fract(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return fract(v.components);
#else
        return {{fract(v.components.lo), fract(v.components.hi)}};
#endif
    }

    v_f32_8 modulo(const v_f32_8& a, const v_f32_8& b) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return modulo(a.components, b.components);
#else
        return {{modulo(a.components.lo, b.components.lo), modulo(a.components.hi, b.components.hi)}};
#endif
    }

    v_f32_8 abs(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return abs(v.components);
#else
        return {{abs(v.components.lo), abs(v.components.hi)}};
#endif
    }

    v_f32_8 clamp(const v_f32_8& v, const v_f32_8& lower, const v_f32_8& upper) noexcept
    {
        return max(lower, min(v, upper));
    }

    bool anyTrue(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return movemask(v.components) != 0;
#else
        return movemask(v.components.lo) != 0
            || movemask(v.components.hi) != 0;
#endif
    }

    bool anyFalse(const v_f32_8& v) noexcept
    {
        return !allTrue(v);
    }

    bool allTrue(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return movemask(v.components) == 0xFF;
#else
        return movemask(v.components.lo) == 0xF
            && movemask(v.components.hi) == 0xF;
#endif
    }

    bool allFalse(const v_f32_8& v) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        return movemask(v.components) == 0;
#else
        return movemask(v.components.lo) == 0
            && movemask(v.components.hi) == 0;
#endif
    }
}
