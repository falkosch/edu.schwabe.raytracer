#pragma once

#include "../geometry/ObjectGeometry.h"

#include "LightInfo.h"

#include <vector>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  // per light shadowing information
  struct PerLightShadowCache {
    typedef std::vector<PerLightShadowCache, AlignedAllocator<PerLightShadowCache>> ShadowCacheType;

    // the facet, that has shadowed this light source last time.
    const Intersectable<RayCast, FacetIntersection> *lastShadowedByFacet;

    // the object, that has shadowed this light source last time.
    const ObjectGeometry *lastShadowedByObject;

    PerLightShadowCache();

    PerLightShadowCache(
        const Intersectable<RayCast, FacetIntersection> &lastShadowedByFacet, const ObjectGeometry &lastShadowedByObject
    );
  };
}
