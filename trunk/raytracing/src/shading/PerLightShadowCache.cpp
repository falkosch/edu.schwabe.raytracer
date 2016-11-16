#include "../stdafx.h"

#include "shading/PerLightShadowCache.h"

namespace raytracer
{

    PerLightShadowCache::PerLightShadowCache()
        :
        lastShadowedByFacet(),
        lastShadowedByObject()
    { }

    PerLightShadowCache::PerLightShadowCache(
        const Intersectable<Raycast, FacetIntersection> & lastShadowedByFacetIn,
        const ObjectGeometry & lastShadowedByObjectIn)
        :
        lastShadowedByFacet(&lastShadowedByFacetIn),
        lastShadowedByObject(&lastShadowedByObjectIn)
    { }

}
