#include "vectorization/functions/sign.h"

#include "vectorization/functions/copysign.h"

#include "vectorization/accessors.h"
#include "vectorization/blends.h"
#include "vectorization/constants.h"

namespace vectorization
{
    const Int_8 sign(const Int_8 v) noexcept {
        return static_cast<Int_8>((v > Zero<Int_8>()) - (v < Zero<Int_8>()));
    }

    const Int_16 sign(const Int_16 v) noexcept {
        return static_cast<Int_8>((v > Zero<Int_16>()) - (v < Zero<Int_16>()));
    }

    const Int_32 sign(const Int_32 v) noexcept {
        return static_cast<Int_32>((v > Zero<Int_32>()) - (v < Zero<Int_32>()));
    }

    const Int_64 sign(const Int_64 v) noexcept {
        return static_cast<Int_64>(v > Zero<Int_64>()) - static_cast<Int_64>(v < Zero<Int_64>());
    }

    const PackedFloat4_128 sign(const PackedFloat4_128 & v) noexcept {
        return blendMasked(
            copysign(v),
            Zero<PackedFloat4_128>(),
            _mm_cmpeq_ps(
                Zero<PackedFloat4_128>(),
                _mm_andnot_ps(NegativeZero<PackedFloat4_128>(), v)
            )
        );
    }

    const PackedFloat2_128 sign(const PackedFloat2_128 & v) noexcept {
        return blendMasked(
            copysign(v),
            Zero<PackedFloat2_128>(),
            _mm_cmpeq_pd(
                Zero<PackedFloat2_128>(),
                _mm_andnot_pd(NegativeZero<PackedFloat2_128>(), v)
            )
        );
    }

    const Float_32 sign(const Float_32 v) noexcept {
        const PackedFloat4_128 vv = _mm_set_ss(v);
        return x(
            blendMasked(
                copysign(vv),
                Zero<PackedFloat4_128>(),
                _mm_cmpeq_ss(Zero<PackedFloat4_128>(),
                    _mm_andnot_ps(NegativeZero<PackedFloat4_128>(), vv)
                )
            )
        );
    }

    const Float_64 sign(const Float_64 v) noexcept {
        const PackedFloat2_128 vv = _mm_set_sd(v);
        return x(
            blendMasked(
                copysign(vv),
                Zero<PackedFloat2_128>(),
                _mm_cmpeq_sd(
                    Zero<PackedFloat2_128>(),
                    _mm_andnot_pd(NegativeZero<PackedFloat2_128>(), vv)
                )
            )
        );
    }
}