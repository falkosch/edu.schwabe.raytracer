#pragma once

#include "type.h"

namespace vectorization
{

	//template <typename U, ASizeT Size, typename T>
	//const U convert(const UTVector<Size, T> & v) noexcept
	//{
	//	typedef typename U::ValueType OT;
	//	U t;
	//	const ASizeT m = min(U::SIZE, Size);
	//	for (ASizeT i = VectorIndices::X; i < m; ++i) {
	//		t[i] = v[i];
	//	}
	//	for (ASizeT i = m; i < U::SIZE; ++i) {
	//		t[i] = OT();
	//	}
	//	return t;
	//}



	template <typename T>
	const T convert(const UTVector<VectorSizes::Y, Int_32> & v) noexcept;

	template <>
	const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const UTVector<VectorSizes::Y, Int_32> & v) noexcept;

}
