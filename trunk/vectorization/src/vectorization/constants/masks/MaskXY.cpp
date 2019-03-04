#include "vectorization/constants/masks/MaskXY.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 MaskXY<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::Y * sizeof(Float_32)));
	}

	template <>
	const PackedFloat2_128 MaskXY<PackedFloat2_128>() noexcept
	{
		return MaskAll<PackedFloat2_128>();
	}

}
