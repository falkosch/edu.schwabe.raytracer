#include "vectorization/selects.h"
#include "../stdafx.h"

namespace vectorization
{

#pragma region blendMasked()
    //{ blendMasked()

    const bool blendMasked(const bool onBitNotSet, const bool onBitSet, const bool mask)
    {
        return mask ? onBitSet : onBitNotSet;
    }

    const long blendMasked(const long onBitNotSet, const long onBitSet, const long mask)
    {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const unsigned long blendMasked(const unsigned long onBitNotSet, const unsigned long onBitSet, const unsigned long mask)
    {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const Int_8 blendMasked(const Int_8 onBitNotSet, const Int_8 onBitSet, const Int_8 mask)
    {
        return static_cast<Int_8>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const UInt_8 blendMasked(const UInt_8 onBitNotSet, const UInt_8 onBitSet, const UInt_8 mask)
    {
        return static_cast<UInt_8>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const Int_16 blendMasked(const Int_16 onBitNotSet, const Int_16 onBitSet, const Int_16 mask)
    {
        return static_cast<Int_16>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const UInt_16 blendMasked(const UInt_16 onBitNotSet, const UInt_16 onBitSet, const UInt_16 mask)
    {
        return static_cast<UInt_16>((mask & onBitSet) | (~mask & onBitNotSet));
    }

    const Int_32 blendMasked(const Int_32 onBitNotSet, const Int_32 onBitSet, const Int_32 mask)
    {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const UInt_32 blendMasked(const UInt_32 onBitNotSet, const UInt_32 onBitSet, const UInt_32 mask)
    {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const Int_64 blendMasked(const Int_64 onBitNotSet, const Int_64 onBitSet, const Int_64 mask)
    {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const UInt_64 blendMasked(const UInt_64 onBitNotSet, const UInt_64 onBitSet, const UInt_64 mask)
    {
        return (mask & onBitSet) | (~mask & onBitNotSet);
    }

    const Float_32 blendMasked(const Float_32 onBitNotSet, const Float_32 onBitSet, const BoolTypes<Float_32>::Type mask)
    {
        return _mm_cvtss_f32(blendMasked(
            _mm_set_ss(onBitNotSet),
            _mm_set_ss(onBitSet),
            _mm_castsi128_ps(_mm_set1_epi32(static_cast<int>(mask)))));
    }

    const Float_64 blendMasked(const Float_64 onBitNotSet, const Float_64 onBitSet, const BoolTypes<Float_64>::Type mask)
    {
#ifdef ARCH_X64
        const PackedInts_128 m = _mm_set1_epi64x(mask);
#else
        PackedInts_128 m;
        reinterpret_cast<BoolTypes<Float_64>::Type * const>(&m)[VectorIndices::X] = mask;
#endif
        return _mm_cvtsd_f64(blendMasked(
            _mm_set_sd(onBitNotSet),
            _mm_set_sd(onBitSet),
            _mm_castsi128_pd(m)));
    }

    const PackedInts_128 blendMasked(const PackedInts_128 & onBitNotSet, const PackedInts_128 & onBitSet, const PackedInts_128 & mask)
    {
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
        return _mm_or_si128(_mm_and_si128(mask, onBitSet), _mm_andnot_si128(mask, onBitNotSet));
#else
        return _mm_blendv_epi8(onBitNotSet, onBitSet, mask);
#endif
    }

    const PackedFloat4_128 blendMasked(const PackedFloat4_128 & onBitNotSet, const PackedFloat4_128 & onBitSet, const PackedFloat4_128 & mask)
    {
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
        return _mm_or_ps(_mm_and_ps(mask, onBitSet), _mm_andnot_ps(mask, onBitNotSet));
#else
        return _mm_blendv_ps(onBitNotSet, onBitSet, mask);
#endif
    }

    const PackedFloat2_128 blendMasked(const PackedFloat2_128 & onBitNotSet, const PackedFloat2_128 & onBitSet, const PackedFloat2_128 & mask)
    {
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
        return _mm_or_pd(_mm_and_pd(mask, onBitSet), _mm_andnot_pd(mask, onBitNotSet));
#else
        return _mm_blendv_pd(onBitNotSet, onBitSet, mask);
#endif
    }

    //}
#pragma endregion

#pragma region select()
    //{ select()

    const bool select(const bool mask, const bool onTrue, const bool onFalse)
    {
        return mask ? onTrue : onFalse;
    }

    const long select(const long mask, const long onTrue, const long onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<long>() : Zero<long>());
    }

    const unsigned long select(const unsigned long mask, const unsigned long onTrue, const unsigned long onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<unsigned long>() : Zero<unsigned long>());
    }

    const Int_8 select(const Int_8 mask, const Int_8 onTrue, const Int_8 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_8>() : Zero<Int_8>());
    }

    const UInt_8 select(const UInt_8 mask, const UInt_8 onTrue, const UInt_8 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_8>() : Zero<UInt_8>());
    }

    const Int_16 select(const Int_16 mask, const Int_16 onTrue, const Int_16 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_16>() : Zero<Int_16>());
    }

    const UInt_16 select(const UInt_16 mask, const UInt_16 onTrue, const UInt_16 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_16>() : Zero<UInt_16>());
    }

    const Int_32 select(const Int_32 mask, const Int_32 onTrue, const Int_32 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_32>() : Zero<Int_32>());
    }

    const UInt_32 select(const UInt_32 mask, const UInt_32 onTrue, const UInt_32 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_32>() : Zero<UInt_32>());
    }

    const Int_64 select(const Int_64 mask, const Int_64 onTrue, const Int_64 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_64>() : Zero<Int_64>());
    }

    const UInt_64 select(const UInt_64 mask, const UInt_64 onTrue, const UInt_64 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_64>() : Zero<UInt_64>());
    }

    const long select(const bool mask, const long onTrue, const long onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<long>(mask));
    }

    const unsigned long select(const bool mask, const unsigned long onTrue, const unsigned long onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<unsigned long>(mask));
    }

    const Int_8 select(const bool mask, const Int_8 onTrue, const Int_8 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<Int_8>(mask));
    }

    const UInt_8 select(const bool mask, const UInt_8 onTrue, const UInt_8 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<UInt_8>(mask));
    }

    const Int_16 select(const bool mask, const Int_16 onTrue, const Int_16 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<Int_16>(mask));
    }

    const UInt_16 select(const bool mask, const UInt_16 onTrue, const UInt_16 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<UInt_16>(mask));
    }

    const Int_32 select(const bool mask, const Int_32 onTrue, const Int_32 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<Int_32>(mask));
    }

    const UInt_32 select(const bool mask, const UInt_32 onTrue, const UInt_32 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<UInt_32>(mask));
    }

    const Int_64 select(const bool mask, const Int_64 onTrue, const Int_64 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<Int_64>(mask));
    }

    const UInt_64 select(const bool mask, const UInt_64 onTrue, const UInt_64 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<UInt_64>(mask));
    }

    const Float_32 select(const bool mask, const Float_32 onTrue, const Float_32 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<BoolTypes<Float_32>::Type>(mask));
    }

    const Float_32 select(const BoolTypes<Float_32>::Type mask, const Float_32 onTrue, const Float_32 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<BoolTypes<Float_32>::Type>() : Zero<BoolTypes<Float_32>::Type>());
    }

    const Float_64 select(const bool mask, const Float_64 onTrue, const Float_64 onFalse)
    {
        return blendMasked(onFalse, onTrue, boolMask<BoolTypes<Float_64>::Type>(mask));
    }

    const Float_64 select(const BoolTypes<Float_64>::Type mask, const Float_64 onTrue, const Float_64 onFalse)
    {
        return blendMasked(onFalse, onTrue, mask ? MaskAll<BoolTypes<Float_64>::Type>() : Zero<BoolTypes<Float_64>::Type>());
    }

    const PackedInts_128 select(const bool mask, const PackedInts_128 & onTrue, const PackedInts_128 & onFalse)
    {
        return blendMasked(onFalse, onTrue, _mm_set1_epi32(boolMask<int>(mask)));
    }

    const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse)
    {
        return blendMasked(onFalse, onTrue, _mm_castsi128_ps(_mm_set1_epi32(boolMask<int>(mask))));
    }

    const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse)
    {
        // reverse onTrue & onFalse since we test for mask components to be zero
        return blendMasked(onTrue, onFalse, _mm_cmpeq_ps(mask, Zero<PackedFloat4_128>()));
    }

    const PackedFloat2_128 select(const bool mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse)
    {
        return blendMasked(onFalse, onTrue, _mm_castsi128_pd(_mm_set1_epi32(boolMask<int>(mask))));
    }

    const PackedFloat2_128 select(const PackedFloat2_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse)
    {
        // reverse onTrue & onFalse since we test for mask components to be zero
        return blendMasked(onTrue, onFalse, _mm_cmpeq_pd(mask, Zero<PackedFloat2_128>()));
    }

    //}
#pragma endregion

}
