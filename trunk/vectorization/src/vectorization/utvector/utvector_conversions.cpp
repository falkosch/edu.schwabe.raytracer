#include "vectorization/utvector/conversions.h"

#include "vectorization/conversions/native.h"

namespace vectorization
{

	template <>
	const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const UTVector<VectorSizes::Y, Int_32> & v) noexcept
	{
		return UTVector<VectorSizes::Y, UInt_32>(convert<UInt_32>(x(v)), convert<UInt_32>(y(v)));
	}

	template <>
	const UTVector<VectorSizes::Y, Float_32> convert<UTVector<VectorSizes::Y, Float_32>>(const UTVector<VectorSizes::Y, Int_32> & v) noexcept
	{
		return UTVector<VectorSizes::Y, Float_32>(convert<Float_32>(x(v)), convert<Float_32>(y(v)));
	}

}
