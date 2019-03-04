#include "vectorization/constants/values/OneZ.h"

#include "vectorization/constants/values/One.h"

namespace vectorization
{

	template <>
	const PackedFloat4_128 OneZ<PackedFloat4_128>() noexcept
	{
		return _mm_castsi128_ps(
			_mm_srli_si128(
				_mm_slli_si128(
					_mm_castps_si128(One<PackedFloat4_128>()),
					VectorSizes::Z * sizeof(Float_32)
				),
				VectorSizes::X * sizeof(Float_32)
			)
		);
	}

}