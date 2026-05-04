#include "raytracing/shading/SceneShaderContainment.h"
#include "../../stdafx.h"

namespace raytracer
{
    SceneShaderContainment::SceneShaderContainment(
        PerLightShadowCache::ShadowCacheType& shadowCacheIn, StatisticsCookie& statisticsIn
    )
        : incidentRay(), F0(), roughness(), visibilityCutoff(), shadowCache(&shadowCacheIn), statistics(&statisticsIn)
    {
    }
}
