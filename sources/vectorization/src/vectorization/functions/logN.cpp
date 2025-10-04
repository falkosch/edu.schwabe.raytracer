#include "vectorization/functions/logN.h"

#include "vectorization/functions/log.h"

namespace vectorization {
  Float_32 logN(const Float_32 value, const Float_32 base) noexcept {
    return log(value) / log(base);
  }

  Float_64 logN(const Float_64 value, const Float_64 base) noexcept {
    return log(value) / log(base);
  }
}
