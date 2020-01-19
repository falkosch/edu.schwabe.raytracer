#include "vectorization/functions/sin.h"

#include <cmath>

namespace vectorization
{
    const Float_32 sin(const Float_32 v) noexcept {
        return std::sin(v);
    }

    const Float_64 sin(const Float_64 v) noexcept {
        return std::sin(v);
    }
}