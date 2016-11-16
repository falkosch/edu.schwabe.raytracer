#include "../stdafx.h"

#include "shading/SceneShaderContainment.h"

namespace raytracer
{

    SceneShaderContainment::SceneShaderContainment(
        PerLightShadowCache::ShadowCacheType & shadowCacheIn,
        StatisticsCookie & statisticsIn)
        :
        incidentRay(),
        shininess(),
        visibilityCutoff(),
        shadowCache(&shadowCacheIn),
        statistics(&statisticsIn)
    { }

}
