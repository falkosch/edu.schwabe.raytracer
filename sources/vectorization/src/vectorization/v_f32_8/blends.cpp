#include "vectorization/v_f32_8/blends.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/blends/blend_masked_256s.h"
#else
#include "vectorization/blends/blend_masked_128s.h"
#endif

namespace vectorization {
  template <>
  v_f32_8 blend<false, false, false, false, false, false, false, false>(
      const v_f32_8 &a, const v_f32_8 &
  ) noexcept {
    return a;
  }

  template <>
  v_f32_8 blend<true, true, true, true, true, true, true, true>(
      const v_f32_8 &, const v_f32_8 &b
  ) noexcept {
    return b;
  }

  v_f32_8 blendMasked(const v_f32_8 &onBitNotSet, const v_f32_8 &onBitSet, const v_f32_8 &mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
#else
    return {{blendMasked(onBitNotSet.components.lo, onBitSet.components.lo, mask.components.lo),
             blendMasked(onBitNotSet.components.hi, onBitSet.components.hi, mask.components.hi)}};
#endif
  }

  v_f32_8 blendMasked(
      const v_f32_8 &onBitNotSet, const v_f32_8 &onBitSet, const v_f32_8::VectorBoolType &mask
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
#else
    return {{blendMasked(onBitNotSet.components.lo, onBitSet.components.lo, _mm_castsi128_ps(mask.components.lo)),
             blendMasked(onBitNotSet.components.hi, onBitSet.components.hi, _mm_castsi128_ps(mask.components.hi))}};
#endif
  }
}
