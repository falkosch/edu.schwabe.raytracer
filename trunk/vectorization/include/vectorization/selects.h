/*
* Branchless blending and selecting for primitive types.
*
* @file vectorization/selects.h
*/

#pragma once

#include "constants.h"

#include <type_traits>

namespace vectorization
{

#pragma region boolMask()
	//{ boolMask

	template <typename T>
	inline constexpr T boolMask(const bool mask) noexcept
	{
		static_assert(std::is_integral<T>::value, "boolMask<T> must be an integral type");
		return static_cast<T>(-static_cast<typename std::make_signed<T>::type>(mask));
	}

	//}
#pragma endregion

#pragma region blendMasked()
	//{ blendMasked()

	const bool blendMasked(const bool onBitNotSet, const bool onBitSet, const bool mask) noexcept;

	const long blendMasked(const long onBitNotSet, const long onBitSet, const long mask) noexcept;
	const unsigned long blendMasked(const unsigned long onBitNotSet, const unsigned long onBitSet, const unsigned long mask) noexcept;

	const Int_8 blendMasked(const Int_8 onBitNotSet, const Int_8 onBitSet, const Int_8 mask) noexcept;
	const UInt_8 blendMasked(const UInt_8 onBitNotSet, const UInt_8 onBitSet, const UInt_8 mask) noexcept;

	const Int_16 blendMasked(const Int_16 onBitNotSet, const Int_16 onBitSet, const Int_16 mask) noexcept;
	const UInt_16 blendMasked(const UInt_16 onBitNotSet, const UInt_16 onBitSet, const UInt_16 mask) noexcept;

	const Int_32 blendMasked(const Int_32 onBitNotSet, const Int_32 onBitSet, const Int_32 mask) noexcept;
	const UInt_32 blendMasked(const UInt_32 onBitNotSet, const UInt_32 onBitSet, const UInt_32 mask) noexcept;

	const Int_64 blendMasked(const Int_64 onBitNotSet, const Int_64 onBitSet, const Int_64 mask) noexcept;
	const UInt_64 blendMasked(const UInt_64 onBitNotSet, const UInt_64 onBitSet, const UInt_64 mask) noexcept;

	const Float_32 blendMasked(const Float_32 onBitNotSet, const Float_32 onBitSet, const BoolTypes<Float_32>::Type mask) noexcept;
	const Float_64 blendMasked(const Float_64 onBitNotSet, const Float_64 onBitSet, const BoolTypes<Float_64>::Type mask) noexcept;

	const PackedInts_128 blendMasked(const PackedInts_128 & onBitNotSet, const PackedInts_128 & onBitSet, const PackedInts_128 & mask) noexcept;
	const PackedFloat4_128 blendMasked(const PackedFloat4_128 & onBitNotSet, const PackedFloat4_128 & onBitSet, const PackedFloat4_128 & mask) noexcept;
	const PackedFloat2_128 blendMasked(const PackedFloat2_128 & onBitNotSet, const PackedFloat2_128 & onBitSet, const PackedFloat2_128 & mask) noexcept;

	//}
#pragma endregion

#pragma region select()
	//{ select()

	const bool select(const bool mask, const bool onTrue, const bool onFalse) noexcept;

	const long select(const long mask, const long onTrue, const long onFalse) noexcept;
	const unsigned long select(const unsigned long mask, const unsigned long onTrue, const unsigned long onFalse) noexcept;

	const Int_8 select(const Int_8 mask, const Int_8 onTrue, const Int_8 onFalse) noexcept;
	const UInt_8 select(const UInt_8 mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept;

	const Int_16 select(const Int_16 mask, const Int_16 onTrue, const Int_16 onFalse) noexcept;
	const UInt_16 select(const UInt_16 mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept;

	const Int_32 select(const Int_32 mask, const Int_32 onTrue, const Int_32 onFalse) noexcept;
	const UInt_32 select(const UInt_32 mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept;

	const Int_64 select(const Int_64 mask, const Int_64 onTrue, const Int_64 onFalse) noexcept;
	const UInt_64 select(const UInt_64 mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept;

	const Float_32 select(const BoolTypes<Float_32>::Type mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;
	const Float_64 select(const BoolTypes<Float_64>::Type mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;

	const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;
	const PackedFloat2_128 select(const PackedFloat2_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;


	const long select(const bool mask, const long onTrue, const long onFalse) noexcept;
	const unsigned long select(const bool mask, const unsigned long onTrue, const unsigned long onFalse) noexcept;

	const Int_8 select(const bool mask, const Int_8 onTrue, const Int_8 onFalse) noexcept;
	const UInt_8 select(const bool mask, const UInt_8 onTrue, const UInt_8 onFalse) noexcept;

	const Int_16 select(const bool mask, const Int_16 onTrue, const Int_16 onFalse) noexcept;
	const UInt_16 select(const bool mask, const UInt_16 onTrue, const UInt_16 onFalse) noexcept;

	const Int_32 select(const bool mask, const Int_32 onTrue, const Int_32 onFalse) noexcept;
	const UInt_32 select(const bool mask, const UInt_32 onTrue, const UInt_32 onFalse) noexcept;

	const Int_64 select(const bool mask, const Int_64 onTrue, const Int_64 onFalse) noexcept;
	const UInt_64 select(const bool mask, const UInt_64 onTrue, const UInt_64 onFalse) noexcept;

	const Float_32 select(const bool mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;
	const Float_64 select(const bool mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;

	const PackedInts_128 select(const bool mask, const PackedInts_128 & onTrue, const PackedInts_128 & onFalse) noexcept;
	const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;
	const PackedFloat2_128 select(const bool mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;


	template <typename T>
	inline constexpr T select(const bool mask, const T & onTrue, const T & onFalse) noexcept
	{
		return *(mask ? &onTrue : &onFalse);
	}

	template <typename T, bool B>
	struct BoolSelector;

	template <typename T>
	struct BoolSelector < T, false >
	{
		inline constexpr T & operator()(const T & onTrue, const T & onFalse) noexcept
		{
			return onFalse;
		}
	};

	template <typename T>
	struct BoolSelector < T, true >
	{
		inline constexpr T & operator()(const T & onTrue, const T & onFalse) noexcept
		{
			return onTrue;
		}
	};

	//}
#pragma endregion

}
