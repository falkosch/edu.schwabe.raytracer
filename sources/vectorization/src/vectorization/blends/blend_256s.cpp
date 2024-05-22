#include "vectorization/blends/blend_256s.h"

namespace vectorization {
  template <>
  PackedFloat8_256 blend<
      false, false, false, false, false, false, false,
      false>(const PackedFloat8_256 &a, const PackedFloat8_256 & /*unused*/) noexcept {
    return a;
  }

  template <>
  PackedFloat8_256 blend<true, true, true, true, true, true, true, true>(
      const PackedFloat8_256 & /*unused*/, const PackedFloat8_256 &b
  ) noexcept {
    return b;
  }
}
