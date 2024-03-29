#include "vectorization/functions/pow.h"

#include <cmath>

namespace vectorization {
  const Float_32 pow(const Float_32 base, const Float_32 exponent) noexcept {
    return std::pow(base, exponent);
  }

  const Float_64 pow(const Float_64 base, const Float_64 exponent) noexcept {
    return std::pow(base, exponent);
  }
}