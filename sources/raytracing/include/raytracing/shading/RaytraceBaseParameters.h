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

    // Distribution ray tracing: the most reflection rays of a glossy surface per hit, and the most shadow rays to an
    // area light (a sphere or box light) per intersection. Fewer are traced the less they add to the pixel. 0 is
    // Whitted ray tracing: one mirror ray (rough surfaces keep their highlights), one shadow ray to the area light's
    // nearest point (hard shadows).
    ASizeT glossySamples;
    ASizeT sphereLightSamples;

    // Picks the sample pattern of the distribution ray tracing: 0 is the default, other values render the same image
    // with other samples (e.g. to measure its noise). Whitted ray tracing doesn't sample and ignores it.
    UInt_32 seedOffset;

    // The most radiance one glossy reflection ray adds: a brighter one is scaled down to it, keeping its colour. It
    // keeps single rays that find a small, bright spot from dominating their pixel ("fireflies"), but biases: very
    // bright glossy reflections get darker. 0 is off.
    Float glossyClamp;

    // After a glossy reflection, the mirror reflections and refractions further along the path don't show the
    // emissive objects of area lights either (Raytrace::glossy carries on). It removes the glitter of glossy -> glass
    // or mirror -> light paths, but biases: the light along them goes missing.
    bool regularizePaths;

    RaytraceBaseParameters();
  };
}
