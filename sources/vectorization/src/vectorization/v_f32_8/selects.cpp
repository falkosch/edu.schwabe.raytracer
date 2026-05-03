#include "vectorization/v_f32_8/selects.h"

#include "vectorization/v_f32_8/blends.h"

namespace vectorization {
  v_f32_8 select(const bool mask, const v_f32_8 &onTrue, const v_f32_8 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  v_f32_8 select(const v_f32_8::VectorBoolType &mask, const v_f32_8 &onTrue, const v_f32_8 &onFalse) noexcept {
    return blendMasked(onFalse, onTrue, mask);
  }

  v_f32_8 select(const v_f32_8 &mask, const v_f32_8 &onTrue, const v_f32_8 &onFalse) noexcept {
    return blendMasked(onFalse, onTrue, mask);
  }
}
