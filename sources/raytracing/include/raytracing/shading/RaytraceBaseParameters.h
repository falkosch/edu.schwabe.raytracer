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

    // Distribution ray tracing: the most reflection rays of a glossy surface per hit, and the most shadow rays to a
    // sphere light per intersection. Fewer are traced the less they add to the pixel. 0 is Whitted ray tracing: one
    // mirror ray (rough surfaces keep their highlights), one shadow ray to the sphere light's nearest point (hard
    // shadows).
    ASizeT glossySamples;
    ASizeT sphereLightSamples;

    RaytraceBaseParameters();
  };
}
