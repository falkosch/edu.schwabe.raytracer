#pragma once

#include "geometry/SceneIntersection.h"

#include "primitives/Raycast.h"

namespace raytracer
{

    /*
    * Specializes RayCast by visibility properties
    */
    struct Raytrace
    {
        primitives::Raycast raycast;

        const SceneIntersection * originIntersection;

        vectorization::ASizeT traceDepth;

        vectorization::Float visibilityIndex;

        ALIGNED_ALLOCATORS(__alignof(Raytrace));

        Raytrace();

        explicit Raytrace(
            const primitives::Raycast & raycast,
            const SceneIntersection * const originIntersection,
            const vectorization::ASizeT traceDepth,
            const vectorization::Float visibilityIndex);

    };

}
