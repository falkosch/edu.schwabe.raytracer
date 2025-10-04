#include "vectorization/functions/tan.h"

#include <cmath>

namespace vectorization {
  Float_32 tan(const Float_32 value) noexcept {
    return std::tan(value);
  }

  Float_64 tan(const Float_64 value) noexcept {
    return std::tan(value);
  }
}
