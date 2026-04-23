#pragma once

#include "common/HDRImage.h"
#include "common/StatisticsCookie.h"

#include <memory>

namespace raytracer
{
  /*
   * Struct for the raytrace output
   */
  struct RaytraceOutput
  {
    std::shared_ptr<HDRImage> image, timingMap, depthMap;

    bool state;

    Float_64 durationSeconds;

    StatisticsCookie statistics;

    RaytraceOutput();
  };
}
