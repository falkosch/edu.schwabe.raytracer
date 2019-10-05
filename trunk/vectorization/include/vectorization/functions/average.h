#pragma once

#include "staticFor.h"

#include "../constants/values/Zero.h"

#include "../architecture/meta_types.h"

namespace vectorization
{
    template <ASizeT Size, typename T>
    inline const T average(const T * const v) noexcept {
        T s = Zero<T>();

        StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
            s += v[i];
        });

        return s / static_cast<T>(Size);
    }
}
