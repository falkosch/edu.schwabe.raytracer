#pragma once

#include <primitives.h>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    // A ray's origin and reciprocal direction, each component broadcast to all lanes, for the wide node test.
    struct KDTreeWideRay
    {
        Float4 originX, originY, originZ, reciprocalX, reciprocalY, reciprocalZ;

        explicit KDTreeWideRay(const Ray& ray) noexcept;
    };
}
