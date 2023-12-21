#include "vectorization/functions/acos.h"

#include <cmath>

namespace vectorization {
  const Float_32 acos(const Float_32 v) noexcept {
    return std::acos(v);
  }

  const Float_64 acos(const Float_64 v) noexcept {
    return std::acos(v);
  }
}