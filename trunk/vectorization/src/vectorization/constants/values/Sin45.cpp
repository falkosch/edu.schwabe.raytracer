#include "vectorization/constants/values/Sin45.h"

#define SIN_45 0.70710678118654752440084436210485

namespace vectorization
{

	const PackedFloat4_128 Sin45_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(SIN_45));
	const PackedFloat2_128 Sin45_PackedFloat2_128 = _mm_set1_pd(SIN_45);

	template <>
	const PackedFloat4_128 Sin45<PackedFloat4_128>() noexcept
	{
		return Sin45_PackedFloat4_128;
	}

	template <>
	const PackedFloat2_128 Sin45<PackedFloat2_128>() noexcept
	{
		return Sin45_PackedFloat2_128;
	}

	template <>
	const Float_32 Sin45<Float_32>() noexcept
	{
		return _mm_cvtss_f32(Sin45<PackedFloat4_128>());
	}

	template <>
	const Float_64 Sin45<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(Sin45<PackedFloat2_128>());
	}

}
