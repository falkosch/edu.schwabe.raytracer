#include "vectorization/blends/blend_256i.h"

namespace vectorization {
  template <>
  PackedInts_256 blend<
      false, false, false, false, false, false, false,
      false>(const PackedInts_256 &a, const PackedInts_256 & /*unused*/) noexcept {
    return a;
  }

  template <>
  PackedInts_256 blend<true, true, true, true, true, true, true, true>(
      const PackedInts_256 & /*unused*/, const PackedInts_256 &b
  ) noexcept {
    return b;
  }
}
