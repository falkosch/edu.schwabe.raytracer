#include "vectorization/constants/values/ReciprocalPi.h"

#include "vectorization/constants/values/Pi.h"

#define RECIPROCAL_PI 1.0 / C_PI

namespace vectorization
{

	const PackedFloat4_128 ReciprocalPi_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(RECIPROCAL_PI));
	const PackedFloat2_128 ReciprocalPi_PackedFloat2_128 = _mm_set1_pd(RECIPROCAL_PI);

	template <>
	const PackedFloat4_128 ReciprocalPi<PackedFloat4_128>() noexcept
	{
		return ReciprocalPi_PackedFloat4_128;
	}

	template <>
	const PackedFloat2_128 ReciprocalPi<PackedFloat2_128>() noexcept
	{
		return ReciprocalPi_PackedFloat2_128;
	}

	template <>
	const Float_32 ReciprocalPi<Float_32>() noexcept
	{
		return _mm_cvtss_f32(ReciprocalPi<PackedFloat4_128>());
	}

	template <>
	const Float_64 ReciprocalPi<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(ReciprocalPi<PackedFloat2_128>());
	}

}
