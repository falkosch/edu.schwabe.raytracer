#pragma once

#include "RaytraceConfiguration.h"

#include "shading/Raytrace.h"
#include "shading/PerLightShadowCache.h"

namespace raytracer
{
    // per worker thread cached raytracing information
    struct RaytracerCache
    {
        RaytraceConfiguration configuration;

        StatisticsCookie statistics;

        PerLightShadowCache::ShadowCacheType shadowCache;

        ALIGNED_ALLOCATORS(__alignof(RaytracerCache));

        RaytracerCache(const RaytraceConfiguration & configuration);
    };
}