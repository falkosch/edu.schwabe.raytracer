#include "vectorization/v_f64_4/selects.h"

#include "vectorization/v_f64_4/blends.h"

namespace vectorization {
  v_f64_4 select(const bool mask, const v_f64_4 &onTrue, const v_f64_4 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  v_f64_4 select(const v_f64_4::VectorBoolType &mask, const v_f64_4 &onTrue, const v_f64_4 &onFalse) noexcept {
    return blendMasked(onFalse, onTrue, mask);
  }

  v_f64_4 select(const v_f64_4 &mask, const v_f64_4 &onTrue, const v_f64_4 &onFalse) noexcept {
    return blendMasked(onFalse, onTrue, mask);
  }
}
