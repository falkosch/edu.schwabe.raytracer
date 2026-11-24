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
      this->glossySamples = parameters.glossySamples;
      this->sphereLightSamples = parameters.sphereLightSamples;
      this->diffuseSamples = parameters.diffuseSamples;
      this->manyLightSamples = parameters.manyLightSamples;
      this->seedOffset = parameters.seedOffset;
      this->glossyClamp = parameters.glossyClamp;
      this->regularizePaths = parameters.regularizePaths;
      this->adaptiveRounds = parameters.adaptiveRounds;
      this->adaptiveThreshold = parameters.adaptiveThreshold;
      this->denoise = parameters.denoise;
      this->irradianceCaching = parameters.irradianceCaching;

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
