#pragma once

#include "geometry/FacetIntersection.h"
#include "geometry/ObjectGeometry.h"

namespace raytracer
{

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
