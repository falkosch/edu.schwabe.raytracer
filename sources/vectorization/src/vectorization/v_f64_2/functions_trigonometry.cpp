#include "vectorization/v_f64_2/functions_trigonometry.h"

#include "vectorization/v_f64_2/accessors.h"

#include "vectorization/functions/cos.h"
#include "vectorization/functions/exp.h"
#include "vectorization/functions/log.h"
#include "vectorization/functions/pow.h"
#include "vectorization/functions/sin.h"

namespace vectorization {
  v_f64_2 log(const v_f64_2 &v) noexcept {
    return v_f64_2(log(x(v)), log(y(v)));
  }

  v_f64_2 exp(const v_f64_2 &v) noexcept {
    return v_f64_2(exp(x(v)), exp(y(v)));
  }

  v_f64_2 pow(const v_f64_2 &v, const v_f64_2 &exponent) noexcept {
    return v_f64_2(pow(x(v), x(exponent)), pow(y(v), y(exponent)));
  }

  v_f64_2 pow(const v_f64_2 &v, const v_f64_2::ValueType exponent) noexcept {
    return v_f64_2(pow(x(v), exponent), pow(y(v), exponent));
  }

  v_f64_2 sin(const v_f64_2 &v) noexcept {
    return v_f64_2(sin(x(v)), sin(y(v)));
  }

  v_f64_2 cos(const v_f64_2 &v) noexcept {
    return v_f64_2(cos(x(v)), cos(y(v)));
  }

  void sincos(const v_f64_2 &v, v_f64_2 &outSin, v_f64_2 &outCos) noexcept {
    outSin = v_f64_2(sin(x(v)), sin(y(v)));
    outCos = v_f64_2(cos(x(v)), cos(y(v)));
  }
}
