#include "vectorization/functions/log.h"

#include <cmath>

namespace vectorization {
  Float_32 log(const Float_32 value) noexcept {
    return std::log(value);
  }

  Float_64 log(const Float_64 value) noexcept {
    return std::log(value);
  }
}
