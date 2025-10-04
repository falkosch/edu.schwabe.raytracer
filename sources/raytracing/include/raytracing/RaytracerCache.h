#pragma once

#include "RaytraceConfiguration.h"

#include "shading/PerLightShadowCache.h"

namespace raytracer {
  // per worker thread cached raytracing information
  struct RaytracerCache {
    RaytraceConfiguration configuration;
    StatisticsCookie statistics;
    PerLightShadowCache::ShadowCacheType shadowCache;

    explicit RaytracerCache(const RaytraceConfiguration &configuration);
  };
}
