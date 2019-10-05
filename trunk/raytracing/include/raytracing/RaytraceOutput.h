#pragma once

#include "common/StatisticsCookie.h"
#include "common/HDRImage.h"

namespace raytracer
{
    /*
    * Struct for the raytrace output
    */
    struct RaytraceOutput
    {
        HDRImage * image, * timingMap, * depthMap;

        bool state;

        StatisticsCookie statistics;

        ALIGNED_ALLOCATORS(__alignof(RaytraceOutput));

        RaytraceOutput();
    };
}
