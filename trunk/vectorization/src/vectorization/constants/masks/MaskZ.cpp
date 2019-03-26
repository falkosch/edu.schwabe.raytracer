#include "vectorization/constants/masks/MaskZ.h"

#include "vectorization/constants/masks/MaskW.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 MaskZ<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(
			_mm_srli_si128(
				_mm_castps_si128(MaskW<PackedFloat4_128>()),
				VectorSizes::X * sizeof(Float_32)
			)
		);
	}

}
