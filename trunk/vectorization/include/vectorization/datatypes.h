/*
 * Defines the core specifications:
 *      - Aligned datatypes and their numerical limits.
 *
 * @file vectorization/datatypes.h
 */

#pragma once

#include "compiler.h"

#include <cstddef>

namespace vectorization
{

	// Architecture dependend size type

	typedef std::size_t ASizeT;

	// Shortcuts for platform types

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

	// Shortcuts for integer-based booleans

	typedef Int_8 Bool_8;
	typedef Int_16 Bool_16;
	typedef Int_32 Bool_32;
	typedef Int_64 Bool_64;


	// Shortcuts for the types aligned with the architecture

#ifdef ARCH_X64

	typedef Float_64 AFloat;
	typedef Int_64 AInt;
	typedef UInt_64 AUInt;
	typedef Bool_64 ABool;

#else

	typedef Float_32 AFloat;
	typedef Int_32 AInt;
	typedef UInt_32 AUInt;
	typedef Bool_32 ABool;

#endif

	struct VectorSizes
	{
		static const ASizeT TooSmall = 0;
		static const ASizeT X = 1;
		static const ASizeT Y = 2;
		static const ASizeT Z = 3;
		static const ASizeT W = 4;
		static const ASizeT AboveW = 5;
	};

	struct VectorIndices
	{
		static const ASizeT X = 0;
		static const ASizeT Y = 1;
		static const ASizeT Z = 2;
		static const ASizeT W = 3;
		static const ASizeT AboveW = 4;
	};

	struct VectorBits
	{
		static const ASizeT None = 0;
		static const ASizeT X = 1;
		static const ASizeT Y = 2;
		static const ASizeT Z = 4;
		static const ASizeT W = 8;
		static const ASizeT AboveW = 16;
	};


	/// Assigns a integer-based boolean-type to the given ValueType
	template <typename TValueType>
	struct BoolTypes {
		typedef void ValueType;
		typedef void Type;
	};

	template <>
	struct BoolTypes < Int_8 > {
		typedef Int_8 ValueType;
		typedef Bool_8 Type;
	};

	template <>
	struct BoolTypes < Int_16 > {
		typedef Int_16 ValueType;
		typedef Bool_16 Type;
	};

	template <>
	struct BoolTypes < Int_32 > {
		typedef Int_32 ValueType;
		typedef Bool_32 Type;
	};

	template <>
	struct BoolTypes < Int_64 > {
		typedef Int_64 ValueType;
		typedef Bool_64 Type;
	};

	template <>
	struct BoolTypes < UInt_8 > {
		typedef UInt_8 ValueType;
		typedef Bool_8 Type;
	};

	template <>
	struct BoolTypes < UInt_16 > {
		typedef UInt_16 ValueType;
		typedef Bool_16 Type;
	};

	template <>
	struct BoolTypes < UInt_32 > {
		typedef UInt_32 ValueType;
		typedef Bool_32 Type;
	};

	template <>
	struct BoolTypes < UInt_64 > {
		typedef UInt_64 ValueType;
		typedef Bool_64 Type;
	};

	template <>
	struct BoolTypes < Float_32 > {
		typedef Float_32 ValueType;
		typedef Bool_32 Type;
	};

	template <>
	struct BoolTypes < Float_64 > {
		typedef Float_64 ValueType;
		typedef Bool_64 Type;
	};

}
