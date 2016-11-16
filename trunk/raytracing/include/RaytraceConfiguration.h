#pragma once

#include "RaytraceParameters.h"
#include "RaytraceOutput.h"

namespace raytracer
{

    /*
    * Struct for the bundled raytrace configuration.
    */
    struct RaytraceConfiguration : RaytraceParameters, RaytraceOutput
    {
        ASizeT runId;

        ALIGNED_ALLOCATORS(__alignof(RaytraceConfiguration));

        RaytraceConfiguration();

        RaytraceConfiguration & operator=(const RaytraceParameters & parameters)
        {
            this->visibilityCutoff = parameters.visibilityCutoff;
            this->maxDistance = parameters.maxDistance;
            this->maxTraceDepth = parameters.maxTraceDepth;
            this->sceneShader = parameters.sceneShader;
            this->cullingOrientation = parameters.cullingOrientation;

            this->resolution = parameters.resolution;
            this->perspectiveZPlanes = parameters.perspectiveZPlanes;
            this->perspectiveFOV = parameters.perspectiveFOV;
            this->samplingFactor = parameters.samplingFactor;
            this->supersamplingFactor = parameters.supersamplingFactor;
            this->rayPacketSize = parameters.rayPacketSize;
            this->observer = parameters.observer;
            this->camera = parameters.camera;

            return *this;
        }
    };

}
