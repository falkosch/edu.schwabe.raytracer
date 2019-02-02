#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const Float_32 fract(const Float_32 v) noexcept;

	const Float_64 fract(const Float_64 v) noexcept;

	const PackedFloat4_128 fract(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 fract(const PackedFloat2_128 & v) noexcept;

	const PackedFloat8_256 fract(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 fract(const PackedFloat4_256 & v) noexcept;

}
