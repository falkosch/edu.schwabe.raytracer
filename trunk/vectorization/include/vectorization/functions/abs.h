#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const long abs(const long v) noexcept;

	const Int_8 abs(const Int_8 v) noexcept;

	const Int_16 abs(const Int_16 v) noexcept;

	const Int_32 abs(const Int_32 v) noexcept;

	const Int_64 abs(const Int_64 v) noexcept;

	const Float_32 abs(const Float_32 v) noexcept;

	const Float_64 abs(const Float_64 v) noexcept;

	const PackedFloat4_128 abs(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 abs(const PackedFloat2_128 & v) noexcept;

	const PackedFloat8_256 abs(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 abs(const PackedFloat4_256 & v) noexcept;

}
