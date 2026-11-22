#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    // A new seed from a seed and a value, e.g. a pixel index or the number of a sample: integer hashing only, so the
    // same on every compiler
    UInt_32 deriveSeed(UInt_32 seed, UInt_32 value) noexcept;

    // deriveSeed(seed, firstValue + i) in lane i, for the 8 values from firstValue on: the same hash in SIMD lanes
    UInt8_32 deriveSeeds(UInt_32 seed, UInt_32 firstValue) noexcept;

    // The point index of a low-discrepancy sequence in [0, 1)^2 (x, y; Roberts' R2), shifted by the seed
    // (Cranley-Patterson rotation): the first n points spread evenly for any n, and every seed gives its own pattern
    Float4 sequencePoint2D(UInt_32 seed, ASizeT index) noexcept;

    // The shift of the seed's sequence (x, y; z and w are 0), both components hashed in SIMD lanes at once: loops over
    // the points of one seed compute it once and pass it to sequencePoint2D(offset, index)
    Float4 sequenceOffset2D(UInt_32 seed) noexcept;

    // The point index of the sequence with that shift: sequencePoint2D(sequenceOffset2D(seed), index) is
    // sequencePoint2D(seed, index)
    Float4 sequencePoint2D(const Float4& offset, ASizeT index) noexcept;

    // The sequence index of a ray's sample when the rays of several strata (the sub-samples of a pixel) share one
    // sequence: stratum s takes the points s * samples to (s + 1) * samples - 1. samples must be the number of
    // samples the hit takes, not their maximum: a fixed stride would spread few samples per hit along a line.
    ASizeT stratifiedIndex(UInt_32 stratum, ASizeT samples, ASizeT sample) noexcept;
}
