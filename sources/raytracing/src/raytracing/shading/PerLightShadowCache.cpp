#include "raytracing/shading/PerLightShadowCache.h"
#include "../../stdafx.h"

namespace raytracer {
  PerLightShadowCache::PerLightShadowCache() : lastShadowedByFacet(), lastShadowedByObject() {
  }

  PerLightShadowCache::PerLightShadowCache(
      const Intersectable<RayCast, FacetIntersection> &lastShadowedByFacetIn,
      const ObjectGeometry &lastShadowedByObjectIn
  )
      : lastShadowedByFacet(&lastShadowedByFacetIn), lastShadowedByObject(&lastShadowedByObjectIn) {
  }
}