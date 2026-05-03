#pragma once

#include "type.h"

#include "vectorization/blends.h"

namespace vectorization {
  template <bool SelectX, bool SelectY>
  // generic blend of two vectors
  v_f64_2 blend(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return blend<SelectX, SelectY>(a.components, b.components);
  }

  v_f64_2 blendMasked(const v_f64_2 &onBitNotSet, const v_f64_2 &onBitSet, const v_f64_2 &mask) noexcept;

  v_f64_2
  blendMasked(const v_f64_2 &onBitNotSet, const v_f64_2 &onBitSet, const v_f64_2::VectorBoolType &mask) noexcept;
}
