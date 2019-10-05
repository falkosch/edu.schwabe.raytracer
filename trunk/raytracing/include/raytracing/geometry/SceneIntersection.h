#pragma once

#include "FacetIntersection.h"
#include "ObjectGeometry.h"

namespace raytracer
{
    using namespace vectorization;

    /*
    * Struct for the handling of scene intersections
    */
    struct SceneIntersection : FacetIntersection
    {
        Float4 smoothedNdotI;

        Float4 reflectedDirection;

        const ObjectGeometry * object;

        ALIGNED_ALLOCATORS(__alignof(SceneIntersection));

        SceneIntersection();
    };
}
