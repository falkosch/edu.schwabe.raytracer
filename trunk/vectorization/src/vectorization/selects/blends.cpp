#include "vectorization/selects/blends.h"

namespace vectorization
{
    const bool blendMasked(const bool onBitNotSet, const bool onBitSet, const bool mask) noexcept {
        return mask ? onBitSet : onBitNotSet;
    }

    const Int_8 blendMasked(const Int_8 onBitNotSet, const Int_8 onBitSet, const Int_8 mask) noexcept {
        return static_cast<Int_8>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const UInt_8 blendMasked(const UInt_8 onBitNotSet, const UInt_8 onBitSet, const UInt_8 mask) noexcept {
        return static_cast<UInt_8>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const Int_16 blendMasked(const Int_16 onBitNotSet, const Int_16 onBitSet, const Int_16 mask) noexcept {
        return static_cast<Int_16>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const UInt_16 blendMasked(const UInt_16 onBitNotSet, const UInt_16 onBitSet, const UInt_16 mask) noexcept {
        return static_cast<UInt_16>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const Int_32 blendMasked(const Int_32 onBitNotSet, const Int_32 onBitSet, const Int_32 mask) noexcept {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const UInt_32 blendMasked(const UInt_32 onBitNotSet, const UInt_32 onBitSet, const UInt_32 mask) noexcept {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const Int_64 blendMasked(const Int_64 onBitNotSet, const Int_64 onBitSet, const Int_64 mask) noexcept {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const UInt_64 blendMasked(const UInt_64 onBitNotSet, const UInt_64 onBitSet, const UInt_64 mask) noexcept {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const Float_32 blendMasked(const Float_32 onBitNotSet, const Float_32 onBitSet, const BoolTypes<Float_32>::Type mask) noexcept {
        return _mm_cvtss_f32(
            blendMasked(
                _mm_set_ss(onBitNotSet),
                _mm_set_ss(onBitSet),
                _mm_castsi128_ps(_mm_set1_epi32(static_cast<int>(mask)))
            )
        );
    }

    const Float_64 blendMasked(const Float_64 onBitNotSet, const Float_64 onBitSet, const BoolTypes<Float_64>::Type mask) noexcept {
#ifdef ARCH_X64
        const PackedInts_128 m = _mm_set1_epi64x(mask);
#else
        PackedInts_128 m;
        reinterpret_cast<BoolTypes<Float_64>::Type * const>(&m)[VectorIndices::X] = mask;
#endif
        return _mm_cvtsd_f64(
            blendMasked(
                _mm_set_sd(onBitNotSet),
                _mm_set_sd(onBitSet),
                _mm_castsi128_pd(m)
            )
        );
    }

    const PackedInts_128 blendMasked(const PackedInts_128 & onBitNotSet, const PackedInts_128 & onBitSet, const PackedInts_128 & mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
        return _mm_or_si128(_mm_and_si128(mask, onBitSet), _mm_andnot_si128(mask, onBitNotSet));
#else
        return _mm_blendv_epi8(onBitNotSet, onBitSet, mask);
#endif
    }

    const PackedFloat4_128 blendMasked(const PackedFloat4_128 & onBitNotSet, const PackedFloat4_128 & onBitSet, const PackedFloat4_128 & mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
        return _mm_or_ps(_mm_and_ps(mask, onBitSet), _mm_andnot_ps(mask, onBitNotSet));
#else
        return _mm_blendv_ps(onBitNotSet, onBitSet, mask);
#endif
    }

    const PackedFloat2_128 blendMasked(const PackedFloat2_128 & onBitNotSet, const PackedFloat2_128 & onBitSet, const PackedFloat2_128 & mask) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
        return _mm_or_pd(_mm_and_pd(mask, onBitSet), _mm_andnot_pd(mask, onBitNotSet));
#else
        return _mm_blendv_pd(onBitNotSet, onBitSet, mask);
#endif
    }
}