#include "ray_set.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

namespace benchmarks
{
    namespace
    {
        constexpr std::mt19937::result_type RandomSeed{0x5eed1234u};
    }

    RaySet createCoherentRays(const AxisAlignedBoundingBox& bounding)
    {
        const auto minimum = bounding.minimum;
        const auto size = extents(bounding);
        const auto diagonal = length3(size);
        const auto boxCenter = center(bounding);
        const auto eye = Float4(x(boxCenter), y(boxCenter), z(minimum) - 2.0f * diagonal, 1.0f);

        auto rays = RaySet{.name = "coherent", .rayCasts = {}};
        rays.rayCasts.reserve(CoherentResolution * CoherentResolution);

        const auto step = 1.0f / static_cast<Float>(CoherentResolution);
        for (auto row = ASizeT{0}; row < CoherentResolution; ++row)
        {
            for (auto column = ASizeT{0}; column < CoherentResolution; ++column)
            {
                const auto u = (static_cast<Float>(column) + 0.5f) * step;
                const auto v = (static_cast<Float>(row) + 0.5f) * step;
                const auto target =
                    Float4(x(minimum) + u * x(size), y(minimum) + v * y(size), z(minimum), 1.0f);
                const auto toTarget = target - eye;
                rays.rayCasts.emplace_back(
                    Ray(eye, normalize3(toTarget)), cullingOrientationToMask(-1), Zero<Size2>(),
                    length3(toTarget) + diagonal
                );
            }
        }
        return rays;
    }

    RaySet createIncoherentRays(const AxisAlignedBoundingBox& bounding)
    {
        const auto minimum = bounding.minimum;
        const auto size = extents(bounding);
        const auto diagonal = length3(size);

        auto generator = std::mt19937{RandomSeed};
        auto uniform = [&generator] { return static_cast<Float>(generator() >> 8) * (1.0f / 16777216.0f); };

        auto rays = RaySet{"incoherent", {}};
        rays.rayCasts.reserve(IncoherentRayCount);

        for (auto i = ASizeT{0}; i < IncoherentRayCount; ++i)
        {
            const auto origin = Float4(
                x(minimum) + uniform() * x(size), y(minimum) + uniform() * y(size),
                z(minimum) + uniform() * z(size), 1.0f
            );
            const auto cosTheta = 2.0f * uniform() - 1.0f;
            const auto sinTheta = std::sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
            const auto phi = 2.0f * std::numbers::pi_v<Float> * uniform();
            const auto direction = Float4(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta, 0.0f);
            rays.rayCasts.emplace_back(Ray(origin, direction), cullingOrientationToMask(0), Zero<Size2>(),
                                       diagonal);
        }
        return rays;
    }
}
