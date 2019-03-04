#include "vectorization/constants/values/Two.h"

#include "vectorization/constants/masks/MaskAll.h"

#include <limits>

namespace vectorization
{

	template <typename T>
	const T Two() noexcept
	{
		return { 2 };
	}

	template const long Two<long>() noexcept;
	template const unsigned long Two<unsigned long>() noexcept;
	template const Int_8 Two<Int_8>() noexcept;
	template const UInt_8 Two<UInt_8>() noexcept;
	template const Int_16 Two<Int_16>() noexcept;
	template const UInt_16 Two<UInt_16>() noexcept;
	template const Int_32 Two<Int_32>() noexcept;
	template const UInt_32 Two<UInt_32>() noexcept;
	template const Int_64 Two<Int_64>() noexcept;
	template const UInt_64 Two<UInt_64>() noexcept;

	template <>
	const PackedFloat4_128 Two<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(
			_mm_srli_epi32(
				_mm_slli_epi32(
					MaskAll<PackedInts_128>(),
					sizeof(Int_32) * std::numeric_limits<unsigned char>::digits - 1
				),
				1
			)
		);
	}

	template <>
	const PackedFloat2_128 Two<PackedFloat2_128>() noexcept
	{
		return _mm_castsi128_pd(
			_mm_srli_epi64(
				_mm_slli_epi64(
					MaskAll<PackedInts_128>(),
					sizeof(Int_64) * std::numeric_limits<unsigned char>::digits - 1
				),
				1
			)
		);
	}

	template <>
	const Float_32 Two<Float_32>() noexcept
	{
		return _mm_cvtss_f32(Two<PackedFloat4_128>());
	}

	template <>
	const Float_64 Two<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(Two<PackedFloat2_128>());
	}

}
