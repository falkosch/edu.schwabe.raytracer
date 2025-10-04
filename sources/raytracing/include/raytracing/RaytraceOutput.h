#pragma once

#include "common/HDRImage.h"
#include "common/StatisticsCookie.h"

namespace raytracer {
  /*
   * Struct for the raytrace output
   */
  struct RaytraceOutput {
    HDRImage *image, *timingMap, *depthMap;

    bool state;

    StatisticsCookie statistics;

    RaytraceOutput();
  };
}
