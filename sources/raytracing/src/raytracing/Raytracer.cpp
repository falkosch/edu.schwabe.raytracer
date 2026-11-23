#include "raytracing/Raytracer.h"
#include "../stdafx.h"

#include "raytracing/RaytracerPackets.h"

#include "raytracing/shading/ObjectShader.h"
#include "raytracing/shading/SpectralContext.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/sampling.h"
#include "raytracing/shading/spectral/conversion.h"
#include "raytracing/shading/spectral/spectrum.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include "raytracing/common/StatisticsCookie.h"

#include <cassert>
#include <limits>
#include <logging.h>
#include <profileapi.h>

static const auto Log = logging::scope("Raytracer");

namespace raytracer
{
    namespace
    {
        // Seeds of the secondary rays: the transmitted one and the sample-th reflected one (traceGlossyReflection
        // derives the same seeds 8 at a time with deriveSeeds)
        constexpr UInt_32 TransmissionSeed = 0u;

        UInt_32 reflectionSeed(const UInt_32 seed, const ASizeT sample)
        {
            return deriveSeed(seed, static_cast<UInt_32>(sample) + One<UInt_32>());
        }

        // The seed of a pixel's sub-sample; seedOffset 0 keeps the default pattern (sub-samples stay below the stride)
        UInt_32 primarySeed(const ASizeT imageIndex, const ASizeT subSample, const UInt_32 seedOffset)
        {
            constexpr UInt_32 SeedOffsetStride = 0x10000u;
            return deriveSeed(
                static_cast<UInt_32>(imageIndex), static_cast<UInt_32>(subSample) + seedOffset * SeedOffsetStride
            );
        }

        // Smith's masking function G1 of the GGX distribution for one direction
        Float smithG1(const Float cosine, const Float alpha)
        {
            const auto a2 = sqr(alpha);
            const auto root = vectorization::sqrt(multiplyAdd(One<Float>() - a2, sqr(cosine), a2));
            return Two<Float>() * cosine / (cosine + root);
        }
    }

    Raytracer::Raytracer()
        : runId(0), running(), current(), mutex(), workAvailable(),
          workerThread([this](const std::stop_token& stopToken) { workerLoop(stopToken); })
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

    void Raytracer::workerLoop(const std::stop_token& stopToken)
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

            Log.info([id = running.runId, rx = x(running.resolution), ry = y(running.resolution)]
            {
                return "Raytrace " + std::to_string(id) + " (" + std::to_string(rx) + "x" + std::to_string(ry) + "):";
            });

            QueryPerformanceCounter(&start);
            trace();
            QueryPerformanceCounter(&stop);

            const auto timeDuration = static_cast<Int_64>(stop.QuadPart - start.QuadPart);
            const auto timeFrequency = static_cast<Int_64>(frequency.QuadPart);
            running.durationSeconds = convert<Float_64>(timeDuration) / convert<Float_64>(timeFrequency);
            Log.info([d = running.durationSeconds] { return "Duration: " + std::to_string(d) + "s"; });

            running.observer->notifyUpdate(running);
        }
    }

    ASizeT Raytracer::trigger(const RaytraceParameters& parameters)
    {
        assert(parameters.observer);
        assert(parameters.camera);
        assert(parameters.sceneShader);

        const auto newRunId = ++runId;
        {
            const auto samplingResolution =
                max(One<Size2>(), convert<Size2>(convert<Float4>(parameters.resolution) * parameters.samplingFactor));
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
        return newRunId;
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

    using SpectralContexts = std::vector<SpectralContext, AlignedAllocator<SpectralContext>>;

    // Every ray of a path keeps the hero wavelength of its primary ray, which depends only on the pixel sub-sample.
    // So a frame needs one SpectralContext per sub-sample, shared by all pixels.
    SpectralContexts createSpectralContexts(const SceneShader& sceneShader, const Size2::ValueType subSamplesCount)
    {
        auto contexts = SpectralContexts();
        contexts.reserve(subSamplesCount);
        for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount; ++s)
        {
            const auto heroLambda = spectral::wrapToVisible(
                spectral::LAMBDA_MIN
                + (static_cast<Float>(s) + Half<Float>())
                / static_cast<Float>(subSamplesCount) * spectral::LAMBDA_RANGE
            );
            contexts.push_back(sceneShader.createSpectralContext(heroLambda));
        }
        return contexts;
    }

    PackedRaytrace::ListType constructPackedRaytracesList(
        const int packetIndex, const RaytracerPackets& packets, const RaytraceConfiguration& configuration,
        const SpectralContexts& spectralContexts
    )
    {
        const auto nearPlanePixel = packets.packetStartOf(packetIndex);
        const auto minPacketLength = packets.clampPacketLength(configuration.resolution, nearPlanePixel);

        // iterate over camera-pixels cp in packet
        const auto pixelsCount = RaytracerPackets::packetPixelsCount(minPacketLength);
        const auto subSamplesCount = packets.pixelSubSamplesCount();

        auto list = PackedRaytrace::ListType();
        list.reserve(pixelsCount * subSamplesCount);

        for (auto cp = Zero<Size2::ValueType>(); cp < pixelsCount; cp++)
        {
            // address camera-point cp to output pixel
            const auto cpXY = RaytracerPackets::coordsOfPixel(cp, nearPlanePixel, minPacketLength);
            const auto cpXYf = convert<Float4>(cpXY);
            const auto nearTL = packets.pixelNearTopLeft(cpXYf);
            const auto farTL = packets.pixelFarTopLeft(cpXYf);
            const auto imageIndex = RaytracerPackets::imageIndexOfPixel(cpXY, configuration.image->getResolution());
            const auto outputPixel = &configuration.image->getData()[imageIndex];

            // generate super sampling rays
            for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount; ++s)
            {
                // interpolate the camera-point's top-left on the view-plane
                const auto newRay = packets.setupRayOfSampleInPixel(s, nearTL, farTL);
                const auto newRayCast = RayCast(
                    newRay, cullingOrientationToMask(configuration.cullingOrientation), Zero<Size2>(),
                    configuration.maxDistance
                );
                auto newRaytrace = Raytrace(
                    newRayCast, nullptr, Zero<ASizeT>(), One<Float>(), &spectralContexts[s],
                    primarySeed(imageIndex, s, configuration.seedOffset)
                );
                // the sub-samples of a pixel continue one sample sequence, the one of its first sub-sample
                newRaytrace.strataSeed = primarySeed(imageIndex, Zero<ASizeT>(), configuration.seedOffset);
                newRaytrace.stratum = static_cast<UInt_32>(s);
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
        const auto packets = RaytracerPackets(running);
        const auto spectralContexts = createSpectralContexts(*running.sceneShader, packets.pixelSubSamplesCount());

#pragma omp parallel
        {
            // per worker states
            auto cache = RaytracerCache(running);
            StatisticsCookie::current = &cache.statistics;

            // iterate over packets
#pragma omp for schedule(static, 1) nowait
            for (int p = Zero<int>(); p < packets.getPacketCount(); ++p)
            {
                const auto packedRaytraces = constructPackedRaytracesList(
                    p, packets, cache.configuration, spectralContexts
                );
                cache.statistics.primaryRays += packedRaytraces.size();

                for (const auto& packedRaytrace : packedRaytraces)
                {
                    // Timing for each pixel: Read start-time
                    const auto start = perPixelTiming();

                    const auto hit = trace(packedRaytrace.raytrace, cache);
                    cache.statistics.missedPrimaryRays +=
                        static_cast<ASizeT>(outOfReach(packedRaytrace.raytrace.rayCast, x(hit.depth)));

                    const auto xyz = spectral::spectrumToXYZ(hit.color, packedRaytrace.raytrace.spectralContext->xyz);
                    const auto displayRGB = spectral::xyzToDisplayRGB(xyz);
                    // Sample colour into output
                    packets.samplePixel(packedRaytrace.outputPixel, displayRGB.value);

                    const auto imagePtrIndex = packedRaytrace.outputPixel - cache.configuration.image->getData();

                    // Fill depth information and store it in depthMap
                    const auto pixelDepthMask = x_yzw(Infinity<Float4>(), NegativeInfinity<Float4>());
                    const auto depth = packets.superSampledPixelDepth(
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
            const auto etaItoT = xxxx(eta);
            const auto etaItoTMulNdotI = etaItoT * negNdotI;
            const auto cosSqrPhiT = One<v_f32_4>() - multiplySub(etaItoT, etaItoT, etaItoTMulNdotI * etaItoTMulNdotI);
            assert(!isNegative(cosSqrPhiT));
            cosPhi = sqrt(cosSqrPhiT);
        }
        else
        {
            cosPhi = max(Zero<Float4>(), negNdotI);
        }

        // Schlick approximation for polarized light refraction/reflection model:
        // R0 = ((eta_i - eta_t) / (eta_i + eta_t))²
        const auto refractions = addSubtract(zzzz(eta), wwww(eta));
        const auto sqrtR0 = xxxx(refractions) / yyyy(refractions);
        const auto R0 = sqrtR0 * sqrtR0;
        const auto oneSubCosPhi = One<Float4>() - cosPhi;
        const auto sqrOneSubCosPhi = oneSubCosPhi * oneSubCosPhi;
        const auto reflectance = multiplyAdd(One<Float4>() - R0, oneSubCosPhi * sqrOneSubCosPhi * sqrOneSubCosPhi, R0);
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
                cache.configuration.sceneShader->sampleBackground(
                    raytrace.rayCast.ray.direction, *raytrace.spectralContext
                )
            };
        }

        // Sample surface properties and lighting at the intersection
        const auto shadingStart = __rdtsc();

        assert(brdf.intersection.object);
        const auto& objectShader = *dynamic_cast<const ObjectShader*const>(brdf.intersection.object);

        // a glossy reflection gets an area light by its highlight: the light's emissive object would add it twice
        if (raytrace.glossy && objectShader.isLightEmitter())
        {
            return {brdf.viewDistance, spectral::Spectrum::zero()};
        }
        brdf.surface = objectShader.shade(brdf.intersection, *raytrace.spectralContext);

        const auto adapted_visibility_cutoff = SceneShader::adaptedVisibilityCutoff(
            cache.configuration.visibilityCutoff, raytrace.importance);
        brdf.lighting = cache.configuration.sceneShader->sampleLighting(
            raytrace, adapted_visibility_cutoff, x(brdf.surface.roughness), brdf.surface.specular.data,
            brdf.intersection, cache.configuration.sphereLightSamples, cache.shadowCache, cache.statistics
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
        const auto scalarReflectance = fresnelReflectance(
            totalInternalReflection,
            select(leavingMaterial, brdf.intersection.smoothedNdotI, -brdf.intersection.smoothedNdotI),
            select(leavingMaterial, yxwz(brdf.surface.refractionEta), brdf.surface.refractionEta)
        );
        const auto iorReflectance = Float8(scalarReflectance);
        if (spectral::spectralMax(brdf.surface.transmittance) <= Zero<Float>())
        {
            const auto cosTheta = Float8(abs(brdf.intersection.smoothedNdotI));
            brdf.reflectanceCoefficient = max(
                iorReflectance,
                brdf::schlickFresnelSpectral(brdf.surface.specular.data, cosTheta)
            );
        }
        else
        {
            brdf.reflectanceCoefficient = iorReflectance;
        }

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
            brdf.lighting.reflected = spectral::Spectrum::zero();
            return;
        }

        // check whether it would even make a difference in the image
        const auto reflectionWeight =
            spectral::spectralMax(brdf.reflectanceCoefficient) * spectral::spectralMax(brdf.surface.reflectance);
        const auto reflectionVisibilityIndex = incidentRaytrace.visibilityIndex * reflectionWeight;
        const auto reflectionImportance = incidentRaytrace.importance * reflectionWeight;
        if (reflectionImportance < cache.configuration.visibilityCutoff)
        {
            brdf.lighting.reflected = spectral::Spectrum::zero();
            return;
        }

        // without distribution ray tracing (glossySamples 0), a rough surface reflects one mirror ray too
        const auto roughness = x(brdf.surface.roughness);
        const auto rough = roughness > brdf::MirrorRoughness;
        if (rough && cache.configuration.glossySamples > Zero<ASizeT>())
        {
            traceGlossyReflection(
                incidentRaytrace, maxDistance, reflectionVisibilityIndex, reflectionImportance, roughness, cache, brdf
            );
            return;
        }

        const auto reflectedRay = Ray(brdf.intersection.vertex, brdf.intersection.reflectedDirection);
        const auto reflectedRayCast = RayCast(
            reflectedRay, incidentRaytrace.rayCast.cullingMask,
            Size2(reinterpret_cast<ASizeT>(brdf.intersection.object), reinterpret_cast<ASizeT>(brdf.intersection.node)),
            maxDistance
        );
        auto reflectedRaytrace = Raytrace(
            reflectedRayCast, &brdf.intersection, incidentRaytrace.traceDepth + One<ASizeT>(),
            reflectionVisibilityIndex, incidentRaytrace.spectralContext,
            reflectionSeed(incidentRaytrace.seed, Zero<ASizeT>())
        );
        reflectedRaytrace.importance = reflectionImportance;
        // a rough surface gets area lights by its highlight, also without glossy samples; a regularized path stays
        // glossy after a glossy reflection
        reflectedRaytrace.glossy = rough || (cache.configuration.regularizePaths && incidentRaytrace.glossy);
        const auto reflectedHit = trace(reflectedRaytrace, cache);

        brdf.lighting.reflected = reflectedHit.color;

        cache.statistics.secondaryRays += One<ASizeT>();
        cache.statistics.missedSecondaryRays +=
            static_cast<ASizeT>(outOfReach(reflectedRaytrace.rayCast, x(reflectedHit.depth)));
    }

    // Averages reflection rays in directions of microfacet normals drawn from the GGX distribution of visible normals
    // (Heitz 2018), each weighted by Smith's masking of its direction. Directions below the surface count as black.
    void Raytracer::traceGlossyReflection(
        const Raytrace& incidentRaytrace, const Float maxDistance, const Float reflectionVisibilityIndex,
        const Float reflectionImportance, const Float roughness, RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        const auto& intersection = brdf.intersection;
        // the normals on the side the ray comes from, which isn't the outside for a ray inside a transmissive object
        const auto towardsViewer = x(intersection.smoothedNdotI) < Zero<Float>() ? One<Float>() : NegativeOne<Float>();
        const auto N = intersection.smoothedNormal * towardsViewer;
        const auto geometricNormal = intersection.surfaceNormal * towardsViewer;
        const auto V = -incidentRaytrace.rayCast.ray.direction;

        const auto lobeWidth = min(One<Float>(), Two<Float>() * Two<Float>() * roughness);
        const auto maxSamples = static_cast<Float>(cache.configuration.glossySamples);
        const auto samples = static_cast<ASizeT>(
            clamp(vectorization::ceil(maxSamples * reflectionVisibilityIndex * lobeWidth), One<Float>(), maxSamples)
        );
        const auto sampleVisibilityIndex = reflectionVisibilityIndex / static_cast<Float>(samples);

        // the shift of the sample sequence once per hit, and the reflected rays' seeds (reflectionSeed) 8 at a time,
        // all hashed in SIMD lanes
        const auto sequenceOffset = sequenceOffset2D(incidentRaytrace.strataSeed);
        alignas(YMM_ALIGNMENT) UInt_32 reflectionSeeds[UInt8_32::SIZE];

        auto reflected = spectral::Spectrum::zero();
        for (auto sample = Zero<ASizeT>(); sample < samples; ++sample)
        {
            const auto lane = sample % UInt8_32::SIZE;
            if (lane == Zero<ASizeT>())
            {
                const auto firstValue = static_cast<UInt_32>(sample) + One<UInt_32>();
                store(deriveSeeds(incidentRaytrace.seed, firstValue), reflectionSeeds);
            }
            const auto xi =
                sequencePoint2D(sequenceOffset, stratifiedIndex(incidentRaytrace.stratum, samples, sample));
            const auto direction = brdf::sampleGGX(N, V, xi, roughness);
            const auto NoL = dot3(N, direction);
            if (NoL <= Zero<Float>() || dot3(geometricNormal, direction) <= Zero<Float>())
            {
                continue;
            }

            const auto reflectedRayCast = RayCast(
                Ray(intersection.vertex, direction), incidentRaytrace.rayCast.cullingMask,
                Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
                maxDistance
            );
            auto reflectedRaytrace = Raytrace(
                reflectedRayCast, &intersection, incidentRaytrace.traceDepth + One<ASizeT>(), sampleVisibilityIndex,
                incidentRaytrace.spectralContext, reflectionSeeds[lane]
            );
            // each ray estimates the whole reflection: the cutoffs see the reflection's importance, not its share
            reflectedRaytrace.importance = reflectionImportance;
            reflectedRaytrace.glossy = true;
            const auto reflectedHit = trace(reflectedRaytrace, cache);
            // the clamp against fireflies scales the whole spectrum, so that the colour stays
            reflected +=
                spectral::limitPeak(reflectedHit.color, cache.configuration.glossyClamp) * smithG1(NoL, roughness);

            cache.statistics.secondaryRays += One<ASizeT>();
            cache.statistics.missedSecondaryRays +=
                static_cast<ASizeT>(outOfReach(reflectedRaytrace.rayCast, x(reflectedHit.depth)));
        }
        brdf.lighting.reflected = reflected * (One<Float>() / static_cast<Float>(samples));
    }

    void Raytracer::traceTransmission(
        const Raytrace& incidentRaytrace, const Float maxDistance, const bool leavingMaterial,
        const Float4& transmittedDirection, RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        // would still be visible, but tracing is not wanted any more, set transmitted to the background
        if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>())
        {
            brdf.lighting.transmitted = spectral::Spectrum::zero();
            return;
        }

        spectral::SpectralVector fractionTransmitted;
        if (leavingMaterial)
        {
            // world vacuum does not absorb
            fractionTransmitted = One<spectral::SpectralVector>();
        }
        else
        {
            // least possible transmitted fraction of material
            fractionTransmitted = Float8(
                vectorization::exp(
                    -std::numeric_limits<Float>::min() / spectral::spectralMax(brdf.surface.transmittance)));
        }
        brdf.fractionTransmitted = fractionTransmitted;

        // check whether it would even make a difference in the image,
        // or whether it is a total internal reflection (transmissionDirection = 0)
        const auto transmissionWeight = spectral::spectralMax(fractionTransmitted)
            * (One<Float>() - spectral::spectralMax(brdf.reflectanceCoefficient));
        const auto transmissionVisibilityIndex = incidentRaytrace.visibilityIndex * transmissionWeight;
        const auto transmissionImportance = incidentRaytrace.importance * transmissionWeight;
        if (transmissionImportance < cache.configuration.visibilityCutoff)
        {
            brdf.lighting.transmitted = spectral::Spectrum::zero();
            return;
        }

        const auto refractedRay = Ray(brdf.intersection.vertex, transmittedDirection);
        const auto refractedRayCast = RayCast(
            refractedRay, cullingOrientationToMask(-cullingOrientation(incidentRaytrace.rayCast)),
            Size2(reinterpret_cast<ASizeT>(brdf.intersection.object), reinterpret_cast<ASizeT>(brdf.intersection.node)),
            maxDistance
        );
        auto refractedRaytrace = Raytrace(
            refractedRayCast, &brdf.intersection, incidentRaytrace.traceDepth + One<ASizeT>(),
            transmissionVisibilityIndex, incidentRaytrace.spectralContext,
            deriveSeed(incidentRaytrace.seed, TransmissionSeed)
        );
        refractedRaytrace.importance = transmissionImportance;
        refractedRaytrace.glossy = cache.configuration.regularizePaths && incidentRaytrace.glossy;

        const auto refractedHit = trace(refractedRaytrace, cache);
        brdf.lighting.transmitted = refractedHit.color;
        // now that we have the transmission distance through the material, recompute the fractionTransmitted coefficient
        if (!leavingMaterial)
        {
            brdf.fractionTransmitted = spectral::spectralExp(
                -Float8(refractedHit.depth) / brdf.surface.transmittance.data);
        }

        cache.statistics.secondaryRays += One<ASizeT>();
        cache.statistics.missedSecondaryRays +=
            static_cast<ASizeT>(outOfReach(refractedRaytrace.rayCast, x(refractedHit.depth)));
    }

    spectral::Spectrum Raytracer::applyBRDF(const BRDFParameters& brdf)
    {
        const auto ambient = brdf.surface.diffusion * brdf.lighting.ambient;
        const auto diffuse = brdf.surface.diffusion * brdf.lighting.diffuse;
        const auto directLighting = ambient + diffuse + brdf.lighting.specular;

        const auto reflection = brdf.surface.reflectance * brdf.lighting.reflected;
        const auto transmitted = brdf.lighting.transmitted * brdf.fractionTransmitted;

        return brdf.surface.emittance + directLighting
            + spectral::spectralMix(transmitted, reflection, brdf.reflectanceCoefficient);
    }
}
