#include "vectorization/v_i64_4/functions.h"

#include "vectorization/v_i64_4/accessors.h"
#include "vectorization/v_i64_4/blends.h"
#include "vectorization/v_i64_4/constants.h"
#include "vectorization/v_i64_4/operators.h"
#include "vectorization/v_i64_4/swizzles.h"

#include "vectorization/constants/masks/mask_all.h"
#include "vectorization/functions/highest_lane.h"
#include "vectorization/functions/modulo.h"

namespace vectorization {
  v_i64_4 zeroW(const v_i64_4 &v) noexcept {
    return blend<false, false, false, true>(v, Zero<v_i64_4>());
  }

  v_i64_4 oneW(const v_i64_4 &v) noexcept {
    return blend<false, false, false, true>(v, One<v_i64_4>());
  }

  v_i64_4 andnot(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_andnot_si256(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_andnot_si128(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_andnot_si128(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_andnot_si128(a.components.lo, b.components.lo),
             _mm_andnot_si128(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4::BoolType isNegative(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_movemask_pd(_mm256_castsi256_pd(v.components));
#else
    return _mm_movemask_pd(_mm_castsi128_pd(v.components.lo))
        | (_mm_movemask_pd(_mm_castsi128_pd(v.components.hi)) << 2);
#endif
  }

  v_i64_4 abs(const v_i64_4 &v) noexcept {
    // _mm256_abs_epi64 needs AVX-512
    return blendMasked(v, -v, v < Zero<v_i64_4>());
  }

  v_i64_4 sqr(const v_i64_4 &v) noexcept {
    return v * v;
  }

  v_i64_4 min(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    // _mm256_min_epi64 needs AVX-512
    return blendMasked(a, b, a > b);
  }

  v_i64_4 minv(const v_i64_4 &v) noexcept {
    return min(min(xxxx(v), yyyy(v)), min(zzzz(v), wwww(v)));
  }

  v_i64_4::ValueType min(const v_i64_4 &v) noexcept {
    return x(minv(v));
  }

  v_i64_4 min3v(const v_i64_4 &v) noexcept {
    return min(min(xxxx(v), yyyy(v)), zzzz(v));
  }

  v_i64_4::ValueType min3(const v_i64_4 &v) noexcept {
    return x(min3v(v));
  }

  v_i64_4 max(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    // _mm256_max_epi64 needs AVX-512
    return blendMasked(b, a, a > b);
  }

  v_i64_4 maxv(const v_i64_4 &v) noexcept {
    return max(max(xxxx(v), yyyy(v)), max(zzzz(v), wwww(v)));
  }

  v_i64_4::ValueType max(const v_i64_4 &v) noexcept {
    return x(maxv(v));
  }

  v_i64_4 max3v(const v_i64_4 &v) noexcept {
    return max(max(xxxx(v), yyyy(v)), zzzz(v));
  }

  v_i64_4::ValueType max3(const v_i64_4 &v) noexcept {
    return x(max3v(v));
  }

  v_i64_4 horizontalSumv(const v_i64_4 &v) noexcept {
    return xxxx(v) + yyyy(v) + zzzz(v) + wwww(v);
  }

  v_i64_4::ValueType horizontalSum(const v_i64_4 &v) noexcept {
    return x(horizontalSumv(v));
  }

  v_i64_4 horizontalSum3v(const v_i64_4 &v) noexcept {
    return xxxx(v) + yyyy(v) + zzzz(v);
  }

  v_i64_4::ValueType horizontalSum3(const v_i64_4 &v) noexcept {
    return x(horizontalSum3v(v));
  }

  v_i64_4 dotv(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return horizontalSumv(a * b);
  }

  v_i64_4::ValueType dot(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return x(dotv(a, b));
  }

  v_i64_4 dot3v(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return horizontalSum3v(a * b);
  }

  v_i64_4::ValueType dot3(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return x(dot3v(a, b));
  }

  v_i64_4 lengthv(const v_i64_4 &v) noexcept {
    return horizontalSumv(abs(v));
  }

  v_i64_4::ValueType length(const v_i64_4 &v) noexcept {
    return x(lengthv(v));
  }

  v_i64_4 length3v(const v_i64_4 &v) noexcept {
    return horizontalSum3v(abs(v));
  }

  v_i64_4::ValueType length3(const v_i64_4 &v) noexcept {
    return x(length3v(v));
  }

  v_i64_4 distancev(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return lengthv(b - a);
  }

  v_i64_4::ValueType distance(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return x(distancev(a, b));
  }

  v_i64_4 distance3v(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return length3v(b - a);
  }

  v_i64_4::ValueType distance3(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return x(distance3v(a, b));
  }

  v_i64_4 multiplyAdd(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept {
    return a * b + c;
  }

  v_i64_4 multiplySub(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept {
    return a * b - c;
  }

  v_i64_4 multiplyAddSub(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept {
    return addSubtract(a * b, c);
  }

  v_i64_4 multiplySubAdd(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept {
    const v_i64_4 product = a * b;
    return blend<false, true, false, true>(product + c, product - c);
  }

  v_i64_4 negativeMultiplyAdd(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept {
    return c - a * b;
  }

  v_i64_4 negativeMultiplySub(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept {
    return -(a * b) - c;
  }

  v_i64_4 addSubtract(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return blend<false, true, false, true>(a - b, a + b);
  }

  v_i64_4 modulo(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    // No SIMD integer division
    return v_i64_4(modulo(x(a), x(b)), modulo(y(a), y(b)), modulo(z(a), z(b)), modulo(w(a), w(b)));
  }

  v_i64_4 clamp(const v_i64_4 &v, const v_i64_4 &lower, const v_i64_4 &upper) noexcept {
    return max(lower, min(v, upper));
  }

  ASizeT argmin(const v_i64_4 &v) noexcept {
    return highestLane(static_cast<Int_32>(isNegative(v == minv(v))));
  }

  ASizeT argmax(const v_i64_4 &v) noexcept {
    return highestLane(static_cast<Int_32>(isNegative(v == maxv(v))));
  }

  ASizeT argmin3(const v_i64_4 &v) noexcept {
    return highestLane(static_cast<Int_32>(isNegative(zeroW(v == min3v(v)))));
  }

  ASizeT argmax3(const v_i64_4 &v) noexcept {
    return highestLane(static_cast<Int_32>(isNegative(zeroW(v == max3v(v)))));
  }

  bool anyTrue(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return !_mm256_testz_si256(v.components, v.components);
#else
    return !(_mm_testz_si128(v.components.lo, v.components.lo)
          && _mm_testz_si128(v.components.hi, v.components.hi));
#endif
  }

  bool anyTrue3(const v_i64_4 &v) noexcept {
    return anyTrue(zeroW(v));
  }

  bool anyFalse(const v_i64_4 &v) noexcept {
    return !allTrue(v);
  }

  bool anyFalse3(const v_i64_4 &v) noexcept {
    // a cleared bit in x, y or z is a set bit in their complement
    return anyTrue3(~v);
  }

  bool allTrue(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_testc_si256(v.components, MaskAll<v_i64_4::PackedType>());
#else
    const auto ones = MaskAll<v_i64_4::HalfPackedType>();
    return _mm_testc_si128(v.components.lo, ones)
        && _mm_testc_si128(v.components.hi, ones);
#endif
  }

  bool allTrue3(const v_i64_4 &v) noexcept {
    return allFalse3(~v);
  }

  bool allFalse(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_testz_si256(v.components, v.components);
#else
    return _mm_testz_si128(v.components.lo, v.components.lo)
        && _mm_testz_si128(v.components.hi, v.components.hi);
#endif
  }

  bool allFalse3(const v_i64_4 &v) noexcept {
    return allFalse(zeroW(v));
  }
}
