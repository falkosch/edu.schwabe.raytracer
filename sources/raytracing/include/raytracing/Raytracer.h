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

namespace raytracer
{
    using namespace vectorization;

    class Raytracer final
    {
    public:
        Raytracer();

        ~Raytracer();

        RaytraceConfiguration getRunning() const;

        void requestUpdate();

        // Requests a render with the parameters, replacing any requested before; returns the id of its run
        // (RaytraceConfiguration::runId)
        ASizeT trigger(const RaytraceParameters& parameters);

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

        static IlluminatedIntersection trace(const Raytrace& raytrace, RaytracerCache& cache);

        // microfacetEta: the refraction indices of a rough dielectric interface (incident side first), whose lobes
        // weigh each microfacet by its own Fresnel reflectance; nullptr for the smooth surface's
        static void traceGlossyReflection(
            const Raytrace& incidentRaytrace, Float maxDistance, Float reflectionVisibilityIndex,
            Float reflectionImportance, Float roughness, const Float4* microfacetEta, RaytracerCache& cache,
            BRDFParameters& brdf
        );

        static void traceReflection(
            const Raytrace& incidentRaytrace, Float maxDistance, const Float4* microfacetEta, RaytracerCache& cache,
            BRDFParameters& brdf
        );

        static void traceTransmission(
            const Raytrace& incidentRaytrace, Float maxDistance, bool leavingMaterial,
            const Float4& transmittedDirection, const Float4* microfacetEta, RaytracerCache& cache,
            BRDFParameters& brdf
        );

        static void traceRoughTransmission(
            const Raytrace& incidentRaytrace, Float maxDistance, bool leavingMaterial, const Float4& interfaceEta,
            Float transmissionVisibilityIndex, Float transmissionImportance, RaytracerCache& cache,
            BRDFParameters& brdf
        );
    };
}
