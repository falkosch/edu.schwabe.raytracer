#include "vectorization/functions/cos.h"

#include <cmath>

namespace vectorization {
  Float_32 cos(const Float_32 value) noexcept {
    return std::cos(value);
  }

  Float_64 cos(const Float_64 value) noexcept {
    return std::cos(value);
  }
}
