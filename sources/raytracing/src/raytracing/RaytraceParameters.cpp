#include "raytracing/RaytraceParameters.h"
#include "../stdafx.h"

namespace raytracer {
  RaytraceParameters::RaytraceParameters()
      : RaytraceBaseParameters(), resolution(), perspectiveZPlanes(), perspectiveFOV(), samplingFactor(),
        superSamplingFactor(), rayPacketSize(), observer(), camera() {
  }
}