#include "vectorization/v_f32_4x4.h"

#include "vectorization/functions.h"

namespace vectorization {
  v_f32_4x4 transpose(const v_f32_4x4 &m) noexcept {
    // adapted from glm::detail::sse_transpose_ps
    // http://glm.g-truc.net/
    const auto t0 = xy_xy(m.soaX, m.soaY);
    const auto t2 = zw_zw(m.soaX, m.soaY);
    const auto t1 = xy_xy(m.soaZ, m.soaW);
    const auto t3 = zw_zw(m.soaZ, m.soaW);
    return v_f32_4x4(xz_xz(t0, t1), yw_yw(t0, t1), xz_xz(t2, t3), yw_yw(t2, t3));
  }
}
