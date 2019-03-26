#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const bool max(const bool a, const bool b) noexcept;

	const long max(const long a, const long b) noexcept;

	const unsigned long max(const unsigned long a, const unsigned long b) noexcept;

	const Int_8 max(const Int_8 a, const Int_8 b) noexcept;

	const UInt_8 max(const UInt_8 a, const UInt_8 b) noexcept;

	const Int_16 max(const Int_16 a, const Int_16 b) noexcept;

	const UInt_16 max(const UInt_16 a, const UInt_16 b) noexcept;

	const Int_32 max(const Int_32 a, const Int_32 b) noexcept;

	const UInt_32 max(const UInt_32 a, const UInt_32 b) noexcept;

	const Int_64 max(const Int_64 a, const Int_64 b) noexcept;

	const UInt_64 max(const UInt_64 a, const UInt_64 b) noexcept;

	const Float_32 max(const Float_32 a, const Float_32 b) noexcept;

	const Float_64 max(const Float_64 a, const Float_64 b) noexcept;

	const PackedFloat4_128 max(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

	const PackedFloat2_128 max(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

	const PackedFloat8_256 max(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 max(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;

}
