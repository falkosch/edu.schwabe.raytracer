#pragma once

#include "common/HDRImage.h"
#include "common/StatisticsCookie.h"

namespace raytracer
{
  /*
   * Struct for the raytrace output
   */
  struct RaytraceOutput
  {
    HDRImage *image, *timingMap, *depthMap;

    bool state;

    Float_64 durationSeconds;

    StatisticsCookie statistics;

    RaytraceOutput();
  };
}
