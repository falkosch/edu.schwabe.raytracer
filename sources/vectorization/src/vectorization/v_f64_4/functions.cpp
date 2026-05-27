#include "vectorization/v_f64_4/functions.h"

#include "vectorization/v_f64_4/accessors.h"
#include "vectorization/v_f64_4/blends.h"
#include "vectorization/v_f64_4/constants.h"
#include "vectorization/v_f64_4/operators.h"
#include "vectorization/v_f64_4/swizzles.h"

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
#include "vectorization/functions/movemask.h"
#include "vectorization/functions/sqrt.h"

namespace vectorization {
  v_f64_4 min(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return min(a.components, b.components);
#else
    return {{min(a.components.lo, b.components.lo), min(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 minv(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto swapped = _mm256_permute2f128_pd(v.components, v.components, 0x01);
    const auto minHalves = _mm256_min_pd(v.components, swapped);
    const auto minFinal = _mm256_min_pd(minHalves, _mm256_permute_pd(minHalves, 0x05));
    return minFinal;
#else
    const auto minHalves = _mm_min_pd(v.components.lo, v.components.hi);
    const auto swapped = _mm_shuffle_pd(minHalves, minHalves, 1);
    const auto result = _mm_min_pd(minHalves, swapped);
    return {{result, result}};
#endif
  }

  v_f64_4::ValueType min(const v_f64_4 &v) noexcept {
    return x(minv(v));
  }

  v_f64_4 max(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return max(a.components, b.components);
#else
    return {{max(a.components.lo, b.components.lo), max(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 maxv(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto swapped = _mm256_permute2f128_pd(v.components, v.components, 0x01);
    const auto maxHalves = _mm256_max_pd(v.components, swapped);
    const auto maxFinal = _mm256_max_pd(maxHalves, _mm256_permute_pd(maxHalves, 0x05));
    return maxFinal;
#else
    const auto maxHalves = _mm_max_pd(v.components.lo, v.components.hi);
    const auto swapped = _mm_shuffle_pd(maxHalves, maxHalves, 1);
    const auto result = _mm_max_pd(maxHalves, swapped);
    return {{result, result}};
#endif
  }

  v_f64_4::ValueType max(const v_f64_4 &v) noexcept {
    return x(maxv(v));
  }

  v_f64_4 horizontalSumv(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto swapped = _mm256_permute2f128_pd(v.components, v.components, 0x01);
    const auto sumHalves = _mm256_add_pd(v.components, swapped);
    const auto sumFinal = _mm256_add_pd(sumHalves, _mm256_permute_pd(sumHalves, 0x05));
    return sumFinal;
#else
    const auto sumHalves = _mm_add_pd(v.components.lo, v.components.hi);
    const auto swapped = _mm_shuffle_pd(sumHalves, sumHalves, 1);
    const auto result = _mm_add_pd(sumHalves, swapped);
    return {{result, result}};
#endif
  }

  v_f64_4::ValueType horizontalSum(const v_f64_4 &v) noexcept {
    return x(horizontalSumv(v));
  }

  v_f64_4 dotv(const v_f64_4 &a, const v_f64_4 &b) noexcept {
    return horizontalSumv(a * b);
  }

  v_f64_4::ValueType dot(const v_f64_4 &a, const v_f64_4 &b) noexcept {
    return x(dotv(a, b));
  }

  v_f64_4 lengthv(const v_f64_4 &v) noexcept {
    return sqrt(dotv(v, v));
  }

  v_f64_4::ValueType length(const v_f64_4 &v) noexcept {
    return x(lengthv(v));
  }

  v_f64_4 rlengthv(const v_f64_4 &v) noexcept {
    return rsqrt(dotv(v, v));
  }

  v_f64_4::ValueType rlength(const v_f64_4 &v) noexcept {
    return x(rlengthv(v));
  }

  v_f64_4 normalize(const v_f64_4 &v) noexcept {
    assert(dot(v, v) > Zero<v_f64_4::ValueType>() && "normalize of zero-length vector");
#ifdef VECTORIZATION_APPROXIMATIONS
    return v * rlengthv(v);
#else
    return v / lengthv(v);
#endif
  }

  v_f64_4 distancev(const v_f64_4 &a, const v_f64_4 &b) noexcept {
    return lengthv(a - b);
  }

  v_f64_4::ValueType distance(const v_f64_4 &a, const v_f64_4 &b) noexcept {
    return x(distancev(a, b));
  }

  v_f64_4 mix(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &factor) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return mix(a.components, b.components, factor.components);
#else
    return {{mix(a.components.lo, b.components.lo, factor.components.lo),
             mix(a.components.hi, b.components.hi, factor.components.hi)}};
#endif
  }

  v_f64_4 mix(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4::ValueType factor) noexcept {
    return mix(a, b, v_f64_4(factor));
  }

  v_f64_4 multiplyAdd(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &c) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return multiplyAdd(a.components, b.components, c.components);
#else
    return {{multiplyAdd(a.components.lo, b.components.lo, c.components.lo),
             multiplyAdd(a.components.hi, b.components.hi, c.components.hi)}};
#endif
  }

  v_f64_4 multiplySub(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &c) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return multiplySub(a.components, b.components, c.components);
#else
    return {{multiplySub(a.components.lo, b.components.lo, c.components.lo),
             multiplySub(a.components.hi, b.components.hi, c.components.hi)}};
#endif
  }

  v_f64_4 multiplyAddSub(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &c) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return multiplyAddSub(a.components, b.components, c.components);
#else
    return {{multiplyAddSub(a.components.lo, b.components.lo, c.components.lo),
             multiplyAddSub(a.components.hi, b.components.hi, c.components.hi)}};
#endif
  }

  v_f64_4 multiplySubAdd(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &c) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return multiplySubAdd(a.components, b.components, c.components);
#else
    return {{multiplySubAdd(a.components.lo, b.components.lo, c.components.lo),
             multiplySubAdd(a.components.hi, b.components.hi, c.components.hi)}};
#endif
  }

  v_f64_4 negativeMultiplyAdd(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &c) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return negativeMultiplyAdd(a.components, b.components, c.components);
#else
    return {{negativeMultiplyAdd(a.components.lo, b.components.lo, c.components.lo),
             negativeMultiplyAdd(a.components.hi, b.components.hi, c.components.hi)}};
#endif
  }

  v_f64_4 negativeMultiplySub(const v_f64_4 &a, const v_f64_4 &b, const v_f64_4 &c) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return negativeMultiplySub(a.components, b.components, c.components);
#else
    return {{negativeMultiplySub(a.components.lo, b.components.lo, c.components.lo),
             negativeMultiplySub(a.components.hi, b.components.hi, c.components.hi)}};
#endif
  }

  v_f64_4::BoolType isNegative(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return movemask(v.components);
#else
    return movemask(v.components.lo)
        | (movemask(v.components.hi) << 2);
#endif
  }

  v_f64_4::VectorBoolType isNaN(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(isNaN(v.components));
#else
    return {{_mm_castpd_si128(isNaN(v.components.lo)), _mm_castpd_si128(isNaN(v.components.hi))}};
#endif
  }

  v_f64_4::VectorBoolType isNaN(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(isNaN(a.components, b.components));
#else
    return {{_mm_castpd_si128(isNaN(a.components.lo, b.components.lo)),
             _mm_castpd_si128(isNaN(a.components.hi, b.components.hi))}};
#endif
  }

  v_f64_4 addSubtract(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return addSubtract(a.components, b.components);
#else
    return {{addSubtract(a.components.lo, b.components.lo),
             addSubtract(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 sqrt(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return sqrt(v.components);
#else
    return {{sqrt(v.components.lo), sqrt(v.components.hi)}};
#endif
  }

  v_f64_4 reciprocal(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return reciprocal(v.components);
#else
    return {{reciprocal(v.components.lo), reciprocal(v.components.hi)}};
#endif
  }

  v_f64_4 rsqrt(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return rsqrt(v.components);
#else
    return {{rsqrt(v.components.lo), rsqrt(v.components.hi)}};
#endif
  }

  v_f64_4 ceil(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return ceil(v.components);
#else
    return {{ceil(v.components.lo), ceil(v.components.hi)}};
#endif
  }

  v_f64_4 floor(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return floor(v.components);
#else
    return {{floor(v.components.lo), floor(v.components.hi)}};
#endif
  }

  v_f64_4 round(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return round(v.components);
#else
    return {{round(v.components.lo), round(v.components.hi)}};
#endif
  }

  v_f64_4 fract(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return fract(v.components);
#else
    return {{fract(v.components.lo), fract(v.components.hi)}};
#endif
  }

  v_f64_4 modulo(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return modulo(a.components, b.components);
#else
    return {{modulo(a.components.lo, b.components.lo), modulo(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 abs(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return abs(v.components);
#else
    return {{abs(v.components.lo), abs(v.components.hi)}};
#endif
  }

  v_f64_4 clamp(const v_f64_4 &v, const v_f64_4 &lower, const v_f64_4 &upper) noexcept {
    return max(lower, min(v, upper));
  }

  bool anyTrue(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return movemask(v.components) != 0;
#else
    return movemask(v.components.lo) != 0
        || movemask(v.components.hi) != 0;
#endif
  }

  bool anyFalse(const v_f64_4 &v) noexcept {
    return !allTrue(v);
  }

  bool allTrue(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return movemask(v.components) == 0xF;
#else
    return movemask(v.components.lo) == 0x3
        && movemask(v.components.hi) == 0x3;
#endif
  }

  bool allFalse(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return movemask(v.components) == 0;
#else
    return movemask(v.components.lo) == 0
        && movemask(v.components.hi) == 0;
#endif
  }
}
