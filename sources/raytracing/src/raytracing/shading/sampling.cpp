#include "raytracing/shading/sampling.h"
#include "../../stdafx.h"

namespace raytracer
{
    namespace
    {
        // Chris Wellons' lowbias32 integer hash
        UInt_32 hash(UInt_32 x) noexcept
        {
            x ^= x >> 16;
            x *= 0x7feb352du;
            x ^= x >> 15;
            x *= 0x846ca68bu;
            x ^= x >> 16;
            return x;
        }

        // The upper 24 bits as a float in [0, 1)
        Float unitFloat(const UInt_32 bits) noexcept
        {
            return static_cast<Float>(bits >> 8) * (One<Float>() / static_cast<Float>(One<UInt_32>() << 24));
        }
    }

    UInt_32 deriveSeed(const UInt_32 seed, const UInt_32 value) noexcept
    {
        // the golden ratio's fraction keeps consecutive values apart before hashing
        return hash(seed ^ hash(value + 0x9e3779b9u));
    }

    Float4 sequencePoint2D(const UInt_32 seed, const ASizeT index) noexcept
    {
        // 1 / g and 1 / g^2 for the plastic number g, the solution of x^3 = x + 1
        constexpr Float R2X = 0.7548776662466927f;
        constexpr Float R2Y = 0.5698402909980532f;
        const auto offset =
            Float4(unitFloat(hash(seed)), unitFloat(hash(seed ^ 0x5bd1e995u)), Zero<Float>(), Zero<Float>());
        const auto step = Float4(R2X, R2Y, Zero<Float>(), Zero<Float>()) * static_cast<Float>(index);
        return fract(offset + step);
    }
}
