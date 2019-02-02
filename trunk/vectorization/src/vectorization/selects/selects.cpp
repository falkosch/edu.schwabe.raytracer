#include "vectorization/selects/selects.h"

#include "vectorization/selects/blends.h"

namespace vectorization
{

	const bool select(const bool mask, const bool onTrue, const bool onFalse) noexcept
	{
		return mask ? onTrue : onFalse;
	}

	const long select(const long mask, const long onTrue, const long onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<long>() : Zero<long>());
	}

	const unsigned long select(const unsigned long mask, const unsigned long onTrue, const unsigned long onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<unsigned long>() : Zero<unsigned long>());
	}

	const Int_8 select(const Int_8 mask, const Int_8 onTrue, const Int_8 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_8>() : Zero<Int_8>());
	}

	const UInt_8 select(const UInt_8 mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_8>() : Zero<UInt_8>());
	}

	const Int_16 select(const Int_16 mask, const Int_16 onTrue, const Int_16 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_16>() : Zero<Int_16>());
	}

	const UInt_16 select(const UInt_16 mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_16>() : Zero<UInt_16>());
	}

	const Int_32 select(const Int_32 mask, const Int_32 onTrue, const Int_32 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_32>() : Zero<Int_32>());
	}

	const UInt_32 select(const UInt_32 mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_32>() : Zero<UInt_32>());
	}

	const Int_64 select(const Int_64 mask, const Int_64 onTrue, const Int_64 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<Int_64>() : Zero<Int_64>());
	}

	const UInt_64 select(const UInt_64 mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<UInt_64>() : Zero<UInt_64>());
	}

	const long select(const bool mask, const long onTrue, const long onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<long>(mask));
	}

	const unsigned long select(const bool mask, const unsigned long onTrue, const unsigned long onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<unsigned long>(mask));
	}

	const Int_8 select(const bool mask, const Int_8 onTrue, const Int_8 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<Int_8>(mask));
	}

	const UInt_8 select(const bool mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<UInt_8>(mask));
	}

	const Int_16 select(const bool mask, const Int_16 onTrue, const Int_16 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<Int_16>(mask));
	}

	const UInt_16 select(const bool mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<UInt_16>(mask));
	}

	const Int_32 select(const bool mask, const Int_32 onTrue, const Int_32 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<Int_32>(mask));
	}

	const UInt_32 select(const bool mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<UInt_32>(mask));
	}

	const Int_64 select(const bool mask, const Int_64 onTrue, const Int_64 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<Int_64>(mask));
	}

	const UInt_64 select(const bool mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<UInt_64>(mask));
	}

	const Float_32 select(const bool mask, const Float_32 onTrue, const Float_32 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<BoolTypes<Float_32>::Type>(mask));
	}

	const Float_32 select(const BoolTypes<Float_32>::Type mask, const Float_32 onTrue, const Float_32 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<BoolTypes<Float_32>::Type>() : Zero<BoolTypes<Float_32>::Type>());
	}

	const Float_64 select(const bool mask, const Float_64 onTrue, const Float_64 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, boolMask<BoolTypes<Float_64>::Type>(mask));
	}

	const Float_64 select(const BoolTypes<Float_64>::Type mask, const Float_64 onTrue, const Float_64 onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, mask ? MaskAll<BoolTypes<Float_64>::Type>() : Zero<BoolTypes<Float_64>::Type>());
	}

	const PackedInts_128 select(const bool mask, const PackedInts_128 & onTrue, const PackedInts_128 & onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, _mm_set1_epi32(boolMask<int>(mask)));
	}

	const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, _mm_castsi128_ps(_mm_set1_epi32(boolMask<int>(mask))));
	}

	const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept
	{
		// reverse onTrue & onFalse since we test for mask components to be zero
		return blendMasked(onTrue, onFalse, _mm_cmpeq_ps(mask, Zero<PackedFloat4_128>()));
	}

	const PackedFloat2_128 select(const bool mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept
	{
		return blendMasked(onFalse, onTrue, _mm_castsi128_pd(_mm_set1_epi32(boolMask<int>(mask))));
	}

	const PackedFloat2_128 select(const PackedFloat2_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept
	{
		// reverse onTrue & onFalse since we test for mask components to be zero
		return blendMasked(onTrue, onFalse, _mm_cmpeq_pd(mask, Zero<PackedFloat2_128>()));
	}

}
