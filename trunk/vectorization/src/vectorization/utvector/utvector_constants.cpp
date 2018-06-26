#include "vectorization/utvector/constants.h"

namespace vectorization
{

	template <>
	const UTVector<VectorSizes::Y, UInt_32> Zero<UTVector<VectorSizes::Y, UInt_32>>() noexcept
	{
		return UTVector<VectorSizes::Y, UInt_32>(Zero<UInt_32>());
	}

	template <>
	const UTVector<VectorSizes::Y, UInt_32> One<UTVector<VectorSizes::Y, UInt_32>>() noexcept
	{
		return UTVector<VectorSizes::Y, UInt_32>(One<UInt_32>());
	}

}
