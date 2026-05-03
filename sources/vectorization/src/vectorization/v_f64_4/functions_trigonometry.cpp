#include "vectorization/v_f64_4/functions_trigonometry.h"

#include "vectorization/v_f64_4/accessors.h"

#include <cmath>

#include "vectorization/functions/exp.h"
#include "vectorization/functions/log.h"
#include "vectorization/functions/pow.h"

namespace vectorization {
  v_f64_4 log(const v_f64_4 &v) noexcept {
    return v_f64_4(std::log(x(v)), std::log(y(v)), std::log(z(v)), std::log(w(v)));
  }

  v_f64_4 exp(const v_f64_4 &v) noexcept {
    return v_f64_4(std::exp(x(v)), std::exp(y(v)), std::exp(z(v)), std::exp(w(v)));
  }

  v_f64_4 pow(const v_f64_4 &v, const v_f64_4 &exponent) noexcept {
    return v_f64_4(
        std::pow(x(v), x(exponent)), std::pow(y(v), y(exponent)),
        std::pow(z(v), z(exponent)), std::pow(w(v), w(exponent))
    );
  }

  v_f64_4 pow(const v_f64_4 &v, const v_f64_4::ValueType exponent) noexcept {
    return v_f64_4(
        std::pow(x(v), exponent), std::pow(y(v), exponent),
        std::pow(z(v), exponent), std::pow(w(v), exponent)
    );
  }

  v_f64_4 sin(const v_f64_4 &v) noexcept {
    return v_f64_4(std::sin(x(v)), std::sin(y(v)), std::sin(z(v)), std::sin(w(v)));
  }

  v_f64_4 cos(const v_f64_4 &v) noexcept {
    return v_f64_4(std::cos(x(v)), std::cos(y(v)), std::cos(z(v)), std::cos(w(v)));
  }

  void sincos(const v_f64_4 &v, v_f64_4 &outSin, v_f64_4 &outCos) noexcept {
    outSin = v_f64_4(std::sin(x(v)), std::sin(y(v)), std::sin(z(v)), std::sin(w(v)));
    outCos = v_f64_4(std::cos(x(v)), std::cos(y(v)), std::cos(z(v)), std::cos(w(v)));
  }
}
