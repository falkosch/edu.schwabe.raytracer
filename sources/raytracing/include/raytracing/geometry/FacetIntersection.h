#pragma once

#include "Intersectable.h"

#include <primitives.h>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    // coords at facet intersections
    struct FacetIntersection
    {
        // point of intersection in model space
        Float4 msVertex;

        // surface normal at intersection in model space
        Float4 msSurfaceNormal;

        // point of intersection
        Float4 vertex;

        // surface normal at intersection
        Float4 surfaceNormal;

        // smoothed normal at intersection
        Float4 smoothedNormal;

        // texture coords at intersection
        Float4 texCoords;

        // node of the facet
        const Intersectable<Raycast, FacetIntersection> * node;

        FacetIntersection();
    };
}
