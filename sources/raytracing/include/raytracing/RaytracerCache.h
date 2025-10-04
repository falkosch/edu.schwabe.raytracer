#pragma once

#include "RaytraceConfiguration.h"

#include "shading/PerLightShadowCache.h"
#include "shading/Raytrace.h"

namespace raytracer {
  // per worker thread cached raytracing information
  struct RaytracerCache {
    RaytraceConfiguration configuration;

    StatisticsCookie statistics;

    PerLightShadowCache::ShadowCacheType shadowCache;

    RaytracerCache(const RaytraceConfiguration &configuration);
  };
}