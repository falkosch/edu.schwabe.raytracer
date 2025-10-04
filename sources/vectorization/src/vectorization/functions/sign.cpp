#include "vectorization/functions/sign.h"

#include "vectorization/functions/copySign.h"

#include "vectorization/accessors.h"
#include "vectorization/blends.h"
#include "vectorization/constants.h"

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
}
