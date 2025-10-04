#include "vectorization/functions/logN.h"

#include "vectorization/functions/log.h"

namespace vectorization {
  const Float_32 logN(const Float_32 v, const Float_32 logBase) noexcept {
    return log(v) / log(logBase);
  }

  const Float_64 logN(const Float_64 v, const Float_64 logBase) noexcept {
    return log(v) / log(logBase);
  }
}