#include "vectorization/v_f64_2/swizzles.h"

namespace vectorization {
  v_f64_2 xx(const v_f64_2 &v) noexcept {
    return xx(v.components);
  }

  v_f64_2 yx(const v_f64_2 &v) noexcept {
    return yx(v.components);
  }

  v_f64_2 yy(const v_f64_2 &v) noexcept {
    return yy(v.components);
  }
}
