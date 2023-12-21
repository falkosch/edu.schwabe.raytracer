#include "vectorization/blends/blend_256d.h"

namespace vectorization {
  template <>
  const PackedFloat4_256
  blend<false, false, false, false>(const PackedFloat4_256 &a, const PackedFloat4_256 &) noexcept {
    return a;
  }

  template <>
  const PackedFloat4_256 blend<true, true, true, true>(const PackedFloat4_256 &, const PackedFloat4_256 &b) noexcept {
    return b;
  }
}