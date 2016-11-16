#include "../stdafx.h"

#include "shading/RaytraceBaseParameters.h"

namespace raytracer
{

    RaytraceBaseParameters::RaytraceBaseParameters()
        :
        visibilityCutoff(),
        maxDistance(),
        maxTraceDepth(),
        sceneShader(),
        cullingOrientation()
    { }

}
