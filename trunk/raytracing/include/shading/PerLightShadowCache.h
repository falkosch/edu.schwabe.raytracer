#pragma once

#include "geometry/ObjectGeometry.h"

#include "LightInfo.h"

#include <vector>

namespace raytracer
{

    // per light shadowing information
    struct PerLightShadowCache
    {
        typedef std::vector<PerLightShadowCache, vectorization::AlignedAllocator<PerLightShadowCache>> ShadowCacheType;

        // the facet, that has shadowed this lightsource last time.
        const Intersectable<primitives::Raycast, FacetIntersection> * lastShadowedByFacet;

        // the object, that has shadowed this lightsource last time.
        const ObjectGeometry * lastShadowedByObject;

        ALIGNED_ALLOCATORS(__alignof(PerLightShadowCache));

        PerLightShadowCache();

        PerLightShadowCache(
            const Intersectable<primitives::Raycast, FacetIntersection> & lastShadowedByFacet,
            const ObjectGeometry & lastShadowedByObject);
    };

}
