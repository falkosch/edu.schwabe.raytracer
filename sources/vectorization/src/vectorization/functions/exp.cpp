#include "vectorization/functions/exp.h"

#include <cmath>

namespace vectorization {
  Float_32 exp(const Float_32 value) noexcept {
    return std::exp(value);
  }

  Float_64 exp(const Float_64 value) noexcept {
    return std::exp(value);
  }
}
