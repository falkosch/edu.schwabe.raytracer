#include "vectorization/constants/masks/MaskAll.h"

#include "vectorization/constants/values/Zero.h"

namespace vectorization
{

	template <>
	const bool MaskAll<bool>() noexcept
	{
		return static_cast<bool>(-1);
	}

	template <>
	const long MaskAll<long>() noexcept
	{
		return { -1 };
	}

	template <>
	const unsigned long MaskAll<unsigned long>() noexcept
	{
		return static_cast<unsigned long>(-1);
	}

	template <>
	const Int_8 MaskAll<Int_8>() noexcept
	{
		return { -1 };
	}

	template <>
	const UInt_8 MaskAll<UInt_8>() noexcept
	{
		return static_cast<UInt_8>(-1);
	}

	template <>
	const Int_16 MaskAll<Int_16>() noexcept
	{
		return { -1 };
	}

	template <>
	const UInt_16 MaskAll<UInt_16>() noexcept
	{
		return static_cast<UInt_16>(-1);
	}

	template <>
	const Int_32 MaskAll<Int_32>() noexcept
	{
		return { -1 };
	}

	template <>
	const UInt_32 MaskAll<UInt_32>() noexcept
	{
		return static_cast<UInt_32>(-1);
	}

	template <>
	const Int_64 MaskAll<Int_64>() noexcept
	{
		return { -1 };
	}

	template <>
	const UInt_64 MaskAll<UInt_64>() noexcept
	{
		return static_cast<UInt_64>(-1);
	}

	template <>
	const PackedInts_128 MaskAll<PackedInts_128>() noexcept
	{
		return _mm_cmpeq_epi32(Zero<PackedInts_128>(), Zero<PackedInts_128>());
	}

	template <>
	const PackedFloat4_128 MaskAll<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(MaskAll<PackedInts_128>());
	}

	template <>
	const PackedFloat2_128 MaskAll<PackedFloat2_128>() noexcept
	{
		return _mm_castsi128_pd(MaskAll<PackedInts_128>());
	}

}
