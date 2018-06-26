/*
* Includes the required header for the intrinsics defines some useful macros not always readily available for all compilers.
*/

#pragma once

#include "compiler_definitions.h"

#if defined(__GNUC__)
#include <x86intrin.h>
#else
#include <intrin.h>
#endif


#define BLEND_MASK(X, Y, Z, W) ( \
		((X & 1) << 0) \
		| ((Y & 1) << 1) \
		| ((Z & 1) << 2) \
		| ((W & 1) << 3) \
		)

#define BLEND_MASK_i4(X, Y, Z, W) ( \
        ((X & 1) << 0) \
		| ((X & 1) << 1) \
		| ((Y & 1) << 2) \
		| ((Y & 1) << 3) \
        | ((Z & 1) << 4) \
		| ((Z & 1) << 5) \
		| ((W & 1) << 6) \
		| ((W & 1) << 7) \
		)


namespace vectorization
{

	typedef char Int_8;
	typedef short Int_16;
	typedef int Int_32;
	typedef long long Int_64;


	typedef unsigned char UInt_8;
	typedef unsigned short UInt_16;
	typedef unsigned int UInt_32;
	typedef unsigned long long UInt_64;


	typedef float Float_32;
	typedef double Float_64;


	typedef Int_8 Bool_8;
	typedef Int_16 Bool_16;
	typedef Int_32 Bool_32;
	typedef Int_64 Bool_64;


	typedef __m128 PackedFloat4_128;
	typedef __m128d PackedFloat2_128;
	typedef __m128i PackedInts_128;
	typedef __m256 PackedFloat8_256;
	typedef __m256d PackedFloat4_256;
	typedef __m256i PackedInts_256;



	const PackedFloat4_128 _mm_cvtepu32_ps(const PackedInts_128 v);

}
