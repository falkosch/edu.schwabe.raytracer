#include "vectorization/v_f64_2/selects.h"

#include "vectorization/selects/selects_128d.h"
#include "vectorization/selects/selects_128i.h"
#include "vectorization/selects/selects_scalar.h"

namespace vectorization {
  v_f64_2 select(const bool mask, const v_f64_2 &onTrue, const v_f64_2 &onFalse) noexcept {
    return select(mask, onTrue.components, onFalse.components);
  }

  v_f64_2 select(const v_f64_2::VectorBoolType &mask, const v_f64_2 &onTrue, const v_f64_2 &onFalse) noexcept {
    return select(mask.components, onTrue.components, onFalse.components);
  }

  v_f64_2 select(const v_f64_2 &mask, const v_f64_2 &onTrue, const v_f64_2 &onFalse) noexcept {
    return select(mask.components, onTrue.components, onFalse.components);
  }
}
