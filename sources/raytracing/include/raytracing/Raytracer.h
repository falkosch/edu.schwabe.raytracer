#pragma once

#include "shading/Raytrace.h"

#include "BRDFParameters.h"
#include "IlluminatedIntersection.h"
#include "RaytracerCache.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace raytracer {
  using namespace vectorization;

  class Raytracer final {
  public:
    Raytracer();

    ~Raytracer();

    RaytraceConfiguration getRunning() const;

    void requestUpdate();

    void trigger(const RaytraceParameters &parameters);

    void stop();

    void enqueueWork(std::function<void()> work);

  private:
    std::atomic<ASizeT> runId;

    RaytraceConfiguration running;

    RaytraceConfiguration current;

    std::mutex mutex;

    std::condition_variable_any workAvailable;

    std::function<void()> pendingWork;

    std::jthread workerThread;

    void workerLoop(const std::stop_token& stopToken);

    void trace();

    static RGBS applyBRDF(const BRDFParameters &brdf);

    static IlluminatedIntersection trace(const Raytrace &raytrace, RaytracerCache &cache);

    static void
    traceReflection(const Raytrace &incidentRaytrace, Float maxDistance, RaytracerCache &cache, BRDFParameters &brdf);

    static void traceTransmission(
        const Raytrace &incidentRaytrace, Float maxDistance, bool leavingMaterial, const Float4 &transmittedDirection,
        RaytracerCache &cache, BRDFParameters &brdf
    );
  };
}
