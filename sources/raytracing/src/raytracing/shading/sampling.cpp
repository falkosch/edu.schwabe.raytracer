#include "raytracing/shading/sampling.h"
#include "../../stdafx.h"

namespace raytracer
{
    namespace
    {
        // the golden ratio's fraction keeps consecutive values apart before hashing
        constexpr UInt_32 ValueSpread = 0x9e3779b9u;

        // the second offset component hashes the seed with these bits flipped
        constexpr UInt_32 OffsetYBits = 0x5bd1e995u;

        // Chris Wellons' lowbias32 integer hash, for a scalar (UInt_32) or for every lane of UInt4, UInt8_32: one
        // definition, so that the SIMD lanes hash exactly like the scalar
        template <typename V>
        V hash(V x) noexcept
        {
            x ^= x >> 16u;
            x *= V(0x7feb352du);
            x ^= x >> 15u;
            x *= V(0x846ca68bu);
            x ^= x >> 16u;
            return x;
        }

        // 2^-24: the upper 24 bits of a hash as a float in [0, 1)
        constexpr Float UnitScale = 1.0f / static_cast<Float>(1u << 24);
    }

    UInt_32 deriveSeed(const UInt_32 seed, const UInt_32 value) noexcept
    {
        return hash(seed ^ hash(value + ValueSpread));
    }

    UInt8_32 deriveSeeds(const UInt_32 seed, const UInt_32 firstValue) noexcept
    {
        const auto values = UInt8_32(firstValue) + UInt8_32(0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u);
        return hash(UInt8_32(seed) ^ hash(values + UInt8_32(ValueSpread)));
    }

    Float4 sequenceOffset2D(const UInt_32 seed) noexcept
    {
        // the upper 24 bits convert exactly, and scale exactly by a power of two; z and w scale to 0
        const auto hashes = hash(UInt4(seed, seed ^ OffsetYBits, Zero<UInt_32>(), Zero<UInt_32>()));
        const auto scale = Float4(UnitScale, UnitScale, Zero<Float>(), Zero<Float>());
        return convert<Float4>(hashes >> 8u) * scale;
    }

    Float4 sequencePoint2D(const Float4& offset, const ASizeT index) noexcept
    {
        // 1 / g and 1 / g^2 for the plastic number g, the solution of x^3 = x + 1
        constexpr Float R2X = 0.7548776662466927f;
        constexpr Float R2Y = 0.5698402909980532f;
        const auto step = Float4(R2X, R2Y, Zero<Float>(), Zero<Float>()) * static_cast<Float>(index);
        return fract(offset + step);
    }

    Float4 sequencePoint2D(const UInt_32 seed, const ASizeT index) noexcept
    {
        return sequencePoint2D(sequenceOffset2D(seed), index);
    }

    ASizeT stratifiedIndex(const UInt_32 stratum, const ASizeT samples, const ASizeT sample) noexcept
    {
        return static_cast<ASizeT>(stratum) * samples + sample;
    }
}
