#include "vectorization/v_f32_8/functions_trigonometry.h"

#include "vectorization/v_f32_8/accessors.h"

#include "vectorization/functions/cos.h"
#include "vectorization/functions/exp.h"
#include "vectorization/functions/log.h"
#include "vectorization/functions/pow.h"
#include "vectorization/functions/sin.h"

#define USE_THIRD_PARTY 1

#if defined(USE_THIRD_PARTY)

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include <avx_mathfun.h>
#else
#define USE_SSE2 1
#include <sse_mathfun.h>
#undef USE_SSE2
#endif

#endif

namespace vectorization {
  v_f32_8 log(const v_f32_8 &v) noexcept {
#ifdef USE_THIRD_PARTY
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return log256_ps(v.components);
#else
    return {{log_ps(v.components.lo), log_ps(v.components.hi)}};
#endif
#else
    return v_f32_8(log(x1(v)), log(x2(v)), log(x3(v)), log(x4(v)),
                   log(x5(v)), log(x6(v)), log(x7(v)), log(x8(v)));
#endif
  }

  v_f32_8 exp(const v_f32_8 &v) noexcept {
#ifdef USE_THIRD_PARTY
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return exp256_ps(v.components);
#else
    return {{exp_ps(v.components.lo), exp_ps(v.components.hi)}};
#endif
#else
    return v_f32_8(exp(x1(v)), exp(x2(v)), exp(x3(v)), exp(x4(v)),
                   exp(x5(v)), exp(x6(v)), exp(x7(v)), exp(x8(v)));
#endif
  }

  v_f32_8 pow(const v_f32_8 &v, const v_f32_8 &exponent) noexcept {
    return v_f32_8(
        pow(x1(v), x1(exponent)), pow(x2(v), x2(exponent)),
        pow(x3(v), x3(exponent)), pow(x4(v), x4(exponent)),
        pow(x5(v), x5(exponent)), pow(x6(v), x6(exponent)),
        pow(x7(v), x7(exponent)), pow(x8(v), x8(exponent))
    );
  }

  v_f32_8 pow(const v_f32_8 &v, const v_f32_8::ValueType exponent) noexcept {
    return v_f32_8(
        pow(x1(v), exponent), pow(x2(v), exponent),
        pow(x3(v), exponent), pow(x4(v), exponent),
        pow(x5(v), exponent), pow(x6(v), exponent),
        pow(x7(v), exponent), pow(x8(v), exponent)
    );
  }

  v_f32_8 sin(const v_f32_8 &v) noexcept {
#ifdef USE_THIRD_PARTY
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return sin256_ps(v.components);
#else
    return {{sin_ps(v.components.lo), sin_ps(v.components.hi)}};
#endif
#else
    return v_f32_8(sin(x1(v)), sin(x2(v)), sin(x3(v)), sin(x4(v)),
                   sin(x5(v)), sin(x6(v)), sin(x7(v)), sin(x8(v)));
#endif
  }

  v_f32_8 cos(const v_f32_8 &v) noexcept {
#ifdef USE_THIRD_PARTY
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return cos256_ps(v.components);
#else
    return {{cos_ps(v.components.lo), cos_ps(v.components.hi)}};
#endif
#else
    return v_f32_8(cos(x1(v)), cos(x2(v)), cos(x3(v)), cos(x4(v)),
                   cos(x5(v)), cos(x6(v)), cos(x7(v)), cos(x8(v)));
#endif
  }

  void sincos(const v_f32_8 &v, v_f32_8 &outSin, v_f32_8 &outCos) noexcept {
#ifdef USE_THIRD_PARTY
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    v8sf s;
    v8sf c;
    sincos256_ps(v.components, &s, &c);
    outSin = s;
    outCos = c;
#else
    v4sf slo, shi, clo, chi;
    sincos_ps(v.components.lo, &slo, &clo);
    sincos_ps(v.components.hi, &shi, &chi);
    outSin = {{slo, shi}};
    outCos = {{clo, chi}};
#endif
#else
    outSin = v_f32_8(sin(x1(v)), sin(x2(v)), sin(x3(v)), sin(x4(v)),
                     sin(x5(v)), sin(x6(v)), sin(x7(v)), sin(x8(v)));
    outCos = v_f32_8(cos(x1(v)), cos(x2(v)), cos(x3(v)), cos(x4(v)),
                     cos(x5(v)), cos(x6(v)), cos(x7(v)), cos(x8(v)));
#endif
  }
}
