#include "stdafx.h"

#include "RaytraceParameters.h"

namespace raytracer
{

    RaytraceParameters::RaytraceParameters()
        :
        RaytraceBaseParameters(),
        resolution(),
        perspectiveZPlanes(),
        perspectiveFOV(),
        samplingFactor(),
        supersamplingFactor(),
        rayPacketSize(),
        observer(),
        camera()
    { }

}