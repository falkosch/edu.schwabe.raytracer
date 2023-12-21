#include "vectorization/functions/cos.h"

#include <cmath>

namespace vectorization {
  const Float_32 cos(const Float_32 v) noexcept {
    return std::cos(v);
  }

  const Float_64 cos(const Float_64 v) noexcept {
    return std::cos(v);
  }
}