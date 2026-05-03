#include "vectorization/v_f64_2/blends.h"

namespace vectorization {
  v_f64_2 blendMasked(const v_f64_2 &onBitNotSet, const v_f64_2 &onBitSet, const v_f64_2 &mask) noexcept {
    return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
  }

  v_f64_2
  blendMasked(const v_f64_2 &onBitNotSet, const v_f64_2 &onBitSet, const v_f64_2::VectorBoolType &mask) noexcept {
    return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
  }
}
