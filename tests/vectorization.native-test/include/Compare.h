#pragma once

#include <vectorization.h>

namespace vectorization
{
    namespace test
    {
        struct Compare
        {
            template <typename T>
            static const auto nearEqualsRelative(const T & a, const T & b) {
                const auto maxOfAbsoluteOfBoth = max(abs(a), abs(b));
                return (abs(a - b) / maxOfAbsoluteOfBoth) < Epsilon<T>();
            }

            template <typename T>
            static const auto equalsOrdered(const T & a, const T & b) {
                return a == b;
            }

            template <typename T>
            static const auto isBothNotANumber(const T & a, const T & b) {
                return vectorization::isNaN(a) & vectorization::isNaN(b);
            }
        };
    }
}
