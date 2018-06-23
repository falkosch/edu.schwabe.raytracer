#pragma once

#include "compiler_types.h"

namespace vectorization
{

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


	/// Reflects the packed-type of the given typename TValueType
	template <typename TValueType, ASizeT TElements>
	struct PackedTypes
	{
		static const ASizeT Elements = TElements;
		typedef TValueType ValueType;
		typedef void Type;
	};


	/// Reflects the integer-based boolean-type of the given typename TValueType
	template <typename TValueType>
	struct BoolTypes {
		typedef TValueType ValueType;
		typedef void Type;
	};

}
