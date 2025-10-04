#include "vectorization/blends/blend_128d.h"

namespace vectorization {
  template <>
  // pass through a
  PackedFloat2_128 blend<false, false>(const PackedFloat2_128 &a, const PackedFloat2_128 & /*unused*/) noexcept {
    return a;
  }

  template <>
  // pass through b
  PackedFloat2_128 blend<true, true>(const PackedFloat2_128 & /*unused*/, const PackedFloat2_128 &b) noexcept {
    return b;
  }

  template <>
  // x(a), y(b)
  PackedFloat2_128 blend<false, true>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    // argument order is indeed (b, a)
    return _mm_move_sd(b, a);
  }

  template <>
  // x(b), y(a)
  PackedFloat2_128 blend<true, false>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    // argument order is indeed (b, a)
    return blend<false, true>(b, a);
  }
}
