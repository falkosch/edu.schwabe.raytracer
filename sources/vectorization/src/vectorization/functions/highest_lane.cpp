#include "vectorization/functions/highest_lane.h"

#include "vectorization/constants/values/one.h"

namespace vectorization {
  ASizeT highestLane(const Int_32 laneBits) noexcept {
    static constexpr ASizeT HIGHEST_BIT = sizeof(UInt_32) * 8 - 1;
    // The lowest bit keeps the result at lane 0 when no bit is set.
    const auto bits = static_cast<UInt_32>(laneBits) | One<UInt_32>();
    return HIGHEST_BIT - static_cast<ASizeT>(_lzcnt_u32(bits));
  }
}
