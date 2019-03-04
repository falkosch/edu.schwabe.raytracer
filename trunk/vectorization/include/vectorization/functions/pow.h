#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const Float_32 pow(const Float_32 base, const Float_32 exponent) noexcept;

	const Float_64 pow(const Float_64 base, const Float_64 exponent) noexcept;

}