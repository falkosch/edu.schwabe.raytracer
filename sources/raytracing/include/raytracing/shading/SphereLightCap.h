#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct LightInfo;

    // The cap of a sphere light (LightInfo::radius) that a point sees, the part within acos(radius / distance) of the
    // side facing the point, and the points its shadow rays go to: points spread evenly over the disk that the cap
    // projects to along the direction to the center, lifted onto the cap.
    struct SphereLightCap
    {
        Float4 origin;
        Float4 center;
        Float radius;

        // a basis with w towards the light's center
        Float4 u, v, w;
        Float centerDistance;

        // the radius of the disk the cap projects to, in units of the light's radius: the sine of the cap's angle
        Float visibleCap;

        SphereLightCap(const LightInfo& light, const Float4& point) noexcept;

        // The point of the cap for xi in [0, 1)^2 (x, y), relative to the point: equal areas of xi get equal areas of the
        // disk. xi = 0 gives the cap's center, the light's point closest to the point.
        Float4 samplePoint(const Float4& xi) const noexcept;

        // The density per solid angle of the directions to the points samplePoint gives, at a point of the cap relative
        // to the point. 0 for a point off the cap; it grows towards the cap's rim, where the sphere is seen edge-on, and
        // stays finite there.
        Float solidAnglePdf(const Float4& toPoint) const noexcept;
    };
}
