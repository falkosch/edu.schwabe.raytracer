#pragma once

#include "compiler_intrinsics.h"

#include <cstddef>

namespace vectorization
{

	typedef std::size_t ASizeT;


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

}
