#include "vectorization/constants/values/RadianToDegree.h"

#include "vectorization/constants/values/Pi.h"

#define RADIAN_TO_DEGREE 180.0 / C_PI

namespace vectorization
{

	const PackedFloat4_128 RadianToDegree_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(RADIAN_TO_DEGREE));
	const PackedFloat2_128 RadianToDegree_PackedFloat2_128 = _mm_set1_pd(RADIAN_TO_DEGREE);

	template <>
	const PackedFloat4_128 RadianToDegree<PackedFloat4_128>() noexcept
	{
		return RadianToDegree_PackedFloat4_128;
	}

	template <>
	const PackedFloat2_128 RadianToDegree<PackedFloat2_128>() noexcept
	{
		return RadianToDegree_PackedFloat2_128;
	}

	template <>
	const Float_32 RadianToDegree<Float_32>() noexcept
	{
		return _mm_cvtss_f32(RadianToDegree<PackedFloat4_128>());
	}

	template <>
	const Float_64 RadianToDegree<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(RadianToDegree<PackedFloat2_128>());
	}

}
