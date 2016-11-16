#pragma once

#include <vectorization/vectorization.h>

namespace raytracer
{

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
