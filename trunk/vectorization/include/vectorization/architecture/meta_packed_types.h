#pragma once

#include "meta_types.h"

namespace vectorization
{

	template<>
	struct PackedTypes < Int_8, 16 >
	{
		static const ASizeT Elements = 16;
		typedef Int_8 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_8, 32 >
	{
		static const ASizeT Elements = 32;
		typedef Int_8 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_8, 64 >
	{
		static const ASizeT Elements = 64;
		typedef Int_8 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < Int_16, 8 >
	{
		static const ASizeT Elements = 8;
		typedef Int_16 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_16, 16 >
	{
		static const ASizeT Elements = 16;
		typedef Int_16 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_16, 32 >
	{
		static const ASizeT Elements = 32;
		typedef Int_16 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < Int_32, 4 >
	{
		static const ASizeT Elements = 4;
		typedef Int_32 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_32, 8 >
	{
		static const ASizeT Elements = 8;
		typedef Int_32 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_32, 16 >
	{
		static const ASizeT Elements = 16;
		typedef Int_32 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < Int_64, 2 >
	{
		static const ASizeT Elements = 2;
		typedef Int_64 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_64, 4 >
	{
		static const ASizeT Elements = 4;
		typedef Int_64 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_64, 8 >
	{
		static const ASizeT Elements = 8;
		typedef Int_64 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < UInt_8, 16 >
	{
		static const ASizeT Elements = 16;
		typedef UInt_8 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_8, 32 >
	{
		static const ASizeT Elements = 32;
		typedef UInt_8 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_8, 64 >
	{
		static const ASizeT Elements = 64;
		typedef UInt_8 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < UInt_16, 8 >
	{
		static const ASizeT Elements = 8;
		typedef UInt_16 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_16, 16 >
	{
		static const ASizeT Elements = 16;
		typedef UInt_16 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_16, 32 >
	{
		static const ASizeT Elements = 32;
		typedef UInt_16 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < UInt_32, 4 >
	{
		static const ASizeT Elements = 4;
		typedef UInt_32 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_32, 8 >
	{
		static const ASizeT Elements = 8;
		typedef UInt_32 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_32, 16 >
	{
		static const ASizeT Elements = 16;
		typedef UInt_32 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < UInt_64, 2 >
	{
		static const ASizeT Elements = 2;
		typedef UInt_64 ValueType;
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_64, 4 >
	{
		static const ASizeT Elements = 4;
		typedef UInt_64 ValueType;
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_64, 8 >
	{
		static const ASizeT Elements = 8;
		typedef UInt_64 ValueType;
		typedef PackedInts_512 Type;
	};



	template<>
	struct PackedTypes < Float_32, 4 >
	{
		static const ASizeT Elements = 4;
		typedef Float_32 ValueType;
		typedef PackedFloat4_128 Type;
	};

	template<>
	struct PackedTypes < Float_32, 8 >
	{
		static const ASizeT Elements = 8;
		typedef Float_32 ValueType;
		typedef PackedFloat8_256 Type;
	};

	template<>
	struct PackedTypes < Float_32, 16 >
	{
		static const ASizeT Elements = 16;
		typedef Float_32 ValueType;
		typedef PackedFloat16_512 Type;
	};



	template<>
	struct PackedTypes < Float_64, 2 >
	{
		static const ASizeT Elements = 2;
		typedef Float_64 ValueType;
		typedef PackedFloat2_128 Type;
	};

	template<>
	struct PackedTypes < Float_64, 4 >
	{
		static const ASizeT Elements = 4;
		typedef Float_64 ValueType;
		typedef PackedFloat4_256 Type;
	};

	template<>
	struct PackedTypes < Float_64, 8 >
	{
		static const ASizeT Elements = 8;
		typedef Float_64 ValueType;
		typedef PackedFloat8_512 Type;
	};

}
