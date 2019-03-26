#include "vectorization/constants/values/NegativeTwo.h"

#include "vectorization/constants/masks/MaskAll.h"

#include <limits>

namespace vectorization
{

	template <typename T>
	const T NegativeTwo() noexcept
	{
		return { -2 };
	}

	template const long NegativeTwo<long>() noexcept;
	template const Int_8 NegativeTwo<Int_8>() noexcept;
	template const Int_16 NegativeTwo<Int_16>() noexcept;
	template const Int_32 NegativeTwo<Int_32>() noexcept;
	template const Int_64 NegativeTwo<Int_64>() noexcept;

	template <>
	const PackedFloat4_128 NegativeTwo<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(
			_mm_slli_epi32(
				MaskAll<PackedInts_128>(),
				sizeof(Int_32) * std::numeric_limits<unsigned char>::digits - 2
			)
		);
	}

	template <>
	const PackedFloat2_128 NegativeTwo<PackedFloat2_128>() noexcept
	{
		return _mm_castsi128_pd(
			_mm_slli_epi64(
				MaskAll<PackedInts_128>(),
				sizeof(Int_64) * std::numeric_limits<unsigned char>::digits - 2)
		);
	}

	template <>
	const Float_32 NegativeTwo<Float_32>() noexcept
	{
		return _mm_cvtss_f32(NegativeTwo<PackedFloat4_128>());
	}

	template <>
	const Float_64 NegativeTwo<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(NegativeTwo<PackedFloat2_128>());
	}

}
