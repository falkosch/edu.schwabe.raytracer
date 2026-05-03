#include "vectorization/v_ui64_4/blends.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_masked_256i.h"
#else
#include "vectorization/blends/blend_masked_128i.h"
#endif

namespace vectorization {
  template <>
  v_ui64_4 blend<false, false, false, false>(const v_ui64_4 &a, const v_ui64_4 &) noexcept {
    return a;
  }

  template <>
  v_ui64_4 blend<true, true, true, true>(const v_ui64_4 &, const v_ui64_4 &b) noexcept {
    return b;
  }

  v_ui64_4 blendMasked(const v_ui64_4 &a, const v_ui64_4 &b, const v_ui64_4 &mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blendMasked(a.components, b.components, mask.components);
#else
    return {{blendMasked(a.components.lo, b.components.lo, mask.components.lo),
             blendMasked(a.components.hi, b.components.hi, mask.components.hi)}};
#endif
  }
}
