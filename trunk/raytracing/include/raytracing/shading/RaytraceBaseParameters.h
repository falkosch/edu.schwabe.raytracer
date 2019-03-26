#pragma once

#include <vectorization.h>

namespace raytracer
{
	using namespace vectorization;

    // forward decleration
    class SceneShader;

    /*
    * Struct for the fixed raytrace parameters.
    */
    struct RaytraceBaseParameters
    {
        // parameters used within raytracing
        Float visibilityCutoff;
        Float maxDistance;
        ASizeT maxTraceDepth;
        SceneShader* sceneShader;
        Int cullingOrientation;

        ALIGNED_ALLOCATORS(__alignof(RaytraceBaseParameters));

        RaytraceBaseParameters();
    };

}
