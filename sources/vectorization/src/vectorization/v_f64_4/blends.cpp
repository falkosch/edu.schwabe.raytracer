#include "vectorization/v_f64_4/blends.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_masked_256d.h"
#else
#include "vectorization/blends/blend_masked_128d.h"
#endif

namespace vectorization {
  template <>
  v_f64_4 blend<false, false, false, false>(
      const v_f64_4 &a, const v_f64_4 &
  ) noexcept {
    return a;
  }

  template <>
  v_f64_4 blend<true, true, true, true>(
      const v_f64_4 &, const v_f64_4 &b
  ) noexcept {
    return b;
  }

  v_f64_4 blendMasked(const v_f64_4 &onBitNotSet, const v_f64_4 &onBitSet, const v_f64_4 &mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
#else
    return {{blendMasked(onBitNotSet.components.lo, onBitSet.components.lo, mask.components.lo),
             blendMasked(onBitNotSet.components.hi, onBitSet.components.hi, mask.components.hi)}};
#endif
  }

  v_f64_4 blendMasked(
      const v_f64_4 &onBitNotSet, const v_f64_4 &onBitSet, const v_f64_4::VectorBoolType &mask
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
#else
    return {{blendMasked(onBitNotSet.components.lo, onBitSet.components.lo, _mm_castsi128_pd(mask.components.lo)),
             blendMasked(onBitNotSet.components.hi, onBitSet.components.hi, _mm_castsi128_pd(mask.components.hi))}};
#endif
  }
}
