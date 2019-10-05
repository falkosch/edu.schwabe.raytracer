#pragma once

#include <vectorization.h>

namespace vectorization
{
    namespace test
    {
        struct Compare
        {
            static const v_f32_4::VectorBoolType nearEqualsRelative(const v_f32_4 & a, const v_f32_4 & b) {
                const auto maxOfAbsoluteOfBoth = max(abs(a), abs(b));
                return (abs(a - b) / maxOfAbsoluteOfBoth) < Epsilon<v_f32_4>();
            }

            static const v_f32_4::VectorBoolType equalsOrdered(const v_f32_4 & a, const v_f32_4 & b) {
                return a == b;
            }

            static const v_f32_4::VectorBoolType isBothNotANumber(const v_f32_4 & a, const v_f32_4 & b) {
                return vectorization::isNaN(a) & vectorization::isNaN(b);
            }
        };
    }
}
