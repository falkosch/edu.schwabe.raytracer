#include "vectorization/constants/values/Half.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 Half<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 26), 2));
	}

	template <>
	const PackedFloat2_128 Half<PackedFloat2_128>() noexcept
	{
		return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 55), 2));
	}

	template <>
	const Float_32 Half<Float_32>() noexcept
	{
		return _mm_cvtss_f32(Half<PackedFloat4_128>());
	}

	template <>
	const Float_64 Half<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(Half<PackedFloat2_128>());
	}

}
