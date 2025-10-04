#pragma once

#include "shading/RaytraceBaseParameters.h"

#include "scene/Camera.h"

#include "RaytraceObserver.h"

namespace raytracer {
  using namespace vectorization;

  /*
   * Struct for the fixed raytrace parameters.
   */
  struct RaytraceParameters : RaytraceBaseParameters {
    Size2 resolution;

    // x is near-z-plane, y is far-z-plane
    Float2 perspectiveZPlanes;

    Float perspectiveFOV;

    Float samplingFactor;

    // Sets the anti-aliasing ray count
    // is 0 == "1x1 sampling"
    // is 1 == "2x2 sampling"
    // is 2 == "3x3 sampling"
    // ...
    ASizeT superSamplingFactor;

    // rectangular patches of primary rays are packed together
    // is 0 == "1x1 packets"
    // is 1 == "2x2 packets"
    // is 2 == "3x3 packets"
    // ...
    ASizeT rayPacketSize;

    RaytraceObserver *observer;

    Camera *camera;

    RaytraceParameters();
  };
}
