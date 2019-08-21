#pragma once

#include "../../architecture.h"

namespace vectorization
{

	// Convenience function for MaskAll. Return is equivalent to 0.
	template <typename V>
	const V MaskNone() noexcept;

	template <>
	const bool MaskNone<bool>() noexcept;

	template <>
	const long MaskNone<long>() noexcept;

	template <>
	const unsigned long MaskNone<unsigned long>() noexcept;

	template <>
	const Int_8 MaskNone<Int_8>() noexcept;

	template <>
	const UInt_8 MaskNone<UInt_8>() noexcept;

	template <>
	const Int_16 MaskNone<Int_16>() noexcept;

	template <>
	const UInt_16 MaskNone<UInt_16>() noexcept;

	template <>
	const Int_32 MaskNone<Int_32>() noexcept;

	template <>
	const UInt_32 MaskNone<UInt_32>() noexcept;

	template <>
	const Int_64 MaskNone<Int_64>() noexcept;

	template <>
	const UInt_64 MaskNone<UInt_64>() noexcept;

	template <>
	const PackedInts_128 MaskNone<PackedInts_128>() noexcept;

	template <>
	const PackedFloat4_128 MaskNone<PackedFloat4_128>() noexcept;

	template <>
	const PackedFloat2_128 MaskNone<PackedFloat2_128>() noexcept;

}
