#include "raytracing/shading/SceneShaderContainment.h"
#include "../../stdafx.h"

namespace raytracer {
  SceneShaderContainment::SceneShaderContainment(
      PerLightShadowCache::ShadowCacheType &shadowCacheIn, StatisticsCookie &statisticsIn
  )
      : incidentRay(), shininess(), visibilityCutoff(), shadowCache(&shadowCacheIn), statistics(&statisticsIn) {
  }
}