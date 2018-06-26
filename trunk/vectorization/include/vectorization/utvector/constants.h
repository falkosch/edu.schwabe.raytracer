#pragma once

#include "type.h"

#include "../constants/values/One.h"
#include "../constants/values/Zero.h"

namespace vectorization
{

	template <>
	const UTVector<VectorSizes::Y, UInt_32> Zero<UTVector<VectorSizes::Y, UInt_32>>() noexcept;

	template <>
	const UTVector<VectorSizes::Y, UInt_32> One<UTVector<VectorSizes::Y, UInt_32>>() noexcept;

}
