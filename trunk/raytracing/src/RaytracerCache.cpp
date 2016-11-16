#include "stdafx.h"

#include "RaytracerCache.h"

#include "shading/SceneShader.h"

namespace raytracer
{

    RaytracerCache::RaytracerCache(const RaytraceConfiguration & configurationIn)
        :
        configuration(configurationIn),
        statistics(),
        shadowCache(configurationIn.sceneShader->getLights().size())
    { }

}
