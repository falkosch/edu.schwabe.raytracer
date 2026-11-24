#include "raytracing/Raytracer.h"
#include "../stdafx.h"

#include "raytracing/RaytracerPackets.h"
#include "raytracing/SubSampleSpread.h"

#include "raytracing/shading/LightInfo.h"
#include "raytracing/shading/ObjectShader.h"
#include "raytracing/shading/SpectralContext.h"
#include "raytracing/shading/beer_lambert_transmittance.h"
#include "raytracing/shading/brdf/apply_brdf.h"
#include "raytracing/shading/brdf/fresnel_reflectance.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/power_heuristic.h"
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
        // derives the same seeds 8 at a time with deriveSeeds), and the rough transmission's from this value on
        constexpr UInt_32 TransmissionSeed = 0u;
        constexpr UInt_32 RoughTransmissionSeeds = 0x8000u;

        // The sample sequence of a rough transmission, derived from the strata seed (the area lights take 0x1000 and
        // the following values, one per light)
        constexpr UInt_32 TransmissionSequenceSeed = 0x2000u;

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

        // The number of glossy reflection rays of a rough surface: up to glossySamples, fewer the less the reflection
        // adds to the pixel and the narrower its lobe
        ASizeT glossyReflectionSamples(
            const ASizeT glossySamples, const Float reflectionVisibilityIndex, const Float roughness
        )
        {
            const auto lobeWidth = min(One<Float>(), Two<Float>() * Two<Float>() * roughness);
            const auto maxSamples = static_cast<Float>(glossySamples);
            return static_cast<ASizeT>(
                clamp(vectorization::ceil(maxSamples * reflectionVisibilityIndex * lobeWidth), One<Float>(), maxSamples)
            );
        }

        // A microfacet tilted by an angle turns the refracted ray by about |1 - eta| times it (eta the ratio of the
        // refraction indices), the reflected one by twice it: the refracted lobe is as wide as a reflected lobe of this
        // roughness
        Float refractedLobeRoughness(const Float roughness, const Float etaRatio)
        {
            return roughness * Half<Float>() * vectorization::abs(One<Float>() - etaRatio);
        }

        // The weight of the reflection (its Fresnel reflectance times the material's reflectance), on which the
        // reflected rays' visibility index and importance and the cutoff of the reflection depend
        Float reflectionWeightOf(const BRDFParameters& brdf)
        {
            return spectral::spectralMax(brdf.reflectanceCoefficient) * spectral::spectralMax(brdf.surface.reflectance);
        }

        // The number of glossy reflection rays traceReflection will trace at the hit, 0 for none: the light's
        // highlights weigh their shadow rays against them
        ASizeT glossyRaysOf(
            const Raytrace& raytrace, const Float maxDistance, const RaytraceConfiguration& configuration,
            const BRDFParameters& brdf
        )
        {
            const auto roughness = x(brdf.surface.roughness);
            const auto reflectionWeight = reflectionWeightOf(brdf);
            if (raytrace.traceDepth >= configuration.maxTraceDepth || maxDistance <= Zero<Float>()
                || configuration.glossySamples == Zero<ASizeT>() || !(roughness > brdf::MirrorRoughness)
                || raytrace.importance * reflectionWeight < configuration.visibilityCutoff)
            {
                return Zero<ASizeT>();
            }
            return glossyReflectionSamples(
                configuration.glossySamples, raytrace.visibilityIndex * reflectionWeight, roughness
            );
        }

        // The highlight of an area light that a glossy ray (direction L) found at its emitter, against the light's
        // shadow rays (multiple importance sampling): the light's radiance as the shadow rays take it times the BRDF
        // per density of the VNDF samples, F * G2 / G1(V), weighed by the power heuristic. A surface without highlights
        // (F0 = 0, the refraction index of the vacuum) reflects no light, as sampleLighting has it.
        spectral::SpectralVector weighedEmitterHighlight(
            const SceneShader& sceneShader, const LightInfo& light, const Raytrace& incidentRaytrace,
            const Float4& adaptedVisibilityCutoff, const SceneIntersection& intersection, const Float4& toPoint,
            const Float4& N, const Float4& V, const Float4& L, const Float roughness,
            const spectral::SpectralVector& F0, const ASizeT glossyRays, const ASizeT lightSamples
        )
        {
            if (!(spectral::spectralMax(F0) > Zero<Float>()))
            {
                return Zero<spectral::SpectralVector>();
            }
            const auto density =
                sceneShader.lightPointDensity(light, incidentRaytrace, adaptedVisibilityCutoff, intersection, toPoint);
            if (!density.emits)
            {
                return Zero<spectral::SpectralVector>();
            }
            const auto NoV = dot3(N, V);
            const auto NoL = dot3(N, L);
            const auto H = normalize3(V + L);
            const auto NoH = max(dot3(N, H), Zero<Float>());
            const auto glossyPdf = brdf::pdfGGX(NoV, NoH, roughness);
            const auto weight = powerHeuristic(
                static_cast<Float>(glossyRays), glossyPdf, static_cast<Float>(lightSamples), density.pdf
            );
            const auto G2 = brdf::ggxG_smith(NoV, NoL, roughness);
            const auto F = brdf::schlickFresnelSpectral(F0, Float8(max(dot3v(V, H), Zero<Float4>())));
            return density.radiance.data * F * spectral::SpectralVector(G2 / brdf::smithG1(NoV, roughness) * weight);
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
    // So a frame needs one SpectralContext per sub-sample, shared by all pixels, and per round of adaptive sampling:
    // round 0 at the centers of the sub-samples' strata of the visible range, the further rounds shifted within them
    // (by the golden ratio's fraction from round to round).
    SpectralContexts createSpectralContexts(
        const SceneShader& sceneShader, const Size2::ValueType subSamplesCount, const ASizeT rounds
    )
    {
        constexpr Float GoldenRatioFraction = 0.6180339887f;
        auto contexts = SpectralContexts();
        contexts.reserve(subSamplesCount * (rounds + One<ASizeT>()));
        for (auto round = Zero<ASizeT>(); round <= rounds; ++round)
        {
            const auto shift = round == Zero<ASizeT>()
                                   ? Half<Float>()
                                   : fract(Half<Float>() + GoldenRatioFraction * static_cast<Float>(round));
            for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount; ++s)
            {
                const auto heroLambda = spectral::wrapToVisible(
                    spectral::LAMBDA_MIN
                    + (static_cast<Float>(s) + shift)
                    / static_cast<Float>(subSamplesCount) * spectral::LAMBDA_RANGE
                );
                contexts.push_back(sceneShader.createSpectralContext(heroLambda));
            }
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
        const auto subSamplesCount = packets.pixelSubSamplesCount();
        // adaptive sampling judges a pixel by the spread of its sub-samples, which needs a few of them, against the
        // noise of the distribution ray tracing: Whitted ray tracing doesn't sample adaptively
        constexpr Size2::ValueType AdaptiveSubSamples = 4;
        const auto distributed =
            running.glossySamples > Zero<ASizeT>() || running.sphereLightSamples > Zero<ASizeT>();
        const auto adaptiveRounds =
            subSamplesCount >= AdaptiveSubSamples && distributed ? running.adaptiveRounds : Zero<ASizeT>();
        const auto spectralContexts = createSpectralContexts(*running.sceneShader, subSamplesCount, adaptiveRounds);

        // Adaptive sampling: further rounds of a pixel's sub-samples while the spread of all its sub-samples so far is
        // above the threshold, each round on the grid shifted within the sub-samples' cells (Roberts' R2 sequence) and
        // at its own hero wavelengths. Its sub-samples continue the pixel's sample sequence (Raytrace::stratum).
        const auto refinePixel = [&](Float4* const outputPixel, SubSampleSpread spread, RaytracerCache& cache)
        {
            const auto& configuration = cache.configuration;
            const auto imageIndex = static_cast<ASizeT>(outputPixel - configuration.image->getData());
            const auto width = x(configuration.image->getResolution());
            const auto pixelXYf = convert<Float4>(Size2(imageIndex % width, imageIndex / width));
            const auto nearTL = packets.pixelNearTopLeft(pixelXYf);
            const auto farTL = packets.pixelFarTopLeft(pixelXYf);
            const auto cullingMask = cullingOrientationToMask(configuration.cullingOrientation);
            const auto strataSeed = primarySeed(imageIndex, Zero<ASizeT>(), configuration.seedOffset);

            auto rounds = Zero<ASizeT>();
            auto sum = Zero<Float4>();
            while (rounds < adaptiveRounds && spread.error() > configuration.adaptiveThreshold)
            {
                ++rounds;
                const auto shift =
                    sequencePoint2D(Zero<Float4>(), rounds) - Float4(Half<Float>(), Half<Float>(), Zero<Float>(),
                                                                     Zero<Float>());
                const auto shiftedNearTL =
                    nearTL + packets.ssNearRightDir * x(shift) + packets.ssNearBottomDir * y(shift);
                const auto shiftedFarTL = farTL + packets.ssFarRightDir * x(shift) + packets.ssFarBottomDir * y(shift);
                for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount; ++s)
                {
                    const auto subSample = rounds * subSamplesCount + s;
                    const auto rayCast = RayCast(
                        packets.setupRayOfSampleInPixel(s, shiftedNearTL, shiftedFarTL), cullingMask, Zero<Size2>(),
                        configuration.maxDistance
                    );
                    auto raytrace = Raytrace(
                        rayCast, nullptr, Zero<ASizeT>(), One<Float>(), &spectralContexts[subSample],
                        primarySeed(imageIndex, subSample, configuration.seedOffset)
                    );
                    raytrace.strataSeed = strataSeed;
                    raytrace.stratum = static_cast<UInt_32>(subSample);

                    const auto hit = Raytracer::trace(raytrace, cache);
                    cache.statistics.primaryRays += One<ASizeT>();
                    cache.statistics.missedPrimaryRays += static_cast<ASizeT>(outOfReach(rayCast, x(hit.depth)));
                    const auto xyz = spectral::spectrumToXYZ(hit.color, raytrace.spectralContext->xyz);
                    const auto displayRGB = spectral::xyzToDisplayRGB(xyz);
                    sum += displayRGB.value;
                    spread.add(displayRGB.value);
                }
            }
            if (rounds > Zero<ASizeT>())
            {
                // the pixel holds the mean of the first round
                const auto mean =
                    (Float4(outputPixel) + sum * packets.ssAvg) / Float4(static_cast<Float>(rounds + One<ASizeT>()));
                store(mean, outputPixel);
            }
        };

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

                // a pixel's sub-samples follow each other in the list
                auto spread = SubSampleSpread();
                for (auto i = Zero<ASizeT>(); i < packedRaytraces.size(); ++i)
                {
                    const auto& packedRaytrace = packedRaytraces[i];
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

                    if (adaptiveRounds > Zero<ASizeT>())
                    {
                        spread.add(displayRGB.value);
                        if ((i + One<ASizeT>()) % subSamplesCount == Zero<ASizeT>())
                        {
                            refinePixel(packedRaytrace.outputPixel, spread, cache);
                            // the timing of the last sub-sample with the further rounds
                            store(perPixelTiming(start), cache.configuration.timingMap->getData() + imagePtrIndex);
                            spread = SubSampleSpread();
                        }
                    }
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

        // a glossy reflection gets an area light by its highlight: the light's emissive object would add it twice,
        // unless the reflection weighs it against the highlight's shadow rays
        if (raytrace.glossy && objectShader.isLightEmitter())
        {
            if (raytrace.weighsEmitters)
            {
                return {brdf.viewDistance, spectral::Spectrum::zero(), objectShader.getEmittedLight()};
            }
            return {brdf.viewDistance, spectral::Spectrum::zero()};
        }
        brdf.surface = objectShader.shade(brdf.intersection, *raytrace.spectralContext);

        // Whitted raytracing part, reflection and transmission
        const auto transmittedDirection = refract(
            raytrace.rayCast.cullingMask, raytrace.rayCast.ray.direction, brdf.intersection.smoothedNormal,
            brdf.intersection.smoothedNdotI, brdf.surface.refractionEta
        );

        const auto leavingMaterial = frontfaceCulled(raytrace.rayCast)
            || (notCulled(raytrace.rayCast) && isNegative(-brdf.intersection.smoothedNdotI));

        const auto totalInternalReflection = allTrue3(!transmittedDirection);
        const auto scalarReflectance = brdf::fresnelReflectance(
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

        // A rough dielectric whose refracted lobe is wider than a mirror's reflects and refracts through sampled
        // microfacet normals, each with its own Fresnel reflectance (Walter et al. 2007). A narrower refracted lobe
        // refracts as a mirror (as reflections do up to MirrorRoughness), both lobes with the smooth surface's Fresnel.
        const auto interfaceEta =
            select(leavingMaterial, yxwz(brdf.surface.refractionEta), brdf.surface.refractionEta);
        const auto roughInterface = !totalInternalReflection
            && cache.configuration.glossySamples > Zero<ASizeT>()
            && spectral::spectralMax(brdf.surface.transmittance) > Zero<Float>()
            && refractedLobeRoughness(x(brdf.surface.roughness), x(interfaceEta)) > brdf::MirrorRoughness;
        const auto* const microfacetEta = roughInterface ? &interfaceEta : nullptr;

        // the area lights weigh the shadow rays of their highlights against the glossy rays
        const auto glossyRays = cache.configuration.sphereLightSamples > Zero<ASizeT>()
                                    ? glossyRaysOf(raytrace, maxDistance, cache.configuration, brdf)
                                    : Zero<ASizeT>();
        const auto adapted_visibility_cutoff = SceneShader::adaptedVisibilityCutoff(
            cache.configuration.visibilityCutoff, raytrace.importance);
        brdf.lighting = cache.configuration.sceneShader->sampleLighting(
            raytrace, adapted_visibility_cutoff, x(brdf.surface.roughness), brdf.surface.specular.data,
            brdf.intersection, cache.configuration.sphereLightSamples, glossyRays, cache.shadowCache,
            cache.statistics
        );

        cache.statistics.shadingTicks += __rdtsc() - shadingStart;

        traceReflection(raytrace, maxDistance, microfacetEta, cache, brdf);
        traceTransmission(raytrace, maxDistance, leavingMaterial, transmittedDirection, microfacetEta, cache, brdf);

        return {brdf.viewDistance, brdf::applyBRDF(brdf)};
    }

    void Raytracer::traceReflection(
        const Raytrace& incidentRaytrace, const Float maxDistance, const Float4* const microfacetEta,
        RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        // would still be visible but tracing is not wanted any more
        if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>())
        {
            brdf.lighting.reflected = spectral::Spectrum::zero();
            return;
        }

        // check whether it would even make a difference in the image
        const auto reflectionWeight = reflectionWeightOf(brdf);
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
                incidentRaytrace, maxDistance, reflectionVisibilityIndex, reflectionImportance, roughness,
                microfacetEta, cache, brdf
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
    // With shadow rays to area lights, a ray that finds a light's emitter adds the light to the highlight instead,
    // weighed against the light's shadow rays (multiple importance sampling, SceneShader::sampleLighting). At a rough
    // dielectric interface (microfacetEta), each ray takes its microfacet's own Fresnel reflectance instead of the
    // smooth surface's, which applyBRDF multiplies with: 1 for a microfacet that reflects totally (Walter et al. 2007).
    void Raytracer::traceGlossyReflection(
        const Raytrace& incidentRaytrace, const Float maxDistance, const Float reflectionVisibilityIndex,
        const Float reflectionImportance, const Float roughness, const Float4* const microfacetEta,
        RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        const auto& intersection = brdf.intersection;
        // the normals on the side the ray comes from, which isn't the outside for a ray inside a transmissive object
        const auto towardsViewer = x(intersection.smoothedNdotI) < Zero<Float>() ? One<Float>() : NegativeOne<Float>();
        const auto N = intersection.smoothedNormal * towardsViewer;
        const auto geometricNormal = intersection.surfaceNormal * towardsViewer;
        const auto V = -incidentRaytrace.rayCast.ray.direction;

        const auto samples =
            glossyReflectionSamples(cache.configuration.glossySamples, reflectionVisibilityIndex, roughness);
        const auto sampleVisibilityIndex = reflectionVisibilityIndex / static_cast<Float>(samples);
        // the shadow rays of the area lights at this hit, against which the rays weigh the emitters they find
        const auto weighsEmitters = cache.configuration.sphereLightSamples > Zero<ASizeT>();
        const auto lightSamples =
            SceneShader::areaLightSamples(cache.configuration.sphereLightSamples, incidentRaytrace.visibilityIndex);
        const auto lightCutoff =
            SceneShader::adaptedVisibilityCutoff(cache.configuration.visibilityCutoff, incidentRaytrace.importance);

        // the shift of the sample sequence once per hit, and the reflected rays' seeds (reflectionSeed) 8 at a time,
        // all hashed in SIMD lanes
        const auto sequenceOffset = sequenceOffset2D(incidentRaytrace.strataSeed);
        alignas(YMM_ALIGNMENT) UInt_32 reflectionSeeds[UInt8_32::SIZE];

        auto reflected = spectral::Spectrum::zero();
        auto highlight = Zero<spectral::SpectralVector>();
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
            const auto microfacetNormal = brdf::sampleGGXNormal(N, V, xi, roughness);
            const auto direction = reflect(V * -One<Float>(), microfacetNormal);
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
            reflectedRaytrace.weighsEmitters = weighsEmitters;
            const auto reflectedHit = trace(reflectedRaytrace, cache);

            cache.statistics.secondaryRays += One<ASizeT>();
            cache.statistics.missedSecondaryRays +=
                static_cast<ASizeT>(outOfReach(reflectedRaytrace.rayCast, x(reflectedHit.depth)));

            if (reflectedHit.emitterLight)
            {
                // the light's own estimate, weighed, needs no clamp
                highlight += weighedEmitterHighlight(
                    *cache.configuration.sceneShader, *reflectedHit.emitterLight, incidentRaytrace, lightCutoff,
                    intersection, direction * x(reflectedHit.depth), N, V, direction, roughness,
                    brdf.surface.specular.data, samples, lightSamples
                );
                continue;
            }
            // the clamp against fireflies scales the whole spectrum, so that the colour stays
            auto weight = brdf::smithG1(NoL, roughness);
            if (microfacetEta)
            {
                const auto& incident = incidentRaytrace.rayCast.ray.direction;
                weight *= x(brdf::fresnelReflectance(incident, microfacetNormal, *microfacetEta))
                    / spectral::spectralMax(brdf.reflectanceCoefficient);
            }
            reflected += spectral::limitPeak(reflectedHit.color, cache.configuration.glossyClamp) * weight;
        }
        const auto perSample = One<Float>() / static_cast<Float>(samples);
        brdf.lighting.reflected = reflected * perSample;
        if (weighsEmitters)
        {
            brdf.lighting.specular += spectral::Spectrum{highlight * spectral::SpectralVector(perSample)};
        }
    }

    void Raytracer::traceTransmission(
        const Raytrace& incidentRaytrace, const Float maxDistance, const bool leavingMaterial,
        const Float4& transmittedDirection, const Float4* const microfacetEta, RaytracerCache& cache,
        BRDFParameters& brdf
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

        // a rough dielectric interface refracts through microfacets; otherwise one mirror refraction
        if (microfacetEta)
        {
            traceRoughTransmission(
                incidentRaytrace, maxDistance, leavingMaterial, *microfacetEta, transmissionVisibilityIndex,
                transmissionImportance, cache, brdf
            );
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
            brdf.fractionTransmitted = beerLambertTransmittance(
                Float8(refractedHit.depth), brdf.surface.transmittance.data);
        }

        cache.statistics.secondaryRays += One<ASizeT>();
        cache.statistics.missedSecondaryRays +=
            static_cast<ASizeT>(outOfReach(refractedRaytrace.rayCast, x(refractedHit.depth)));
    }

    // Averages refracted rays through microfacet normals drawn from the GGX distribution of visible normals (Walter et
    // al. 2007, sampled as Heitz 2018), each weighted by Smith's masking of its direction (G2 / G1(V), about G1 of
    // the refracted direction, as for the glossy reflection), its microfacet's Fresnel transmission per the smooth
    // surface's (which applyBRDF multiplies with) and its absorption. A microfacet that reflects totally transmits
    // nothing (its light is in the reflection), and a direction that doesn't cross the surface is masked: both count as
    // black. The refracted lobe is narrower than the reflected one and takes fewer rays (refractedLobeRoughness).
    void Raytracer::traceRoughTransmission(
        const Raytrace& incidentRaytrace, const Float maxDistance, const bool leavingMaterial,
        const Float4& interfaceEta, const Float transmissionVisibilityIndex, const Float transmissionImportance,
        RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        const auto& intersection = brdf.intersection;
        const auto& direction = incidentRaytrace.rayCast.ray.direction;
        const auto V = -direction;
        // the normals on the side the ray comes from, as the mirror refraction takes it from the ray's culling
        const auto side = leavingMaterial ? NegativeOne<Float>() : One<Float>();
        const auto N = intersection.smoothedNormal * side;
        const auto geometricNormal = intersection.surfaceNormal * side;
        const auto eta = xxxx(interfaceEta);
        const auto NoV = dot3(N, V);
        const auto roughness = x(brdf.surface.roughness);
        const auto perSmoothTransmission =
            One<Float>() / (One<Float>() - spectral::spectralMax(brdf.reflectanceCoefficient));

        // the refracted lobe takes as many rays as a reflected lobe of its width
        const auto samples = glossyReflectionSamples(
            cache.configuration.glossySamples, transmissionVisibilityIndex, refractedLobeRoughness(roughness, x(eta))
        );
        const auto sampleVisibilityIndex = transmissionVisibilityIndex / static_cast<Float>(samples);
        const auto sequenceOffset =
            sequenceOffset2D(deriveSeed(incidentRaytrace.strataSeed, TransmissionSequenceSeed));
        const auto culling = cullingOrientationToMask(-cullingOrientation(incidentRaytrace.rayCast));

        auto transmitted = spectral::Spectrum::zero();
        for (auto sample = Zero<ASizeT>(); sample < samples; ++sample)
        {
            const auto xi =
                sequencePoint2D(sequenceOffset, stratifiedIndex(incidentRaytrace.stratum, samples, sample));
            const auto microfacetNormal = NoV > Zero<Float>() ? brdf::sampleGGXNormal(N, V, xi, roughness) : N;
            const auto refracted = refractEta(direction, microfacetNormal, dotv(microfacetNormal, direction), eta);
            const auto NoT = -dot3(N, refracted);
            if (allTrue3(!refracted) || !(NoT > Zero<Float>()) || !(dot3(geometricNormal, refracted) < Zero<Float>()))
            {
                continue;
            }
            const auto weight = brdf::smithG1(NoT, roughness) * perSmoothTransmission
                * (One<Float>() - x(brdf::fresnelReflectance(direction, microfacetNormal, interfaceEta)));

            const auto refractedRayCast = RayCast(
                Ray(intersection.vertex, refracted), culling,
                Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
                maxDistance
            );
            auto refractedRaytrace = Raytrace(
                refractedRayCast, &intersection, incidentRaytrace.traceDepth + One<ASizeT>(), sampleVisibilityIndex,
                incidentRaytrace.spectralContext,
                deriveSeed(incidentRaytrace.seed, RoughTransmissionSeeds + static_cast<UInt_32>(sample))
            );
            // each ray estimates the whole transmission
            refractedRaytrace.importance = transmissionImportance;
            refractedRaytrace.glossy = cache.configuration.regularizePaths && incidentRaytrace.glossy;
            const auto refractedHit = trace(refractedRaytrace, cache);

            const auto absorption = leavingMaterial
                                        ? One<spectral::SpectralVector>()
                                        : beerLambertTransmittance(
                                            Float8(refractedHit.depth), brdf.surface.transmittance.data);
            transmitted += spectral::Spectrum{
                refractedHit.color.data * absorption * spectral::SpectralVector(weight)
            };

            cache.statistics.secondaryRays += One<ASizeT>();
            cache.statistics.missedSecondaryRays +=
                static_cast<ASizeT>(outOfReach(refractedRaytrace.rayCast, x(refractedHit.depth)));
        }
        // the absorption is in each ray's share already
        brdf.lighting.transmitted = transmitted * (One<Float>() / static_cast<Float>(samples));
        brdf.fractionTransmitted = One<spectral::SpectralVector>();
    }
}
