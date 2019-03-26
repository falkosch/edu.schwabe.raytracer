#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const bool clamp(const bool v, const bool lower, const bool upper) noexcept;

	const long clamp(const long v, const long lower, const long upper) noexcept;

	const unsigned long clamp(const unsigned long v, const unsigned long lower, const unsigned long upper) noexcept;

	const Int_8 clamp(const Int_8 v, const Int_8 lower, const Int_8 upper) noexcept;

	const UInt_8 clamp(const UInt_8 v, const UInt_8 lower, const UInt_8 upper) noexcept;

	const Int_16 clamp(const Int_16 v, const Int_16 lower, const Int_16 upper) noexcept;

	const UInt_16 clamp(const UInt_16 v, const UInt_16 lower, const UInt_16 upper) noexcept;

	const Int_32 clamp(const Int_32 v, const Int_32 lower, const Int_32 upper) noexcept;

	const UInt_32 clamp(const UInt_32 v, const UInt_32 lower, const UInt_32 upper) noexcept;

	const Int_64 clamp(const Int_64 v, const Int_64 lower, const Int_64 upper) noexcept;

	const UInt_64 clamp(const UInt_64 v, const UInt_64 lower, const UInt_64 upper) noexcept;

	const Float_32 clamp(const Float_32 v, const Float_32 lower, const Float_32 upper) noexcept;

	const Float_64 clamp(const Float_64 v, const Float_64 lower, const Float_64 upper) noexcept;

	const PackedFloat4_128 clamp(const PackedFloat4_128 & v, const PackedFloat4_128 & lower, const PackedFloat4_128 & upper) noexcept;

	const PackedFloat2_128 clamp(const PackedFloat2_128 & v, const PackedFloat2_128 & lower, const PackedFloat2_128 & upper) noexcept;

	const PackedFloat8_256 clamp(const PackedFloat8_256 & v, const PackedFloat8_256 & lower, const PackedFloat8_256 & upper) noexcept;

	const PackedFloat4_256 clamp(const PackedFloat4_256 & v, const PackedFloat4_256 & lower, const PackedFloat4_256 & upper) noexcept;

}
