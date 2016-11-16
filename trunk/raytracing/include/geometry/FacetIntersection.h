#pragma once

#include "geometry/Intersectable.h"

#include <primitives/Raycast.h>

namespace raytracer
{

    // coords at facet intersections
    struct FacetIntersection
    {
        // point of intersection in model space
        vectorization::Float4 msVertex;

        // surface normal at intersection in model space
        vectorization::Float4 msSurfaceNormal;

        // point of intersection
        vectorization::Float4 vertex;

        // surface normal at intersection
        vectorization::Float4 surfaceNormal;

        // smoothed normal at intersection
        vectorization::Float4 smoothedNormal;

        // texture coords at intersection
        vectorization::Float4 texCoords;

        // node of the facet
        const Intersectable<primitives::Raycast, FacetIntersection>* node;

        ALIGNED_ALLOCATORS(__alignof(FacetIntersection));

        FacetIntersection();
    };

}
