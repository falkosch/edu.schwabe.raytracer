#pragma once

#include "RaytraceOutput.h"
#include "RaytraceParameters.h"

namespace raytracer {
  using namespace vectorization;

  /*
   * Struct for the bundled raytrace configuration.
   */
  struct RaytraceConfiguration : RaytraceParameters, RaytraceOutput {
    ASizeT runId;

    RaytraceConfiguration();

    RaytraceConfiguration &operator=(const RaytraceParameters &parameters) {
      this->visibilityCutoff = parameters.visibilityCutoff;
      this->maxDistance = parameters.maxDistance;
      this->maxTraceDepth = parameters.maxTraceDepth;
      this->sceneShader = parameters.sceneShader;
      this->cullingOrientation = parameters.cullingOrientation;

      this->resolution = parameters.resolution;
      this->perspectiveZPlanes = parameters.perspectiveZPlanes;
      this->perspectiveFOV = parameters.perspectiveFOV;
      this->samplingFactor = parameters.samplingFactor;
      this->superSamplingFactor = parameters.superSamplingFactor;
      this->rayPacketSize = parameters.rayPacketSize;
      this->observer = parameters.observer;
      this->camera = parameters.camera;

      return *this;
    }
  };
}
