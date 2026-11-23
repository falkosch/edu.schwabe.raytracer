#include "raytracing/shading/shaders/environment_coordinates.h"
#include "../../../stdafx.h"

#include <limits>

namespace raytracer
{
    namespace
    {
        Float4 fisheyeDomeCoordinates(const Float4& direction) noexcept
        {
            const auto uv = multiplyAdd(Half<Float4>(), xz_xz(direction, direction), Half<Float4>());
            return Float4(x(uv), y(uv), Zero<Float>(), Zero<Float>());
        }

        // The ball as photographed along -z, whose center shows +z, rotated by 180 degrees around y so that its center
        // shows -z: u = 0.5 - x / m, v = 0.5 + y / m with m = 2 * sqrt(x^2 + y^2 + (1 - z)^2)
        Float4 mirrorBallCoordinates(const Float4& direction) noexcept
        {
            const auto towardsBack = direction - Float4(Zero<Float>(), Zero<Float>(), One<Float>(), Zero<Float>());
            const auto m = max(Two<Float>() * length3(towardsBack), std::numeric_limits<Float>::min());
            return Float4(
                Half<Float>() - x(direction) / m, Half<Float>() + y(direction) / m, Zero<Float>(), Zero<Float>()
            );
        }

        // The same orientation as the mirror ball, with a radius of angle / (2 pi) for the angle to -z
        Float4 angularMapCoordinates(const Float4& direction) noexcept
        {
            const auto angle = vectorization::acos(clamp(-z(direction), NegativeOne<Float>(), One<Float>()));
            const auto sine =
                max(vectorization::sqrt(sqr(x(direction)) + sqr(y(direction))), std::numeric_limits<Float>::min());
            const auto scale = Half<Float>() * ReciprocalPi<Float>() * angle / sine;
            return Float4(
                Half<Float>() - x(direction) * scale, Half<Float>() + y(direction) * scale, Zero<Float>(), Zero<Float>()
            );
        }

        // The radius horizonRadius * angle / (pi / 2) for the angle to the zenith, towards (x, z) as with the dome. The
        // nadir has no such direction: it takes -z's.
        Float4 allSkyFisheyeCoordinates(const Float4& direction, const Float horizonRadius) noexcept
        {
            const auto angle = vectorization::acos(clamp(y(direction), NegativeOne<Float>(), One<Float>()));
            const auto sine = vectorization::sqrt(sqr(x(direction)) + sqr(z(direction)));
            const auto onAxis = !(sine > std::numeric_limits<Float>::min());
            const auto towardsX = onAxis ? Zero<Float>() : x(direction) / sine;
            const auto towardsZ = onAxis ? NegativeOne<Float>() : z(direction) / sine;
            const auto radius = horizonRadius * Two<Float>() * ReciprocalPi<Float>() * angle;
            return Float4(
                multiplyAdd(radius, towardsX, Half<Float>()), multiplyAdd(radius, towardsZ, Half<Float>()),
                Zero<Float>(), Zero<Float>()
            );
        }

        Float4 latitudeLongitudeCoordinates(const Float4& direction) noexcept
        {
            // vectorization's atan2 takes x first: this is std::atan2(x, -z), the azimuth from -z towards +x
            const auto azimuth = vectorization::atan2(-z(direction), x(direction));
            const auto polarAngle = vectorization::acos(clamp(y(direction), NegativeOne<Float>(), One<Float>()));
            return Float4(
                multiplyAdd(azimuth, RadianToUniform<Float>(), Half<Float>()),
                One<Float>() - polarAngle * ReciprocalPi<Float>(), Zero<Float>(), Zero<Float>()
            );
        }
    }

    Float4 environmentCoordinates(
        const EnvironmentProjection projection, const Float4& direction, const Float horizonRadius
    ) noexcept
    {
        switch (projection)
        {
        case EnvironmentProjection::MirrorBall:
            return mirrorBallCoordinates(direction);
        case EnvironmentProjection::AngularMap:
            return angularMapCoordinates(direction);
        case EnvironmentProjection::LatitudeLongitude:
            return latitudeLongitudeCoordinates(direction);
        case EnvironmentProjection::AllSkyFisheye:
            return allSkyFisheyeCoordinates(direction, horizonRadius);
        case EnvironmentProjection::FisheyeDome:
        default:
            return fisheyeDomeCoordinates(direction);
        }
    }
}
