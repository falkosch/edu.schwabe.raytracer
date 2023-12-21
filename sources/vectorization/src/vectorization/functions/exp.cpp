#include "vectorization/functions/exp.h"

#include <cmath>

namespace vectorization {
  const Float_32 exp(const Float_32 v) noexcept {
    return std::exp(v);
  }

  const Float_64 exp(const Float_64 v) noexcept {
    return std::exp(v);
  }
}