#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    // A new seed from a seed and a value, e.g. a pixel index or the number of a sample: integer hashing only, so the
    // same on every compiler
    UInt_32 deriveSeed(UInt_32 seed, UInt_32 value) noexcept;

    // The point index of a low-discrepancy sequence in [0, 1)^2 (x, y; Roberts' R2), shifted by the seed
    // (Cranley-Patterson rotation): the first n points spread evenly for any n, and every seed gives its own pattern
    Float4 sequencePoint2D(UInt_32 seed, ASizeT index) noexcept;
}
