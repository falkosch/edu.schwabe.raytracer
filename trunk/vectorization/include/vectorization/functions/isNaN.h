#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	// Checks whether v is a NaN value
	const bool isNaN(const Float_32 v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const bool isNaN(const Float_32 a, const Float_32 b) noexcept;

	const bool isNaN(const Float_64 v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const bool isNaN(const Float_64 a, const Float_64 b) noexcept;

	const PackedFloat4_128 isNaN(const PackedFloat4_128 & v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const PackedFloat4_128 isNaN(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

	const PackedFloat2_128 isNaN(const PackedFloat2_128 & v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const PackedFloat2_128 isNaN(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

	const PackedFloat8_256 isNaN(const PackedFloat8_256 & v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const PackedFloat8_256 isNaN(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 isNaN(const PackedFloat4_256 & v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const PackedFloat4_256 isNaN(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;

}
