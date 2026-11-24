#include "raytracing/shading/SceneShaderContainment.h"
#include "../../stdafx.h"

namespace raytracer
{
    SceneShaderContainment::SceneShaderContainment(
        PerLightShadowCache::ShadowCacheType& shadowCacheIn, StatisticsCookie& statisticsIn
    )
        : incidentRay(), F0(), roughness(), visibilityCutoff(), sphereLightSamples(16), glossyRays(),
          pointLightSamples(),
          shadowCache(&shadowCacheIn), statistics(&statisticsIn)
    {
    }
}
