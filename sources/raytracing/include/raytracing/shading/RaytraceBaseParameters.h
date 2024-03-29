#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  // forward declaration
  class SceneShader;

  /*
   * Struct for the fixed raytrace parameters.
   */
  struct RaytraceBaseParameters {
    // parameters used within raytracing
    Float visibilityCutoff;
    Float maxDistance;
    ASizeT maxTraceDepth;
    SceneShader *sceneShader;
    Int cullingOrientation;

    RaytraceBaseParameters();
  };
}
