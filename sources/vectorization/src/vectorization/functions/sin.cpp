#include "vectorization/functions/sin.h"

#include <cmath>

namespace vectorization {
  Float_32 sin(const Float_32 value) noexcept {
    return std::sin(value);
  }

  Float_64 sin(const Float_64 value) noexcept {
    return std::sin(value);
  }
}
