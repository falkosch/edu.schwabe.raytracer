#include "vectorization/v_i32_8/blends.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_256i.h"
#include "vectorization/blends/blend_masked_256i.h"
#else
#include "vectorization/blends/blend_masked_128i.h"
#endif

namespace vectorization {
  template <>
  v_i32_8 blend<false, false, false, false, false, false, false, false>(const v_i32_8 &a, const v_i32_8 &) noexcept {
    return a;
  }

  template <>
  v_i32_8 blend<true, true, true, true, true, true, true, true>(const v_i32_8 &, const v_i32_8 &b) noexcept {
    return b;
  }

  v_i32_8 blendMasked(const v_i32_8 &a, const v_i32_8 &b, const v_i32_8 &mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blendMasked(a.components, b.components, mask.components);
#else
    return {{blendMasked(a.components.lo, b.components.lo, mask.components.lo),
             blendMasked(a.components.hi, b.components.hi, mask.components.hi)}};
#endif
  }
}
