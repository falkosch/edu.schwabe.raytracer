#include "vectorization/constants/values/Zero.h"

namespace vectorization
{

	template <typename T>
	const T Zero() noexcept
	{
		return T();
	}

	template const bool Zero() noexcept;
	template const long Zero<long>() noexcept;
	template const unsigned long Zero<unsigned long>() noexcept;
	template const Int_8 Zero<Int_8>() noexcept;
	template const UInt_8 Zero<UInt_8>() noexcept;
	template const Int_16 Zero<Int_16>() noexcept;
	template const UInt_16 Zero<UInt_16>() noexcept;
	template const Int_32 Zero<Int_32>() noexcept;
	template const UInt_32 Zero<UInt_32>() noexcept;
	template const Int_64 Zero<Int_64>() noexcept;
	template const UInt_64 Zero<UInt_64>() noexcept;

	template <>
	const PackedInts_128 Zero<PackedInts_128>() noexcept
	{
		return _mm_setzero_si128();
	}

	template <>
	const PackedFloat4_128 Zero<PackedFloat4_128>() noexcept
	{
		return _mm_setzero_ps();
	}

	template <>
	const PackedFloat2_128 Zero<PackedFloat2_128>() noexcept
	{
		return _mm_setzero_pd();
	}

	template <>
	const Float_32 Zero<Float_32>() noexcept
	{
		return _mm_cvtss_f32(Zero<PackedFloat4_128>());
	}

	template <>
	const Float_64 Zero<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(Zero<PackedFloat2_128>());
	}

}
