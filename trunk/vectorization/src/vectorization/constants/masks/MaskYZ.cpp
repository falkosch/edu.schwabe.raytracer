#include "vectorization/constants/masks/MaskYZ.h"

#include "vectorization/constants/masks/MaskXY.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 MaskYZ<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(
			_mm_slli_si128(
				_mm_castps_si128(MaskXY<PackedFloat4_128>()),
				VectorSizes::X * sizeof(Float_32)
			)
		);
	}

}
