#pragma once

#include "../common/StatisticsCookie.h"

#include "PerLightShadowCache.h"
#include "Raytrace.h"

namespace raytracer
{
	using namespace vectorization;

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
