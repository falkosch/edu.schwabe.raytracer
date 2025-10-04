#include "vectorization/functions/tan.h"

#include <cmath>

namespace vectorization {
  const Float_32 tan(const Float_32 v) noexcept {
    return std::tan(v);
  }

  const Float_64 tan(const Float_64 v) noexcept {
    return std::tan(v);
  }
}