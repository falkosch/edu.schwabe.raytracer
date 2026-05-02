#include "raytracing/Raytracer.h"
#include "../stdafx.h"

#include "raytracing/RaytracerPackets.h"

#include "raytracing/shading/ObjectShader.h"

#include "raytracing/common/StatisticsCookie.h"
#include "raytracing/common/Tools.h"

#include <cassert>
#include <limits>
#include <logging.h>
#include <omp.h>
#include <sstream>
#include <profileapi.h>

static const auto Log = logging::scope("Raytracer");

namespace raytracer
{
    Raytracer::Raytracer()
        : runId(0), running(), current(), mutex(), workAvailable(),
          workerThread([this](std::stop_token stopToken) { workerLoop(stopToken); })
    {
    }

    Raytracer::~Raytracer()
    {
        stop();
    }

    void Raytracer::stop()
    {
        if (workerThread.joinable())
        {
            workerThread.request_stop();
            workAvailable.notify_all();
            workerThread.join();
        }
    }

    void Raytracer::enqueueWork(std::function<void()> work)
    {
        std::lock_guard lock(mutex);
        pendingWork = std::move(work);
    }

    RaytraceConfiguration Raytracer::getRunning() const
    {
        return running;
    }

    void Raytracer::requestUpdate()
    {
        // No-op: rendering is now driven by the background worker thread.
        // Kept for interface compatibility.
    }

    void Raytracer::workerLoop(std::stop_token stopToken)
    {
        while (!stopToken.stop_requested())
        {
            {
                std::unique_lock lock(mutex);
                workAvailable.wait(lock, stopToken, [this]
                {
                    return current.runId != running.runId;
                });
            }
            if (stopToken.stop_requested()) break;

            {
                std::function<void()> work;
                {
                    std::lock_guard lock(mutex);
                    work = std::move(pendingWork);
                    pendingWork = nullptr;
                }
                if (work) work();
            }

            {
                std::lock_guard lock(mutex);
                running = current;
            }
            running.image = std::make_shared<HDRImage>(running.resolution);
            running.depthMap = std::make_shared<HDRImage>(running.resolution);
            running.timingMap = std::make_shared<HDRImage>(running.resolution);

            LARGE_INTEGER frequency, start, stop;
            QueryPerformanceFrequency(&frequency);

            Log.info([id = running.runId, rx = x(running.resolution), ry = y(running.resolution)] {
                return "Raytrace " + std::to_string(id) + " (" + std::to_string(rx) + "x" + std::to_string(ry) + "):";
            });

            QueryPerformanceCounter(&start);
            trace();
            QueryPerformanceCounter(&stop);

            const auto timeDuration = static_cast<Int_64>(stop.QuadPart - start.QuadPart);
            const auto timeFrequency = static_cast<Int_64>(frequency.QuadPart);
            running.durationSeconds = convert<Float_64>(timeDuration) / convert<Float_64>(timeFrequency);
            Log.info([d = running.durationSeconds] {
                std::ostringstream oss;
                oss << "Duration: " << d << "s";
                return oss.str();
            });

            running.observer->notifyUpdate(running);
        }
    }

    void Raytracer::trigger(const RaytraceParameters& parameters)
    {
        assert(parameters.observer);
        assert(parameters.camera);
        assert(parameters.sceneShader);

        const auto newRunId = ++runId;

        const auto samplingResolution =
            max(One<Size2>(), convert<Size2>(convert<Float4>(parameters.resolution) * parameters.samplingFactor));

        {
            std::lock_guard lock(mutex);

            // build raytrace configuration
            current = parameters;
            current.maxDistance =
                select(parameters.maxDistance > Zero<Float>(), parameters.maxDistance,
                       std::numeric_limits<Float>::max());
            current.resolution = max(One<Size2>(), samplingResolution);
            current.state = false;
            current.runId = newRunId;
        }
        workAvailable.notify_one();
    }

    Int_64 perPixelTiming()
    {
        LARGE_INTEGER start;
        QueryPerformanceCounter(&start);
        return static_cast<Int_64>(start.QuadPart);
    }

    // Timing for each pixel: Read end-time and calculate differences
    Float4 perPixelTiming(const Int_64 start)
    {
        LARGE_INTEGER stop;
        QueryPerformanceCounter(&stop);
        const auto timeStop = static_cast<Int_64>(stop.QuadPart);
        return Float4(vectorization::log(convert<Float4::ValueType>(timeStop - start + Int_64{1})));
    }

    struct PackedRaytrace
    {
        typedef std::vector<PackedRaytrace, AlignedAllocator<PackedRaytrace>> ListType;

        Raytrace raytrace;
        Float4* outputPixel;

        PackedRaytrace() : raytrace(), outputPixel()
        {
        }

        PackedRaytrace(const Raytrace& raytraceIn, Float4* const outputPixelIn)
            : raytrace(raytraceIn), outputPixel(outputPixelIn)
        {
        }
    };

    PackedRaytrace::ListType constructPackedRaytracesList(
        const int packetIndex, const RaytracerPackets& packets, const RaytraceConfiguration& configuration
    )
    {
        auto nearPlanePixel = packets.packetStartOf(packetIndex);
        auto minPacketLength = packets.clampPacketLength(configuration.resolution, nearPlanePixel);

        // iterate over camera-pixels cp in packet
        auto pixelsCount = RaytracerPackets::packetPixelsCount(minPacketLength);
        auto subSamplesCount = packets.pixelSubSamplesCount();

        auto list = PackedRaytrace::ListType();
        list.reserve(pixelsCount * subSamplesCount);

        for (auto cp = Zero<Size2::ValueType>(); cp < pixelsCount; cp++)
        {
            // address camera-point cp to output pixel
            auto cpXY = RaytracerPackets::coordsOfPixel(cp, nearPlanePixel, minPacketLength);
            auto cpXYf = convert<Float4>(cpXY);
            auto nearTL = packets.pixelNearTopLeft(cpXYf);
            auto farTL = packets.pixelFarTopLeft(cpXYf);
            auto imageIndex = RaytracerPackets::imageIndexOfPixel(cpXY, configuration.image->getResolution());
            auto outputPixel = &configuration.image->getData()[imageIndex];

            // generate super sampling rays
            for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount;)
            {
                // interpolate the camera-point's top-left on the view-plane
                auto newRay = packets.setupRayOfSampleInPixel(s++, nearTL, farTL);
                auto newRayCast = RayCast(
                    newRay, cullingOrientationToMask(configuration.cullingOrientation), Zero<Size2>(),
                    configuration.maxDistance
                );
                auto newRaytrace = Raytrace(newRayCast, nullptr, Zero<ASizeT>(), One<Float>());
                list.emplace_back(newRaytrace, outputPixel);
            }
        }

        return list;
    }

    /**
     * Shoots the primary rays. It's better to not unproject every camera-pixel, but only the four corners. So we only
     * have to bi-linear interpolate between the corner directions while iterating. This eliminates many expensive
     * transformations and other calculations.
     */
    void Raytracer::trace()
    {
        auto packets = RaytracerPackets(running);

#pragma omp parallel
        {
            // per worker states
            auto cache = RaytracerCache(running);
            StatisticsCookie::current = &cache.statistics;

            // iterate over packets
#pragma omp for schedule(static, 1) nowait
            for (int p = Zero<int>(); p < packets.getPacketCount(); ++p)
            {
                auto packedRaytraces = constructPackedRaytracesList(p, packets, cache.configuration);
                cache.statistics.primaryRays += packedRaytraces.size();

                for (const auto& packedRaytrace : packedRaytraces)
                {
                    // Timing for each pixel: Read start-time
                    auto start = perPixelTiming();

                    auto hit = trace(packedRaytrace.raytrace, cache);
                    cache.statistics.missedPrimaryRays +=
                        static_cast<ASizeT>(outOfReach(packedRaytrace.raytrace.rayCast, x(hit.depth)));

                    // Sample colour into output
                    packets.samplePixel(packedRaytrace.outputPixel, hit.color.value);

                    auto imagePtrIndex = packedRaytrace.outputPixel - cache.configuration.image->getData();

                    // Fill depth information and store it in depthMap
                    auto pixelDepthMask = x_yzw(Infinity<Float4>(), NegativeInfinity<Float4>());
                    auto depth = packets.superSampledPixelDepth(
                        RaytracerPackets::samplePixelDepth(pixelDepthMask, hit.depth));
                    store(depth, cache.configuration.depthMap->getData() + imagePtrIndex);

                    // Store timings
                    store(perPixelTiming(start), cache.configuration.timingMap->getData() + imagePtrIndex);
                }
            } // p

#pragma omp critical
            {
                // merge per worker statistics to one global statistic
                running.statistics.merge(cache.statistics);
            }
            StatisticsCookie::current = nullptr;
        }

        running.state = (running.runId == runId.load());
        running.timingMap->normalizeEachChannel();
        running.depthMap->normalizeEachChannel();
    }

    Float4 schlickFresnel(const bool enteringLessDense, const Float4& negNdotI, const Float4& eta)
    {
        // calculate cosPhiT from Snell's law (TIR is already excluded)
        Float4 cosPhi;
        if (enteringLessDense)
        {
            auto etaItoT = xxxx(eta);
            auto etaItoTMulNdotI = etaItoT * negNdotI;
            auto cosSqrPhiT = One<v_f32_4>() - multiplySub(etaItoT, etaItoT, etaItoTMulNdotI * etaItoTMulNdotI);
            assert(!isNegative(cosSqrPhiT));
            cosPhi = sqrt(cosSqrPhiT);
        }
        else
        {
            cosPhi = max(Zero<Float4>(), negNdotI);
        }

        // Schlick approximation for polarized light refraction/reflection model:
        // R0 = ((eta_i - eta_t) / (eta_i + eta_t))²
        auto refractions = addSubtract(zzzz(eta), wwww(eta));
        auto sqrtR0 = xxxx(refractions) / yyyy(refractions);
        auto R0 = sqrtR0 * sqrtR0;
        auto oneSubCosPhi = One<Float4>() - cosPhi;
        auto sqrOneSubCosPhi = oneSubCosPhi * oneSubCosPhi;
        auto reflectance = multiplyAdd(One<Float4>() - R0, oneSubCosPhi * sqrOneSubCosPhi * sqrOneSubCosPhi, R0);
        return reflectance;
    }

    Float4 fresnelReflectance(const bool totalInternalReflection, const Float4& negNdotI, const Float4& eta)
    {
        // entering less dense material and TIR is the case
        const auto enteringLessDense = !!isNegative(wwww(eta) - zzzz(eta));
        if (totalInternalReflection & enteringLessDense)
        {
            return One<Float4>();
        }
        return schlickFresnel(enteringLessDense, negNdotI, eta);
    }

    // checks intersections, applies lighting and texturing and shoots secondary rays
    IlluminatedIntersection Raytracer::trace(const Raytrace& raytrace, RaytracerCache& cache)
    {
        BRDFParameters brdf;

        // Find the nearest intersection
        const auto intersectionStart = __rdtsc();
        brdf.viewDistance = cache.configuration.sceneShader->findNearestIntersection(
            raytrace.rayCast, raytrace.originIntersection, brdf.intersection
        );
        cache.statistics.intersectionTicks += __rdtsc() - intersectionStart;

        if (outOfReach(raytrace.rayCast, brdf.viewDistance))
        {
            return {
                raytrace.rayCast.maxDistance,
                cache.configuration.sceneShader->sampleBackground(raytrace.rayCast.ray.direction)
            };
        }

        // Sample surface properties and lighting at the intersection
        const auto shadingStart = __rdtsc();

        assert(brdf.intersection.object);
        auto& objectShader = *dynamic_cast<const ObjectShader*const>(brdf.intersection.object);
        brdf.surface = objectShader(*cache.configuration.sceneShader, brdf.intersection);

        brdf.lighting = cache.configuration.sceneShader->sampleLighting(
            raytrace, SceneShader::adaptedVisibilityCutoff(cache.configuration.visibilityCutoff,
                                                           raytrace.visibilityIndex),
            brdf.surface.shininess, brdf.intersection, cache.shadowCache, cache.statistics
        );

        cache.statistics.shadingTicks += __rdtsc() - shadingStart;

        // Whitted raytracing part, reflection and transmission
        const auto transmittedDirection = refract(
            raytrace.rayCast.cullingMask, raytrace.rayCast.ray.direction, brdf.intersection.smoothedNormal,
            brdf.intersection.smoothedNdotI, brdf.surface.refractionEta
        );

        const auto leavingMaterial = frontfaceCulled(raytrace.rayCast)
            || (notCulled(raytrace.rayCast) && isNegative(-brdf.intersection.smoothedNdotI));

        const auto totalInternalReflection = allTrue3(!transmittedDirection);
        brdf.reflectanceCoefficient = fresnelReflectance(
            totalInternalReflection,
            select(leavingMaterial, brdf.intersection.smoothedNdotI, -brdf.intersection.smoothedNdotI),
            select(leavingMaterial, yxwz(brdf.surface.refractionEta), brdf.surface.refractionEta)
        );

        const auto maxDistance = raytrace.rayCast.maxDistance - brdf.viewDistance;
        traceReflection(raytrace, maxDistance, cache, brdf);
        traceTransmission(raytrace, maxDistance, leavingMaterial, transmittedDirection, cache, brdf);

        return {brdf.viewDistance, applyBRDF(brdf)};
    }

    void Raytracer::traceReflection(
        const Raytrace& incidentRaytrace, const Float maxDistance, RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        // would still be visible but tracing is not wanted any more
        if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>())
        {
            brdf.lighting.reflected = Zero<Float4>();
            return;
        }

        // check whether it would even make a difference in the image
        auto reflectionVisibilityIndex =
            incidentRaytrace.visibilityIndex * brdf.reflectanceCoefficient.value * max3v(brdf.surface.reflectance.value);
        if (x(reflectionVisibilityIndex) < cache.configuration.visibilityCutoff)
        {
            brdf.lighting.reflected = Zero<Float4>();
            return;
        }

        auto reflectedRay = Ray(brdf.intersection.vertex, brdf.intersection.reflectedDirection);
        auto reflectedRayCast = RayCast(
            reflectedRay, incidentRaytrace.rayCast.cullingMask,
            Size2(reinterpret_cast<ASizeT>(brdf.intersection.object), reinterpret_cast<ASizeT>(brdf.intersection.node)),
            maxDistance
        );
        auto reflectedRaytrace = Raytrace(
            reflectedRayCast, &brdf.intersection, incidentRaytrace.traceDepth + One<ASizeT>(),
            x(reflectionVisibilityIndex)
        );
        auto reflectedHit = trace(reflectedRaytrace, cache);

        brdf.lighting.reflected = reflectedHit.color;

        cache.statistics.secondaryRays += One<ASizeT>();
        cache.statistics.missedSecondaryRays +=
            static_cast<ASizeT>(outOfReach(reflectedRaytrace.rayCast, x(reflectedHit.depth)));
    }

    void Raytracer::traceTransmission(
        const Raytrace& incidentRaytrace, const Float maxDistance, const bool leavingMaterial,
        const Float4& transmittedDirection, RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        // would still be visible, but tracing is not wanted any more, set transmitted to the background
        if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>())
        {
            brdf.lighting.transmitted = Zero<Float4>();
            return;
        }

        Float4 fractionTransmitted;
        if (leavingMaterial)
        {
            // world vacuum does not absorb
            fractionTransmitted = One<Float4>();
        }
        else
        {
            // least possible transmitted fraction of material
            auto minDepth = Float4(std::numeric_limits<Float4::ValueType>::min());
            auto maxTransmittance = max3v(brdf.surface.transmittance);
            fractionTransmitted = vectorization::exp(-minDepth / maxTransmittance);
        }
        brdf.fractionTransmitted = fractionTransmitted;

        // check whether it would even make a difference in the image,
        // or whether it is a total internal reflection (transmissionDirection = 0)
        auto transmissionVisibilityIndex =
            Float4(incidentRaytrace.visibilityIndex) * fractionTransmitted * (One<Float4>() - brdf.
                reflectanceCoefficient.value);
        if (x(transmissionVisibilityIndex) < cache.configuration.visibilityCutoff)
        {
            brdf.lighting.transmitted = Zero<Float4>();
            return;
        }

        auto refractedRay = Ray(brdf.intersection.vertex, transmittedDirection);
        auto refractedRayCast = RayCast(
            refractedRay, cullingOrientationToMask(-cullingOrientation(incidentRaytrace.rayCast)),
            Size2(reinterpret_cast<ASizeT>(brdf.intersection.object), reinterpret_cast<ASizeT>(brdf.intersection.node)),
            maxDistance
        );
        auto refractedRaytrace = Raytrace(
            refractedRayCast, &brdf.intersection, incidentRaytrace.traceDepth + One<ASizeT>(),
            x(transmissionVisibilityIndex)
        );

        auto refractedHit = trace(refractedRaytrace, cache);
        brdf.lighting.transmitted = refractedHit.color;
        // now that we have the transmission distance through the material, recompute the fractionTransmitted coefficient
        if (!leavingMaterial)
        {
            brdf.fractionTransmitted = vectorization::exp(-refractedHit.depth / brdf.surface.transmittance);
        }

        cache.statistics.secondaryRays += One<ASizeT>();
        cache.statistics.missedSecondaryRays +=
            static_cast<ASizeT>(outOfReach(refractedRaytrace.rayCast, x(refractedHit.depth)));
    }

    RGBS Raytracer::applyBRDF(const BRDFParameters& brdf)
    {
        // https://en.wikipedia.org/wiki/Phong_reflection_model
        const auto ambient = brdf.surface.diffusion * brdf.lighting.ambient;
        const auto diffuse = brdf.surface.diffusion * brdf.lighting.diffuse;
        const auto specularReflection = brdf.surface.specular * brdf.lighting.specular;
        const auto phong = ambient + diffuse + specularReflection;

        // Transmittance model: Absorption coefficient/Beer-Lambert-law
        // http://tog.acm.org/resources/RTNews/html/rtnv10n1.html#art3
        // http://en.wikipedia.org/wiki/Absorption_coefficient
        // http://en.wikipedia.org/wiki/Beer%E2%80%93Lambert_law
        const auto reflection = brdf.surface.reflectance * brdf.lighting.reflected;
        const auto transmitted = brdf.fractionTransmitted * brdf.lighting.transmitted;

        return brdf.surface.emittance + phong + mix(transmitted.value, reflection.value, brdf.reflectanceCoefficient.value);
    }
}
