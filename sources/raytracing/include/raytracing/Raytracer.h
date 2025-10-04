#pragma once

#include "shading/Raytrace.h"

#include "BRDFParameters.h"
#include "IlluminatedIntersection.h"
#include "RaytracerCache.h"

namespace raytracer {
  using namespace vectorization;

  class Raytracer final {
  public:
    Raytracer();

    ~Raytracer();

    RaytraceConfiguration getRunning() const;

    void requestUpdate();

    void trigger(const RaytraceParameters &parameters);

  private:
    volatile ASizeT runId;

    RaytraceConfiguration running;

    RaytraceConfiguration current;

    void trace();

    static Float4 applyBRDF(const BRDFParameters &brdf);

    static IlluminatedIntersection trace(const Raytrace &raytrace, RaytracerCache &cache);

    static void
    traceReflection(const Raytrace &incidentRaytrace, Float maxDistance, RaytracerCache &cache, BRDFParameters &brdf);

    static void traceTransmission(
        const Raytrace &incidentRaytrace, Float maxDistance, bool leavingMaterial, const Float4 &transmittedDirection,
        RaytracerCache &cache, BRDFParameters &brdf
    );
  };
}
