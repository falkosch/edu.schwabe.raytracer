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

#define BLEND_MASKED(type) const type blendMasked(const type onBitNotSet, const type onBitSet, const type mask) noexcept
    BLEND_MASKED(bool);
    BLEND_MASKED(long);
    BLEND_MASKED(unsigned long);
    BLEND_MASKED(Int_8);
    BLEND_MASKED(UInt_8);
    BLEND_MASKED(Int_16);
    BLEND_MASKED(UInt_16);
    BLEND_MASKED(Int_32);
    BLEND_MASKED(UInt_32);
    BLEND_MASKED(Int_64);
    BLEND_MASKED(UInt_64);
#undef BLEND_MASKED

    const Float_32 blendMasked(const Float_32 onBitNotSet, const Float_32 onBitSet, const BoolTypes<Float_32>::Type mask) noexcept;

    const Float_64 blendMasked(const Float_64 onBitNotSet, const Float_64 onBitSet, const BoolTypes<Float_64>::Type mask) noexcept;

    const PackedInts_128 blendMasked(const PackedInts_128 & onBitNotSet, const PackedInts_128 & onBitSet, const PackedInts_128 & mask) noexcept;

    const PackedFloat4_128 blendMasked(const PackedFloat4_128 & onBitNotSet, const PackedFloat4_128 & onBitSet, const PackedFloat4_128 & mask) noexcept;

    const PackedFloat2_128 blendMasked(const PackedFloat2_128 & onBitNotSet, const PackedFloat2_128 & onBitSet, const PackedFloat2_128 & mask) noexcept;

    //}
#pragma endregion

#pragma region select()
    //{ select()

#define SELECT(type) const type select(const type mask, const type onTrue, const type onFalse) noexcept
    SELECT(bool);
    SELECT(long);
    SELECT(unsigned long);
    SELECT(Int_8);
    SELECT(UInt_8);
    SELECT(Int_16);
    SELECT(UInt_16);
    SELECT(Int_32);
    SELECT(UInt_32);
    SELECT(Int_64);
    SELECT(UInt_64);
#undef SELECT

#define BOOLEAN_SELECT(type) const type select(const bool mask, const type onTrue, const type onFalse) noexcept
    BOOLEAN_SELECT(long);
    BOOLEAN_SELECT(unsigned long);
    BOOLEAN_SELECT(Int_8);
    BOOLEAN_SELECT(UInt_8);
    BOOLEAN_SELECT(Int_16);
    BOOLEAN_SELECT(UInt_16);
    BOOLEAN_SELECT(Int_32);
    BOOLEAN_SELECT(UInt_32);
    BOOLEAN_SELECT(Int_64);
    BOOLEAN_SELECT(UInt_64);
#undef BOOLEAN_SELECT

    const Float_32 select(const bool mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;

    const Float_32 select(const BoolTypes<Float_32>::Type mask, const Float_32 onTrue, const Float_32 onFalse) noexcept;

    const Float_64 select(const bool mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;

    const Float_64 select(const BoolTypes<Float_64>::Type mask, const Float_64 onTrue, const Float_64 onFalse) noexcept;

    const PackedInts_128 select(const bool mask, const PackedInts_128 & onTrue, const PackedInts_128 & onFalse) noexcept;

    const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;

    const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;

    const PackedFloat2_128 select(const bool mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;

    const PackedFloat2_128 select(const PackedFloat2_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;

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
