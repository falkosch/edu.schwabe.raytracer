#pragma once

#include "shading/Raytrace.h"

#include "RaytracerCache.h"
#include "BRDFParameters.h"
#include "IlluminatedIntersection.h"

namespace raytracer
{

    class Raytracer
    {
    public:

        ALIGNED_ALLOCATORS(__alignof(Raytracer));

        Raytracer();

        virtual ~Raytracer();

        void requestUpdate();

        void trigger(const RaytraceParameters & parameters);

        const RaytraceConfiguration getRunning() const { return running; }

    private:

        volatile vectorization::ASizeT runId;

        RaytraceConfiguration running;

        RaytraceConfiguration current;

        void trace();

        static const vectorization::Float4 applyBRDF(const BRDFParameters & brdf);

        static const IlluminatedIntersection trace(const Raytrace & raytrace, RaytracerCache & cache);

        static void traceReflection(
            const Raytrace & incidentRaytrace,
            const vectorization::Float maxDistance,
            RaytracerCache & cache,
            BRDFParameters & brdf);

        static void traceTransmission(
            const Raytrace & incidentRaytrace,
            const vectorization::Float maxDistance,
            const bool leavingMaterial,
            const vectorization::Float4 & transmittedDirection,
            RaytracerCache & cache,
            BRDFParameters & brdf);

    };

}
