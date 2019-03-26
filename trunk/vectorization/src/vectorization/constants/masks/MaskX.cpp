#include "vectorization/constants/masks/MaskX.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 MaskX<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::Z * sizeof(Float_32)));
	}

	template <>
	const PackedFloat2_128 MaskX<PackedFloat2_128>() noexcept
	{
		return _mm_castsi128_pd(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_64)));
	}

}
