#include "vectorization/constants/masks/MaskYZW.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 MaskYZW<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_32)));
	}

}
