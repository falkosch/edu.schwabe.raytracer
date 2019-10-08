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

        static const ASizeT X1 = 1;
        static const ASizeT X2 = 2;
        static const ASizeT X3 = 3;
        static const ASizeT X4 = 4;
        static const ASizeT X5 = 5;
        static const ASizeT X6 = 6;
        static const ASizeT X7 = 7;
        static const ASizeT X8 = 8;
        static const ASizeT AboveX8 = 9;
    };

    struct VectorIndices
    {
        static const ASizeT X = 0;
        static const ASizeT Y = 1;
        static const ASizeT Z = 2;
        static const ASizeT W = 3;
        static const ASizeT AboveW = 4;

        static const ASizeT X1 = 0;
        static const ASizeT X2 = 1;
        static const ASizeT X3 = 2;
        static const ASizeT X4 = 3;
        static const ASizeT X5 = 4;
        static const ASizeT X6 = 5;
        static const ASizeT X7 = 6;
        static const ASizeT X8 = 7;
        static const ASizeT AboveX8 = 8;
    };

    // Vector bit mask
    struct VectorBits
    {
        static const ASizeT None = 0;
        static const ASizeT X = 1;
        static const ASizeT Y = 2;
        static const ASizeT Z = 4;
        static const ASizeT W = 8;
        static const ASizeT AboveW = 16;

        static const ASizeT X1 = 1;
        static const ASizeT X2 = 2;
        static const ASizeT X3 = 4;
        static const ASizeT X4 = 8;
        static const ASizeT X5 = 16;
        static const ASizeT X6 = 32;
        static const ASizeT X7 = 64;
        static const ASizeT X8 = 128;
        static const ASizeT AboveX8 = 256;
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
    struct BoolTypes
    {
        typedef TValueType ValueType;
        typedef void Type;
    };
}
