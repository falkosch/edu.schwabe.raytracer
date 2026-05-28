#include "vectorization/v_f64_2/functions.h"

#include "vectorization/v_f64_2/accessors.h"
#include "vectorization/v_f64_2/blends.h"
#include "vectorization/v_f64_2/constants.h"
#include "vectorization/v_f64_2/operators.h"
#include "vectorization/v_f64_2/swizzles.h"

#include <cassert>

#include "vectorization/functions/abs.h"
#include "vectorization/functions/add_subtract.h"
#include "vectorization/functions/broadcast.h"
#include "vectorization/functions/ceil.h"
#include "vectorization/functions/clamp.h"
#include "vectorization/functions/floor.h"
#include "vectorization/functions/fract.h"
#include "vectorization/functions/is_nan.h"
#include "vectorization/functions/max.h"
#include "vectorization/functions/min.h"
#include "vectorization/functions/mix.h"
#include "vectorization/functions/movemask.h"
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

namespace vectorization
{
    v_f64_2 min(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return min(a.components, b.components);
    }

    v_f64_2 minv(const v_f64_2& v) noexcept
    {
        return min(v, yx(v));
    }

    v_f64_2::ValueType min(const v_f64_2& v) noexcept
    {
        return x(minv(v));
    }

    v_f64_2 max(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return max(a.components, b.components);
    }

    v_f64_2 maxv(const v_f64_2& v) noexcept
    {
        return max(v, yx(v));
    }

    v_f64_2::ValueType max(const v_f64_2& v) noexcept
    {
        return x(maxv(v));
    }

    v_f64_2 horizontalSumv(const v_f64_2& v) noexcept
    {
        return v + yx(v);
    }

    v_f64_2::ValueType horizontalSum(const v_f64_2& v) noexcept
    {
        return x(horizontalSumv(v));
    }

    v_f64_2 dotv(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return horizontalSumv(a * b);
    }

    v_f64_2::ValueType dot(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return x(dotv(a, b));
    }

    v_f64_2 lengthv(const v_f64_2& v) noexcept
    {
        return sqrt(dotv(v, v));
    }

    v_f64_2::ValueType length(const v_f64_2& v) noexcept
    {
        return x(lengthv(v));
    }

    v_f64_2 rlengthv(const v_f64_2& v) noexcept
    {
        return rsqrt(dotv(v, v));
    }

    v_f64_2::ValueType rlength(const v_f64_2& v) noexcept
    {
        return x(rlengthv(v));
    }

    v_f64_2 normalize(const v_f64_2& v) noexcept
    {
        assert(dot(v, v) > Zero<v_f64_2::ValueType>() && "normalize of zero-length vector");
#ifdef VECTORIZATION_APPROXIMATIONS
        return v * rlengthv(v);
#else
        return v / lengthv(v);
#endif
    }

    v_f64_2 distancev(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return lengthv(a - b);
    }

    v_f64_2::ValueType distance(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return x(distancev(a, b));
    }

    v_f64_2 mix(const v_f64_2& a, const v_f64_2& b, const v_f64_2& factor) noexcept
    {
        return mix(a.components, b.components, factor.components);
    }

    v_f64_2 mix(const v_f64_2& a, const v_f64_2& b, const v_f64_2::ValueType factor) noexcept
    {
        return mix(a.components, b.components, broadcast(factor));
    }

    v_f64_2 multiplyAdd(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept
    {
        return vectorization::multiplyAdd(a.components, b.components, c.components);
    }

    v_f64_2 multiplySub(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept
    {
        return vectorization::multiplySub(a.components, b.components, c.components);
    }

    v_f64_2 multiplyAddSub(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept
    {
        return vectorization::multiplyAddSub(a.components, b.components, c.components);
    }

    v_f64_2 multiplySubAdd(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept
    {
        return vectorization::multiplySubAdd(a.components, b.components, c.components);
    }

    v_f64_2 negativeMultiplyAdd(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept
    {
        return vectorization::negativeMultiplyAdd(a.components, b.components, c.components);
    }

    v_f64_2 negativeMultiplySub(const v_f64_2& a, const v_f64_2& b, const v_f64_2& c) noexcept
    {
        return vectorization::negativeMultiplySub(a.components, b.components, c.components);
    }

    v_f64_2::BoolType isNegative(const v_f64_2& v) noexcept
    {
        return static_cast<v_f64_2::BoolType>(movemask(v.components));
    }

    v_f64_2::VectorBoolType isNaN(const v_f64_2& v) noexcept
    {
        return _mm_castpd_si128(isNaN(v.components));
    }

    v_f64_2::VectorBoolType isNaN(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return _mm_castpd_si128(isNaN(a.components, b.components));
    }

    v_f64_2 addSubtract(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return vectorization::addSubtract(a.components, b.components);
    }

    v_f64_2 sqrt(const v_f64_2& v) noexcept
    {
        return sqrt(v.components);
    }

    v_f64_2 sqr(const v_f64_2& v) noexcept
    {
        return sqr(v.components);
    }

    v_f64_2 reciprocal(const v_f64_2& v) noexcept
    {
        return reciprocal(v.components);
    }

    v_f64_2 rsqrt(const v_f64_2& v) noexcept
    {
        return rsqrt(v.components);
    }

    v_f64_2 ceil(const v_f64_2& v) noexcept
    {
        return ceil(v.components);
    }

    v_f64_2 floor(const v_f64_2& v) noexcept
    {
        return floor(v.components);
    }

    v_f64_2 round(const v_f64_2& v) noexcept
    {
        return round(v.components);
    }

    v_f64_2 fract(const v_f64_2& v) noexcept
    {
        return fract(v.components);
    }

    v_f64_2 modulo(const v_f64_2& a, const v_f64_2& b) noexcept
    {
        return modulo(a.components, b.components);
    }

    v_f64_2 abs(const v_f64_2& v) noexcept
    {
        return abs(v.components);
    }

    v_f64_2 clamp(const v_f64_2& v, const v_f64_2& lower, const v_f64_2& upper) noexcept
    {
        return max(lower, min(v, upper));
    }

    bool anyTrue(const v_f64_2& v) noexcept
    {
        return movemask(v.components) != 0;
    }

    bool anyFalse(const v_f64_2& v) noexcept
    {
        return !allTrue(v);
    }

    bool allTrue(const v_f64_2& v) noexcept
    {
        return movemask(v.components) == 0x3;
    }

    bool allFalse(const v_f64_2& v) noexcept
    {
        return movemask(v.components) == 0;
    }
}
