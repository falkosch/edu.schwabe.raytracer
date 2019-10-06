#pragma once

#include <vectorization.h>

namespace vectorization
{
    namespace test
    {
        template<typename T>
        union ValueBits
        {
            T value;
            typename BoolTypes<T>::Type bits;

            ValueBits(const T value) : value(value) { }
        };
    }
}