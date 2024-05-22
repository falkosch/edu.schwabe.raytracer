#include "vectorization/functions/acos.h"

#include <cmath>

namespace vectorization {
  Float_32 acos(const Float_32 value) noexcept {
    return std::acos(value);
  }

  Float_64 acos(const Float_64 value) noexcept {
    return std::acos(value);
  }
}
