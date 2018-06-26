#include "vectorization/conversions/native.h"

#include "vectorization/constants/values/Zero.h"

namespace vectorization
{

	template <>
	const UInt_32 convert<UInt_32>(const Int_32 v) noexcept
	{
		return static_cast<UInt_32>(v);
	}

	template <>
	const Int_64 convert<Int_64>(const Int_32 v) noexcept
	{
		return static_cast<Int_64>(v);
	}

	template <>
	const UInt_64 convert<UInt_64>(const Int_32 v) noexcept
	{
		return static_cast<UInt_64>(v);
	}

	template <>
	const Float_32 convert<Float_32>(const Int_32 v) noexcept
	{
		return _mm_cvtss_f32(_mm_cvt_si2ss(Zero<PackedFloat4_128>(), v));
	}

	template <>
	const Float_64 convert<Float_64>(const Int_32 v) noexcept
	{
		return _mm_cvtsd_f64(_mm_cvtsi32_sd(Zero<PackedFloat2_128>(), v));
	}



	template <>
	const Int_32 convert<Int_32>(const UInt_32 v) noexcept
	{
		return static_cast<Int_32>(v);
	}

	template <>
	const Int_64 convert<Int_64>(const UInt_32 v) noexcept
	{
		return static_cast<Int_64>(v);
	}

	template <>
	const UInt_64 convert<UInt_64>(const UInt_32 v) noexcept
	{
		return static_cast<UInt_64>(v);
	}

	template <>
	const Float_32 convert<Float_32>(const UInt_32 v) noexcept
	{
		return _mm_cvtss_f32(_mm_cvtepu32_ps(_mm_set1_epi32(static_cast<Int_32>(v))));
	}

	template <>
	const Float_64 convert<Float_64>(const UInt_32 v) noexcept
	{
		return static_cast<Float_64>(v);
	}



	template <>
	const Int_32 convert<Int_32>(const Int_64 v) noexcept
	{
		return static_cast<Int_32>(v);
	}

	template <>
	const UInt_32 convert<UInt_32>(const Int_64 v) noexcept
	{
		return static_cast<UInt_32>(v);
	}

	template <>
	const UInt_64 convert<UInt_64>(const Int_64 v) noexcept
	{
		return static_cast<UInt_64>(v);
	}

	template <>
	const Float_32 convert<Float_32>(const Int_64 v) noexcept
	{
#ifdef ARCH_X64
		return _mm_cvtss_f32(_mm_cvtsi64_ss(Zero<PackedFloat4_128>(), v));
#else
		// let the compiler decide because "_mm_cvtsi64_ss" is not available on 32 bit x86
		return static_cast<Float_32>(v);
#endif
	}

	template <>
	const Float_64 convert<Float_64>(const Int_64 v) noexcept
	{
#ifdef ARCH_X64
		return _mm_cvtsd_f64(_mm_cvtsi64_sd(Zero<PackedFloat2_128>(), v));
#else
		// let the compiler decide because "_mm_cvtsi64_sd" is not available on 32 bit x86
		return static_cast<Float_64>(v);
#endif
	}



	template <>
	const Int_32 convert<Int_32>(const UInt_64 v) noexcept
	{
		return static_cast<Int_32>(v);
	}

	template <>
	const UInt_32 convert<UInt_32>(const UInt_64 v) noexcept
	{
		return static_cast<UInt_32>(v);
	}

	template <>
	const Int_64 convert<Int_64>(const UInt_64 v) noexcept
	{
		return static_cast<Int_64>(v);
	}

	template <>
	const Float_32 convert<Float_32>(const UInt_64 v) noexcept
	{
		return static_cast<Float_32>(v);
	}

	template <>
	const Float_64 convert<Float_64>(const UInt_64 v) noexcept
	{
		return static_cast<Float_64>(v);
	}



	template <>
	const Int_32 convert<Int_32>(const Float_32 v) noexcept
	{
		return _mm_cvtss_si32(_mm_set_ss(v));
	}

	template <>
	const UInt_32 convert<UInt_32>(const Float_32 v) noexcept
	{
		return static_cast<UInt_32>(v);
	}

	template <>
	const Int_64 convert<Int_64>(const Float_32 v) noexcept
	{
#ifdef ARCH_X64
		return _mm_cvtss_si64(_mm_set_ss(v));
#else
		// let the compiler decide because "_mm_cvtss_si64" is not available on 32 bit x86
		return static_cast<Int_64>(v);
#endif
	}

	template <>
	const UInt_64 convert<UInt_64>(const Float_32 v) noexcept
	{
		return static_cast<UInt_64>(v);
	}

	template <>
	const Float_64 convert<Float_64>(const Float_32 v) noexcept
	{
		return static_cast<Float_64>(v);
	}



	template <>
	const Int_32 convert<Int_32>(const Float_64 v) noexcept
	{
		return _mm_cvtsd_si32(_mm_set_sd(v));
	}

	template <>
	const UInt_32 convert<UInt_32>(const Float_64 v) noexcept
	{
		return static_cast<UInt_32>(v);
	}

	template <>
	const Int_64 convert<Int_64>(const Float_64 v) noexcept
	{
#ifdef ARCH_X64
		return _mm_cvtsd_si64(_mm_set_sd(v));
#else
		// let the compiler decide because "_mm_cvtsd_si64" is not available on 32 bit x86
		return static_cast<Int_64>(v);
#endif
	}

	template <>
	const UInt_64 convert<UInt_64>(const Float_64 v) noexcept
	{
		return static_cast<UInt_64>(v);
	}

	template <>
	const Float_32 convert<Float_32>(const Float_64 v) noexcept
	{
		return static_cast<Float_32>(v);
	}

}
