#include "vectorization/constants/values/One.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{

	template <typename T>
	const T One() noexcept
	{
		return { 1 };
	}

	template const bool One<bool>() noexcept;
	template const long One<long>() noexcept;
	template const unsigned long One<unsigned long>() noexcept;
	template const Int_8 One<Int_8>() noexcept;
	template const UInt_8 One<UInt_8>() noexcept;
	template const Int_16 One<Int_16>() noexcept;
	template const UInt_16 One<UInt_16>() noexcept;
	template const Int_32 One<Int_32>() noexcept;
	template const UInt_32 One<UInt_32>() noexcept;
	template const Int_64 One<Int_64>() noexcept;
	template const UInt_64 One<UInt_64>() noexcept;

	template <>
	const PackedFloat4_128 One<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(
			_mm_srli_epi32(
				_mm_slli_epi32(MaskAll<PackedInts_128>(),
					25
				),
				2
			)
		);
	}

	template <>
	const PackedFloat2_128 One<PackedFloat2_128>() noexcept
	{
		return _mm_castsi128_pd(
			_mm_srli_epi64(
				_mm_slli_epi64(
					MaskAll<PackedInts_128>(),
					54
				),
				2
			)
		);
	}

	template <>
	const Float_32 One<Float_32>() noexcept
	{
		return _mm_cvtss_f32(One<PackedFloat4_128>());
	}

	template <>
	const Float_64 One<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(One<PackedFloat2_128>());
	}

}
