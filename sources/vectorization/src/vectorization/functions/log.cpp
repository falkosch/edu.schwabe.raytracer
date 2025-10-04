#include "vectorization/functions/log.h"

#include <cmath>

namespace vectorization {
  const Float_32 log(const Float_32 v) noexcept {
    return std::log(v);
  }

  const Float_64 log(const Float_64 v) noexcept {
    return std::log(v);
  }
}