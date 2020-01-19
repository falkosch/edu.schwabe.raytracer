#pragma once

#include <vectorization.h>

namespace vectorization
{
    namespace test
    {
        struct Mutate
        {
            template <ASizeT ReplacementIndex, typename T, class _InIt, class _OutIt>
            static void copyAndReplaceAt(_InIt _First, _InIt _Last, _OutIt _Dest, const T & replacement) {
                std::copy(_First, _Last, _Dest);
                *(_Dest + ReplacementIndex) = replacement;
            }
        };
    }
}
