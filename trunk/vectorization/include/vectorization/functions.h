/*
* Introduces a centric helper class for general math functions.
*
* @file vectorization/functions.h
*/

#pragma once

#include "selects.h"

namespace vectorization
{

#pragma region min()
	//{ min()

	const bool min(const bool a, const bool b) noexcept;

	const long min(const long a, const long b) noexcept;

	const unsigned long min(const unsigned long a, const unsigned long b) noexcept;

	const Int_8 min(const Int_8 a, const Int_8 b) noexcept;

	const UInt_8 min(const UInt_8 a, const UInt_8 b) noexcept;

	const Int_16 min(const Int_16 a, const Int_16 b) noexcept;

	const UInt_16 min(const UInt_16 a, const UInt_16 b) noexcept;

	const Int_32 min(const Int_32 a, const Int_32 b) noexcept;

	const UInt_32 min(const UInt_32 a, const UInt_32 b) noexcept;

	const Int_64 min(const Int_64 a, const Int_64 b) noexcept;

	const UInt_64 min(const UInt_64 a, const UInt_64 b) noexcept;

	const Float_32 min(const Float_32 a, const Float_32 b) noexcept;

	const Float_64 min(const Float_64 a, const Float_64 b) noexcept;

	const PackedFloat4_128 min(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

	const PackedFloat2_128 min(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 min(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 min(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
#endif

	//}
#pragma endregion

#pragma region max()
	//{ max()

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

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 max(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 max(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
#endif

	//}
#pragma endregion

#pragma region clamp()
	//{ clamp()

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

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 clamp(const PackedFloat8_256 & v, const PackedFloat8_256 & lower, const PackedFloat8_256 & upper) noexcept;

	const PackedFloat4_256 clamp(const PackedFloat4_256 & v, const PackedFloat4_256 & lower, const PackedFloat4_256 & upper) noexcept;
#endif

	//}
#pragma endregion

#pragma region sign()
	//{ sign()

	const long sign(const long v) noexcept;

	const Int_8 sign(const Int_8 v) noexcept;

	const Int_16 sign(const Int_16 v) noexcept;

	const Int_32 sign(const Int_32 v) noexcept;

	const Int_64 sign(const Int_64 v) noexcept;

	const Float_32 sign(const Float_32 v) noexcept;

	const Float_64 sign(const Float_64 v) noexcept;

	const PackedFloat4_128 sign(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 sign(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 sign(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 sign(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region copysign()
	//{ copysign()

	const Float_32 copysign(const Float_32 v) noexcept;

	const Float_64 copysign(const Float_64 v) noexcept;

	const PackedFloat4_128 copysign(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 copysign(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 copysign(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 copysign(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region abs()
	//{ abs()

	const long abs(const long v) noexcept;

	const Int_8 abs(const Int_8 v) noexcept;

	const Int_16 abs(const Int_16 v) noexcept;

	const Int_32 abs(const Int_32 v) noexcept;

	const Int_64 abs(const Int_64 v) noexcept;

	const Float_32 abs(const Float_32 v) noexcept;

	const Float_64 abs(const Float_64 v) noexcept;

	const PackedFloat4_128 abs(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 abs(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 abs(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 abs(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region isNaN()
	//{ isNaN()

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

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 isNaN(const PackedFloat8_256 & v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const PackedFloat8_256 isNaN(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 isNaN(const PackedFloat4_256 & v) noexcept;

	// Checks whether a is Nan or b is a NaN value
	const PackedFloat4_256 isNaN(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
#endif

	//}
#pragma endregion

#pragma region mix()
	//{ mix()

	const Float_32 mix(const Float_32 x, const Float_32 y, const Float_32 factor) noexcept;

	const Float_64 mix(const Float_64 x, const Float_64 y, const Float_64 factor) noexcept;

	const PackedFloat4_128 mix(const PackedFloat4_128 & x, const PackedFloat4_128 & y, const Float_32 factor) noexcept;

	const PackedFloat4_128 mix(const PackedFloat4_128 & x, const PackedFloat4_128 & y, const PackedFloat4_128 & factor) noexcept;

	const PackedFloat2_128 mix(const PackedFloat2_128 & x, const PackedFloat2_128 & y, const Float_64 factor) noexcept;

	const PackedFloat2_128 mix(const PackedFloat2_128 & x, const PackedFloat2_128 & y, const PackedFloat2_128 & factor) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 mix(const PackedFloat8_256 & x, const PackedFloat8_256 & y, const Float_32 factor) noexcept;

	const PackedFloat8_256 mix(const PackedFloat8_256 & x, const PackedFloat8_256 & y, const PackedFloat8_256 & factor) noexcept;

	const PackedFloat4_256 mix(const PackedFloat4_256 & x, const PackedFloat4_256 & y, const Float_64 factor) noexcept;

	const PackedFloat4_256 mix(const PackedFloat4_256 & x, const PackedFloat4_256 & y, const PackedFloat4_256 & factor) noexcept;
#endif

	//}
#pragma endregion

#pragma region sqr()
	//{ sqr()

	const Float_32 sqr(const Float_32 v) noexcept;

	const Float_64 sqr(const Float_64 v) noexcept;

	const PackedFloat4_128 sqr(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 sqr(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 sqr(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 sqr(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region reciprocal()
	//{ reciprocal()

	const Float_32 reciprocal(const Float_32 v) noexcept;

	const Float_64 reciprocal(const Float_64 v) noexcept;

	const PackedFloat4_128 reciprocal(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 reciprocal(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 reciprocal(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 reciprocal(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region sqrt()
	//{ sqrt()

	const Float_32 sqrt(const Float_32 v) noexcept;

	const Float_64 sqrt(const Float_64 v) noexcept;

	const PackedFloat4_128 sqrt(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 sqrt(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 sqrt(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 sqrt(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region rsqrt()
	//{ rsqrt()

	const Float_32 rsqrt(const Float_32 v) noexcept;

	const Float_64 rsqrt(const Float_64 v) noexcept;

	const PackedFloat4_128 rsqrt(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 rsqrt(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 rsqrt(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 rsqrt(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region ceil()
	//{ ceil()

	const Float_32 ceil(const Float_32 v) noexcept;

	const Float_64 ceil(const Float_64 v) noexcept;

	const PackedFloat4_128 ceil(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 ceil(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 ceil(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 ceil(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region floor()
	//{ floor()

	const Float_32 floor(const Float_32 v) noexcept;

	const Float_64 floor(const Float_64 v) noexcept;

	const PackedFloat4_128 floor(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 floor(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 floor(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 floor(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region round()
	//{ round()

	const Float_32 round(const Float_32 v) noexcept;

	const Float_64 round(const Float_64 v) noexcept;

	const PackedFloat4_128 round(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 round(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 round(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 round(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region fract()
	//{ fract()

	const Float_32 fract(const Float_32 v) noexcept;

	const Float_64 fract(const Float_64 v) noexcept;

	const PackedFloat4_128 fract(const PackedFloat4_128 & v) noexcept;

	const PackedFloat2_128 fract(const PackedFloat2_128 & v) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 fract(const PackedFloat8_256 & v) noexcept;

	const PackedFloat4_256 fract(const PackedFloat4_256 & v) noexcept;
#endif

	//}
#pragma endregion

#pragma region divide()
	//{ divide()

	const long divide(const long a, const long b) noexcept;

	const unsigned long divide(const unsigned long a, const unsigned long b) noexcept;

	const Int_8 divide(const Int_8 a, const Int_8 b) noexcept;

	const UInt_8 divide(const UInt_8 a, const UInt_8 b) noexcept;

	const Int_16 divide(const Int_16 a, const Int_16 b) noexcept;

	const UInt_16 divide(const UInt_16 a, const UInt_16 b) noexcept;

	const Int_32 divide(const Int_32 a, const Int_32 b) noexcept;

	const UInt_32 divide(const UInt_32 a, const UInt_32 b) noexcept;

	const Int_64 divide(const Int_64 a, const Int_64 b) noexcept;

	const UInt_64 divide(const UInt_64 a, const UInt_64 b) noexcept;

	const Float_32 divide(const Float_32 a, const Float_32 b) noexcept;

	const Float_64 divide(const Float_64 a, const Float_64 b) noexcept;

	const PackedFloat4_128 divide(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

	const PackedFloat2_128 divide(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 divide(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 divide(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
#endif

	//}
#pragma endregion

#pragma region modulo()
	//{ modulo()

	const long modulo(const long a, const long b) noexcept;

	const unsigned long modulo(const unsigned long a, const unsigned long b) noexcept;

	const Int_8 modulo(const Int_8 a, const Int_8 b) noexcept;

	const UInt_8 modulo(const UInt_8 a, const UInt_8 b) noexcept;

	const Int_16 modulo(const Int_16 a, const Int_16 b) noexcept;

	const UInt_16 modulo(const UInt_16 a, const UInt_16 b) noexcept;

	const Int_32 modulo(const Int_32 a, const Int_32 b) noexcept;

	const UInt_32 modulo(const UInt_32 a, const UInt_32 b) noexcept;

	const Int_64 modulo(const Int_64 a, const Int_64 b) noexcept;

	const UInt_64 modulo(const UInt_64 a, const UInt_64 b) noexcept;

	const Float_32 modulo(const Float_32 a, const Float_32 b) noexcept;

	const Float_64 modulo(const Float_64 a, const Float_64 b) noexcept;

	const PackedFloat4_128 modulo(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept;

	const PackedFloat2_128 modulo(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
	const PackedFloat8_256 modulo(const PackedFloat8_256 & a, const PackedFloat8_256 & b) noexcept;

	const PackedFloat4_256 modulo(const PackedFloat4_256 & a, const PackedFloat4_256 & b) noexcept;
#endif

	//}
#pragma endregion

#pragma region roundDown() for integers
	//{ roundDown() for integers

	const Int_8 roundDown(const Int_8 value, const Int_8 stepSize) noexcept;

	const UInt_8 roundDown(const UInt_8 value, const UInt_8 stepSize) noexcept;

	const Int_16 roundDown(const Int_16 value, const Int_16 stepSize) noexcept;

	const UInt_16 roundDown(const UInt_16 value, const UInt_16 stepSize) noexcept;

	const Int_32 roundDown(const Int_32 value, const Int_32 stepSize) noexcept;

	const UInt_32 roundDown(const UInt_32 value, const UInt_32 stepSize) noexcept;

	const Int_64 roundDown(const Int_64 value, const Int_64 stepSize) noexcept;

	const UInt_64 roundDown(const UInt_64 value, const UInt_64 stepSize) noexcept;

	//}
#pragma endregion

#pragma region bitpad()
	//{ bitpad()

	const Int_8 bitpad(const Int_8 unpadded, const Int_8 bitPadding) noexcept;

	const UInt_8 bitpad(const UInt_8 unpadded, const UInt_8 bitPadding) noexcept;

	const Int_16 bitpad(const Int_16 unpadded, const Int_16 bitPadding) noexcept;

	const UInt_16 bitpad(const UInt_16 unpadded, const UInt_16 bitPadding) noexcept;

	const Int_32 bitpad(const Int_32 unpadded, const Int_32 bitPadding) noexcept;

	const UInt_32 bitpad(const UInt_32 unpadded, const UInt_32 bitPadding) noexcept;

	const Int_64 bitpad(const Int_64 unpadded, const Int_64 bitPadding) noexcept;

	const UInt_64 bitpad(const UInt_64 unpadded, const UInt_64 bitPadding) noexcept;

	//}
#pragma endregion

#pragma region modpad()
	//{ modpad()

	const Int_8 modpad(const Int_8 unpadded, const Int_8 modPadding) noexcept;

	const UInt_8 modpad(const UInt_8 unpadded, const UInt_8 modPadding) noexcept;

	const Int_16 modpad(const Int_16 unpadded, const Int_16 modPadding) noexcept;

	const UInt_16 modpad(const UInt_16 unpadded, const UInt_16 modPadding) noexcept;

	const Int_32 modpad(const Int_32 unpadded, const Int_32 modPadding) noexcept;

	const UInt_32 modpad(const UInt_32 unpadded, const UInt_32 modPadding) noexcept;

	const Int_64 modpad(const Int_64 unpadded, const Int_64 modPadding) noexcept;

	const UInt_64 modpad(const UInt_64 unpadded, const UInt_64 modPadding) noexcept;

	//}
#pragma endregion

#pragma region log()
	//{ log()

	const Float_32 log(const Float_32 v) noexcept;

	const Float_64 log(const Float_64 v) noexcept;

	//}
#pragma endregion

#pragma region logN()
	//{ logN()

	const Float_32 logN(const Float_32 v, const Float_32 logBase) noexcept;

	const Float_64 logN(const Float_64 v, const Float_64 logBase) noexcept;

	//}
#pragma endregion

#pragma region pow()
	//{ pow()

	const Float_32 pow(const Float_32 base, const Float_32 exponent) noexcept;

	const Float_64 pow(const Float_64 base, const Float_64 exponent) noexcept;

	//}
#pragma endregion

#pragma region exp()
	//{ exp()

	// Fixes error in std::exp for MSVC on X64
	const Float_32 exp(const Float_32 v) noexcept;

	const Float_64 exp(const Float_64 v) noexcept;

	//}
#pragma endregion

#pragma region cos()
	//{ cos()

	const Float_32 cos(const Float_32 v) noexcept;

	const Float_64 cos(const Float_64 v) noexcept;

	//}
#pragma endregion

#pragma region sin()
	//{ sin()

	const Float_32 sin(const Float_32 v) noexcept;

	const Float_64 sin(const Float_64 v) noexcept;

	//}
#pragma endregion

#pragma region atan2()
	//{ atan2()

	const Float_32 atan2(const Float_32 x, const Float_32 y) noexcept;

	const Float_64 atan2(const Float_64 x, const Float_64 y) noexcept;

	//}
#pragma endregion

#pragma region acos()
	//{ acos()

	const Float_32 acos(const Float_32 v) noexcept;

	const Float_64 acos(const Float_64 v) noexcept;

	//}
#pragma endregion

#pragma region tan()
	//{ tan()

	const Float_32 tan(const Float_32 v) noexcept;

	const Float_64 tan(const Float_64 v) noexcept;

	//}
#pragma endregion

#pragma region staticFor()
	//{ staticFor()

	// compile time static for loop for range [First, Last)
	template < ASizeT First, ASizeT Last >
	struct StaticFor
	{
		template <typename Lambda>
		static inline constexpr void apply(const Lambda & iteratee) {
			if constexpr (First < Last) {
				iteratee(First);
				if constexpr (First + 1 < Last) {
					StaticFor<First + 1, Last>::apply(iteratee);
				}
			}
		}
	};

	// Can take care for C4100 when First equals Last index
	template < ASizeT N >
	struct StaticFor<N, N>
	{
		template <typename Lambda>
		static inline constexpr void apply(const Lambda & /* iteratee */) {
		}
	};

	// compile time static for loop for range [First, Last)
	template < ASizeT First, ASizeT Last, typename Lambda >
	inline constexpr void staticFor(const Lambda & iteratee) noexcept
	{
		StaticFor<First, Last>::apply(iteratee);
	}

	//}
#pragma endregion


#pragma region average()
	//{ average()

	template <ASizeT Size, typename T>
	inline const T average(const T * const v) noexcept
	{
		T s = Zero<T>();

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			s += v[i];
		});

		return s / static_cast<T>(Size);
	}

	//}
#pragma endregion

#pragma region normalize()
	//{ normalize()

	template <ASizeT Size, typename T>
	inline void normalize(T * const v) noexcept
	{
		T s = Zero<T>();

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			s += sqr(v[i]);
		});

		const T rs = rsqrt(s);
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			v[i] *= rs;
		});
	}

	//}
#pragma endregion

}
