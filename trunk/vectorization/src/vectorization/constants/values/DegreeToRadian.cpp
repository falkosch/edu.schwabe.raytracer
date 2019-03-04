#include "vectorization/constants/values/DegreeToRadian.h"

#include "vectorization/constants/values/Pi.h"

namespace vectorization
{

#define DEGREE_TO_RADIAN C_PI / 180.0

	const PackedFloat4_128 DegreeToRadian_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(DEGREE_TO_RADIAN));
	const PackedFloat2_128 DegreeToRadian_PackedFloat2_128 = _mm_set1_pd(DEGREE_TO_RADIAN);

	template <>
	const PackedFloat4_128 DegreeToRadian<PackedFloat4_128>() noexcept
	{
		return DegreeToRadian_PackedFloat4_128;
	}

	template <>
	const PackedFloat2_128 DegreeToRadian<PackedFloat2_128>() noexcept
	{
		return DegreeToRadian_PackedFloat2_128;
	}

	template <>
	const Float_32 DegreeToRadian<Float_32>() noexcept
	{
		return _mm_cvtss_f32(DegreeToRadian<PackedFloat4_128>());
	}

	template <>
	const Float_64 DegreeToRadian<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(DegreeToRadian<PackedFloat2_128>());
	}

}
