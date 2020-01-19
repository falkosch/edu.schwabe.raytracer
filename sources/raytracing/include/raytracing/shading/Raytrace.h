#pragma once

#include "../geometry/SceneIntersection.h"

#include <primitives.h>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    /*
    * Specializes RayCast by visibility properties
    */
    struct Raytrace
    {
        Raycast raycast;

        const SceneIntersection * originIntersection;

        ASizeT traceDepth;

        Float visibilityIndex;

        Raytrace();

        explicit Raytrace(
            const Raycast & raycast,
            const SceneIntersection * const originIntersection,
            const ASizeT traceDepth,
            const Float visibilityIndex);
    };
}
