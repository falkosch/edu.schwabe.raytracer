#include "vectorization/functions/sign.h"

#include "vectorization/functions/copysign.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#include "vectorization/blends/blend_masked_128d.h"
#include "vectorization/blends/blend_masked_128s.h"
#include "vectorization/blends/blend_masked_256d.h"
#include "vectorization/blends/blend_masked_256s.h"
#include "vectorization/constants/masks.h"
#include "vectorization/constants/values.h"

namespace vectorization {
  Int_8 sign(const Int_8 value) noexcept {
    return static_cast<Int_8>((value > Zero<Int_8>()) - (value < Zero<Int_8>()));
  }

  Int_16 sign(const Int_16 value) noexcept {
    return static_cast<Int_8>((value > Zero<Int_16>()) - (value < Zero<Int_16>()));
  }

  Int_32 sign(const Int_32 value) noexcept {
    return static_cast<Int_32>((value > Zero<Int_32>()) - (value < Zero<Int_32>()));
  }

  Int_64 sign(const Int_64 value) noexcept {
    return static_cast<Int_64>(value > Zero<Int_64>()) - static_cast<Int_64>(value < Zero<Int_64>());
  }

  Float_32 sign(const Float_32 value) noexcept {
    const PackedFloat4_128 vv = _mm_set_ss(value);
    return x(blendMasked(
        copySign(vv), Zero<PackedFloat4_128>(),
        _mm_cmpeq_ss(Zero<PackedFloat4_128>(), _mm_andnot_ps(NegativeZero<PackedFloat4_128>(), vv))
    ));
  }

  Float_64 sign(const Float_64 value) noexcept {
    const PackedFloat2_128 vv = _mm_set_sd(value);
    return x(blendMasked(
        copySign(vv), Zero<PackedFloat2_128>(),
        _mm_cmpeq_sd(Zero<PackedFloat2_128>(), _mm_andnot_pd(NegativeZero<PackedFloat2_128>(), vv))
    ));
  }

  PackedFloat4_128 sign(const PackedFloat4_128 &values) noexcept {
    return blendMasked(
        copySign(values), Zero<PackedFloat4_128>(),
        _mm_cmpeq_ps(Zero<PackedFloat4_128>(), _mm_andnot_ps(NegativeZero<PackedFloat4_128>(), values))
    );
  }

  PackedFloat2_128 sign(const PackedFloat2_128 &values) noexcept {
    return blendMasked(
        copySign(values), Zero<PackedFloat2_128>(),
        _mm_cmpeq_pd(Zero<PackedFloat2_128>(), _mm_andnot_pd(NegativeZero<PackedFloat2_128>(), values))
    );
  }

  // _CMP_EQ_UQ: NaN input produces sign(NaN)=0, matching the 128-bit _mm_cmpeq_ps unordered behavior
  PackedFloat8_256 sign(const PackedFloat8_256 &values) noexcept {
    return blendMasked(
        copySign(values), Zero<PackedFloat8_256>(),
        _mm256_cmp_ps(Zero<PackedFloat8_256>(), _mm256_andnot_ps(NegativeZero<PackedFloat8_256>(), values), _CMP_EQ_UQ)
    );
  }

  PackedFloat4_256 sign(const PackedFloat4_256 &values) noexcept {
    return blendMasked(
        copySign(values), Zero<PackedFloat4_256>(),
        _mm256_cmp_pd(Zero<PackedFloat4_256>(), _mm256_andnot_pd(NegativeZero<PackedFloat4_256>(), values), _CMP_EQ_UQ)
    );
  }
}
