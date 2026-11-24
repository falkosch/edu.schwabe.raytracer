#include "raytracing/shading/RaytraceBaseParameters.h"
#include "../../stdafx.h"

namespace raytracer
{
    RaytraceBaseParameters::RaytraceBaseParameters()
        : visibilityCutoff(), maxDistance(), maxTraceDepth(), sceneShader(), cullingOrientation(), glossySamples(16),
          sphereLightSamples(16), seedOffset(), glossyClamp(),
          regularizePaths(), adaptiveRounds(), adaptiveThreshold()
    {
    }
}