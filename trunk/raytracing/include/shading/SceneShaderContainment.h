#pragma once

#include "common/StatisticsCookie.h"

#include "PerLightShadowCache.h"
#include "Raytrace.h"

namespace raytracer
{

    struct SceneShaderContainment
    {
        Raytrace incidentRay;
        Float4 shininess;
        Float visibilityCutoff;
        PerLightShadowCache::ShadowCacheType *shadowCache;
        StatisticsCookie *statistics;

        ALIGNED_ALLOCATORS(__alignof(SceneShaderContainment));

        SceneShaderContainment(PerLightShadowCache::ShadowCacheType & shadowCache, StatisticsCookie & statistics);
    };

}
