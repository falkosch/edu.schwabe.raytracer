#pragma once

#include "compiler_intrinsics.h"

namespace vectorization
{
    struct VectorSizes
    {
        static constexpr ASizeT TooSmall = 0;
        static constexpr ASizeT X = 1;
        static constexpr ASizeT Y = 2;
        static constexpr ASizeT Z = 3;
        static constexpr ASizeT W = 4;
        static constexpr ASizeT AboveW = 5;

        static constexpr ASizeT X1 = 1;
        static constexpr ASizeT X2 = 2;
        static constexpr ASizeT X3 = 3;
        static constexpr ASizeT X4 = 4;
        static constexpr ASizeT X5 = 5;
        static constexpr ASizeT X6 = 6;
        static constexpr ASizeT X7 = 7;
        static constexpr ASizeT X8 = 8;
        static constexpr ASizeT AboveX8 = 9;
    };

    struct VectorIndices
    {
        static constexpr ASizeT X = 0;
        static constexpr ASizeT Y = 1;
        static constexpr ASizeT Z = 2;
        static constexpr ASizeT W = 3;
        static constexpr ASizeT AboveW = 4;

        static constexpr ASizeT X1 = 0;
        static constexpr ASizeT X2 = 1;
        static constexpr ASizeT X3 = 2;
        static constexpr ASizeT X4 = 3;
        static constexpr ASizeT X5 = 4;
        static constexpr ASizeT X6 = 5;
        static constexpr ASizeT X7 = 6;
        static constexpr ASizeT X8 = 7;
        static constexpr ASizeT AboveX8 = 8;
    };

    // Vector bit mask
    struct VectorBits
    {
        static constexpr ASizeT None = 0;
        static constexpr ASizeT X = 1;
        static constexpr ASizeT Y = 2;
        static constexpr ASizeT Z = 4;
        static constexpr ASizeT W = 8;
        static constexpr ASizeT AboveW = 16;

        static constexpr ASizeT X1 = 1;
        static constexpr ASizeT X2 = 2;
        static constexpr ASizeT X3 = 4;
        static constexpr ASizeT X4 = 8;
        static constexpr ASizeT X5 = 16;
        static constexpr ASizeT X6 = 32;
        static constexpr ASizeT X7 = 64;
        static constexpr ASizeT X8 = 128;
        static constexpr ASizeT AboveX8 = 256;
    };

    /// Reflects the packed-type of the given typename TValueType
    template <typename TValueType, ASizeT TElements>
    struct PackedTypes
    {
        static constexpr ASizeT Elements = TElements;
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
