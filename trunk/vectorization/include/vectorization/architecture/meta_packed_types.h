#pragma once

#include "meta_types.h"

namespace vectorization
{

	template<>
	struct PackedTypes < Float_32, 4 >
	{
		typedef PackedFloat4_128 Type;
	};

	template<>
	struct PackedTypes < Float_64, 2 >
	{
		typedef PackedFloat2_128 Type;
	};

	template<>
	struct PackedTypes < Int_8, 16 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_8, 16 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_16, 8 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_16, 8 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_32, 4 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_32, 4 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_64, 2 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_64, 2 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Float_32, 8 >
	{
		typedef PackedFloat8_256 Type;
	};

	template<>
	struct PackedTypes < Float_64, 4 >
	{
		typedef PackedFloat4_256 Type;
	};

	template<>
	struct PackedTypes < Int_8, 32 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_8, 32 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_16, 16 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_16, 16 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_32, 8 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_32, 8 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_64, 4 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_64, 4 >
	{
		typedef PackedInts_256 Type;
	};

}
