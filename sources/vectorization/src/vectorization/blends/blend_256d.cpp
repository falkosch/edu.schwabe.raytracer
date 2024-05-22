#include "vectorization/blends/blend_256d.h"

namespace vectorization {
  template <>
  PackedFloat4_256
  blend<false, false, false, false>(const PackedFloat4_256 &a, const PackedFloat4_256 & /*unused*/) noexcept {
    return a;
  }

  template <>
  PackedFloat4_256
  blend<true, true, true, true>(const PackedFloat4_256 & /*unused*/, const PackedFloat4_256 &b) noexcept {
    return b;
  }
}
