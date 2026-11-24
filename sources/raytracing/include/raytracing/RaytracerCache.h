#pragma once

#include "RaytraceConfiguration.h"

#include "shading/PerLightShadowCache.h"

namespace raytracer {
  class IrradianceCache;
  struct IrradianceRecord;

  // per worker thread cached raytracing information
  struct RaytracerCache {
    RaytraceConfiguration configuration;
    StatisticsCookie statistics;
    PerLightShadowCache::ShadowCacheType shadowCache;

    // The irradiance cache whose records diffuse hits interpolate (RaytraceBaseParameters::irradianceCaching), and
    // while it's filled, where the primary ray of a record leaves it (radius 0 where records cover its hit already)
    const IrradianceCache *irradianceCache;
    IrradianceRecord *recording;

    explicit RaytracerCache(const RaytraceConfiguration &configuration);
  };
}
