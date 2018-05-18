#include "raytracing/RaytracerCache.h"
#include "../stdafx.h"

#include "raytracing/shading/SceneShader.h"

namespace raytracer
{

    RaytracerCache::RaytracerCache(const RaytraceConfiguration & configurationIn)
        :
        configuration(configurationIn),
        statistics(),
        shadowCache(configurationIn.sceneShader->getLights().size())
    { }

}
