#include "vectorization/functions/atan2.h"

#include <cmath>

namespace vectorization
{
    const Float_32 atan2(const Float_32 x, const Float_32 y) noexcept {
        return std::atan2(y, x);
    }

    const Float_64 atan2(const Float_64 x, const Float_64 y) noexcept {
        return std::atan2(y, x);
    }
}