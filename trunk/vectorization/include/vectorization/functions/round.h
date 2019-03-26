#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const Float_32 round(const Float_32 v) noexcept;

	const Float_64 round(const Float_64 v) noexcept;

	const PackedFloat4_128 round(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 round(const PackedFloat2_128 & v) noexcept;

	const PackedFloat8_256 round(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 round(const PackedFloat4_256 & v) noexcept;

}
