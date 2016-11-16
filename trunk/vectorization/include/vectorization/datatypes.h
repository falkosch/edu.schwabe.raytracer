/*
 * Defines the core specifications:
 *      - Aligned datatypes and their numerical limits.
 *
 * @file vectorization/datatypes.h
 */

#pragma once

#include "compiler.h"

#include <limits>

namespace vectorization
{

    // Architecture dependend size type

    typedef std::size_t ASizeT;
    typedef std::numeric_limits<ASizeT> ASizeT_Limits;

    // Shortcuts for platform types

    typedef char Int_8;
    typedef std::numeric_limits<Int_8> Int_8_Limits;

    typedef short Int_16;
    typedef std::numeric_limits<Int_16> Int_16_Limits;

    typedef int Int_32;
    typedef std::numeric_limits<Int_32> Int_32_Limits;

    typedef long long Int_64;
    typedef std::numeric_limits<Int_64> Int_64_Limits;

    typedef unsigned char UInt_8;
    typedef std::numeric_limits<UInt_8> UInt_8_Limits;

    typedef unsigned short UInt_16;
    typedef std::numeric_limits<UInt_16> UInt_16_Limits;

    typedef unsigned int UInt_32;
    typedef std::numeric_limits<UInt_32> UInt_32_Limits;

    typedef unsigned long long UInt_64;
    typedef std::numeric_limits<UInt_64> UInt_64_Limits;

    typedef float Float_32;
    typedef std::numeric_limits<Float_32> Float_32_Limits;

    typedef double Float_64;
    typedef std::numeric_limits<Float_64> Float_64_Limits;

    // Shortcuts for integer-based booleans

    typedef Int_8 Bool_8;
    typedef Int_8_Limits Bool_8_Limits;

    typedef Int_16 Bool_16;
    typedef Int_16_Limits Bool_16_Limits;

    typedef Int_32 Bool_32;
    typedef Int_32_Limits Bool_32_Limits;

    typedef Int_64 Bool_64;
    typedef Int_64_Limits Bool_64_Limits;

    // Shortcuts for the standard precision

#ifdef VECTORIZATION_HIGH_PRECISION

    typedef Int_64 Int;
    typedef Int_64_Limits Int_Limits;

    typedef UInt_64 UInt;
    typedef UInt_64_Limits UInt_Limits;

    typedef Bool_64 Bool;
    typedef Bool_64_Limits Bool_Limits;

    typedef Float_64 Float;
    typedef Float_64_Limits Float_Limits;

#else

    typedef Int_32 Int;
    typedef Int_32_Limits Int_Limits;

    typedef UInt_32 UInt;
    typedef UInt_32_Limits UInt_Limits;

    typedef Bool_32 Bool;
    typedef Bool_32_Limits Bool_Limits;

    typedef Float_32 Float;
    typedef Float_32_Limits Float_Limits;

#endif

    // Shortcuts for the platform-based aligned standard types

#ifdef ARCH_X64

    typedef Float_64 AFloat;
    typedef Float_64_Limits AFloat_Limits;

    typedef Int_64 AInt;
    typedef Int_64_Limits AInt_Limits;

    typedef UInt_64 AUInt;
    typedef UInt_64_Limits AUInt_Limits;

    typedef Bool_64 ABool;
    typedef Bool_64_Limits ABool_Limits;

#else

    typedef Float_32 AFloat;
    typedef Float_32_Limits AFloat_Limits;

    typedef Int_32 AInt;
    typedef Int_32_Limits AInt_Limits;

    typedef UInt_32 AUInt;
    typedef UInt_32_Limits AUInt_Limits;

    typedef Bool_32 ABool;
    typedef Bool_32_Limits ABool_Limits;

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

#pragma region Bool types
    //{Bool types

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

    //}
#pragma endregion

}
