#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // Returns the index of the highest set bit in a lane bitmask (one bit per lane, as movemask and isNegative return
  // it), or 0 if no bit is set. argmin and argmax use it to pick the highest of several equal lanes.
  ASizeT highestLane(Int_32 laneBits) noexcept;
}
