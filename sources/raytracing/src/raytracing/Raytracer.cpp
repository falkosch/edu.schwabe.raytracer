#include "raytracing/Raytracer.h"
#include "../stdafx.h"

#include "raytracing/ATrousDenoiser.h"
#include "raytracing/AdaptiveRounds.h"
#include "raytracing/IrradianceCache.h"
#include "raytracing/PixelFeatures.h"
#include "raytracing/RaytracerPackets.h"
#include "raytracing/SubSampleSpread.h"

#include "raytracing/shading/LightInfo.h"
#include "raytracing/shading/ObjectShader.h"
#include "raytracing/shading/SpectralContext.h"
#include "raytracing/shading/beer_lambert_transmittance.h"
#include "raytracing/shading/brdf/apply_brdf.h"
#include "raytracing/shading/brdf/SpecularAlbedo.h"
#include "raytracing/shading/brdf/SpecularFresnel.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/brdf/sample_lambert.h"
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
        // derives the same seeds 8 at a time with deriveSeeds), the diffuse rays' and the rough transmission's from
        // these values on
        constexpr UInt_32 TransmissionSeed = 0u;
        constexpr UInt_32 DiffuseSeeds = 0x4000u;
        constexpr UInt_32 RoughTransmissionSeeds = 0x8000u;

        // The sample sequences of a rough transmission and of the diffuse rays, derived from the strata seed (the area
        // lights take 0x1000 and the following values, one per light)
        constexpr UInt_32 TransmissionSequenceSeed = 0x2000u;
        constexpr UInt_32 DiffuseSequenceSeed = 0x3000u;

        // The hash of an irradiance record's pixel that picks its spectral context
        constexpr UInt_32 RecordContextSeed = 0x5000u;

        // Irradiance caching: Ward's accuracy a, the coverage from which the records alone give a hit's irradiance (a
        // single record within 2/3 of its validity radius; less blends them with the hit's own rays), the grids of the
        // records' levels (in pixels, coarse to fine), the span of a record's validity radius a R (in pixels at its
        // distance: from MinRecordPixels to twice its level's grid, the cap) and its rays per diffuseSamples
        constexpr Float IrradianceAccuracy = 0.1f;
        constexpr Float FullCoverage = 0.5f;
        constexpr int CoarsestRecordGrid = 32;
        constexpr int FinestRecordGrid = 4;
        constexpr Float MinRecordPixels = 1.5f;
        constexpr ASizeT RecordRaysPerDiffuseSample = 16;
        constexpr ASizeT MinRecordRays = 256;

        // A record's rays are limited to this many times their mean radiance (twice: the mean of the limited rays
        // again): a rare bright path one of them catches (a light's highlight or caustic through a mirror or glass,
        // unclamped with glossyClamp 0) would otherwise spread over the record's area as a blotch. Paths that at
        // least 1 in 16 of the rays take stay whole.
        constexpr Float RecordOutlierFactor = 16.0f;

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

        // The number of diffuse rays of a hit: up to diffuseSamples, fewer the less its diffuse reflection adds to the
        // pixel
        ASizeT diffuseReflectionSamples(const ASizeT diffuseSamples, const Float diffuseVisibilityIndex)
        {
            const auto maxSamples = static_cast<Float>(diffuseSamples);
            return static_cast<ASizeT>(
                clamp(vectorization::ceil(maxSamples * diffuseVisibilityIndex), One<Float>(), maxSamples)
            );
        }

        // A microfacet tilted by an angle turns the refracted ray by about |1 - eta| times it (eta the ratio of the
        // refraction indices), the reflected one by twice it: the refracted lobe is as wide as a reflected lobe of this
        // roughness
        Float refractedLobeRoughness(const Float roughness, const Float etaRatio)
        {
            return roughness * Half<Float>() * vectorization::abs(One<Float>() - etaRatio);
        }

        // The weight of the reflection (the specular lobe's reflectance towards the viewer, its weight included), on
        // which the reflected rays' visibility index and importance and the cutoff of the reflection depend
        Float reflectionWeightOf(const BRDFParameters& brdf)
        {
            return spectral::spectralMax(brdf.reflectanceCoefficient);
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

        // What the specular lobe leaves of light from all directions alike (1 without a lobe), for the diffuse base
        // under it: the share of the irradiance the diffuse rays gather (the records keep the irradiance itself, for
        // any material)
        spectral::SpectralVector uniformDiffuseShare(const BRDFParameters& brdf)
        {
            if (!brdf.fresnel.reflects())
            {
                return One<spectral::SpectralVector>();
            }
            return One<spectral::SpectralVector>() - brdf.fresnel.averageAlbedo(x(brdf.surface.roughness));
        }

        // What a transmissive surface's diffusion leaves of the light its specular lobe lets pass: the transmission and
        // the diffuse reflection share it (1 - the diffusion, at least 0)
        spectral::SpectralVector undiffusedShare(const BRDFParameters& brdf)
        {
            return max(One<spectral::SpectralVector>() - brdf.surface.diffusion.data, Zero<spectral::SpectralVector>());
        }

        // The highlight of an area light that a glossy ray (direction L) found at its emitter, against the light's
        // shadow rays (multiple importance sampling): the light's radiance as the shadow rays take it times the BRDF
        // per density of the VNDF samples, F * G2 / G1(V), weighed by the power heuristic. F is the specular lobe's
        // Fresnel reflectance, as for the shadow rays' part: a lobe that doesn't reflect (F0 = 0, equal indices)
        // reflects no light, as sampleLighting has it.
        spectral::SpectralVector weighedEmitterHighlight(
            const SceneShader& sceneShader, const LightInfo& light, const Raytrace& incidentRaytrace,
            const Float4& adaptedVisibilityCutoff, const SceneIntersection& intersection, const Float4& toPoint,
            const Float4& N, const Float4& V, const Float4& L, const Float roughness,
            const brdf::SpecularFresnel& fresnel, const ASizeT glossyRays, const ASizeT lightSamples
        )
        {
            if (!fresnel.reflects())
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
            const auto F = fresnel.reflectance(dot3v(V, H));
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

    // The hero wavelengths a sub-sample takes, one per pixel in turn: with fewer sub-samples than a context has
    // wavelengths (spectral::SPECTRAL_SAMPLES), so many that a pixel's neighbourhood takes them all, where the image is
    // denoised (the denoiser averages them; elsewhere their dither would show on saturated colours); otherwise one
    ASizeT contextsPerSubSample(const Size2::ValueType subSamplesCount, const bool denoised)
    {
        assert(subSamplesCount >= spectral::SPECTRAL_SAMPLES || spectral::SPECTRAL_SAMPLES % subSamplesCount == 0);
        return denoised && subSamplesCount < spectral::SPECTRAL_SAMPLES ? spectral::SPECTRAL_SAMPLES / subSamplesCount
                                                                        : One<ASizeT>();
    }

    // The pixel's one of a sub-sample's contexts, by Roberts' R2 sequence over the pixels (a dither: every 5 x 5
    // pixels, the denoiser's first pass, take all 8, every 3 x 3 at least 6). A lattice such as (x + 3 y) mod 8 takes
    // all 8 in every 3 x 3 pixels, but shows as diagonal stripes before denoising.
    ASizeT pixelContext(const Size2& pixel, const ASizeT contexts)
    {
        constexpr Float R2X = 0.7548776662f;
        constexpr Float R2Y = 0.5698402910f;
        const auto position = fract(R2X * static_cast<Float>(x(pixel)) + R2Y * static_cast<Float>(y(pixel)));
        return min(static_cast<ASizeT>(position * static_cast<Float>(contexts)), contexts - One<ASizeT>());
    }

    // Every ray of a path keeps the hero wavelength of its primary ray, which depends on the pixel sub-sample and,
    // with perSubSample contexts per sub-sample, on the pixel (pixelContext). So a frame needs one SpectralContext per
    // sub-sample and pixel context, shared by all pixels, and per round of adaptive sampling: round 0 at the centers
    // of the strata, the further rounds shifted within them (by the golden ratio's fraction from round to round). The
    // strata divide one wavelength step (DELTA_LAMBDA): a context's 8 wavelengths lie a step apart, so that strata of
    // the whole visible range would give 1, 2, 4 or 8 sub-samples the same 8 wavelengths, and strata of the
    // sub-samples alone would give every pixel the same 8 or 32 (a fixed quadrature that shifts saturated colours).
    // Context round * subSamplesCount * perSubSample + s * perSubSample + c is the sub-sample s's context c.
    SpectralContexts createSpectralContexts(
        const SceneShader& sceneShader, const Size2::ValueType subSamplesCount, const ASizeT perSubSample,
        const ASizeT rounds
    )
    {
        constexpr Float GoldenRatioFraction = 0.6180339887f;
        const auto strata = subSamplesCount * perSubSample;
        auto contexts = SpectralContexts();
        contexts.reserve(strata * (rounds + One<ASizeT>()));
        for (auto round = Zero<ASizeT>(); round <= rounds; ++round)
        {
            const auto shift = round == Zero<ASizeT>()
                                   ? Half<Float>()
                                   : fract(Half<Float>() + GoldenRatioFraction * static_cast<Float>(round));
            for (auto stratum = Zero<ASizeT>(); stratum < strata; ++stratum)
            {
                const auto heroLambda = spectral::LAMBDA_MIN
                    + (static_cast<Float>(stratum) + shift) / static_cast<Float>(strata) * spectral::DELTA_LAMBDA;
                contexts.push_back(sceneShader.createSpectralContext(heroLambda));
            }
        }
        return contexts;
    }

    PackedRaytrace::ListType constructPackedRaytracesList(
        const int packetIndex, const RaytracerPackets& packets, const RaytraceConfiguration& configuration,
        const SpectralContexts& spectralContexts, const ASizeT perSubSample
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
            const auto context = pixelContext(cpXY, perSubSample);

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
                    newRayCast, nullptr, Zero<ASizeT>(), One<Float>(), &spectralContexts[s * perSubSample + context],
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
        // the specular lobes' albedo tables, computed in parallel on their first use: not inside the parallel regions
        brdf::SpecularAlbedo::tables();

        const auto packets = RaytracerPackets(running);
        const auto subSamplesCount = packets.pixelSubSamplesCount();
        const auto& sceneResolution = running.image->getResolution();
        // adaptive sampling judges a pixel by the spread of its sub-samples, which needs a few of them, against the
        // noise of the distribution ray tracing: Whitted ray tracing doesn't sample adaptively
        constexpr Size2::ValueType AdaptiveSubSamples = 4;
        const auto distributed = running.glossySamples > Zero<ASizeT>()
            || running.sphereLightSamples > Zero<ASizeT>() || running.diffuseSamples > Zero<ASizeT>();
        const auto adaptiveRounds =
            subSamplesCount >= AdaptiveSubSamples && distributed ? running.adaptiveRounds : Zero<ASizeT>();
        // Whitted ray tracing doesn't sample: nothing to denoise
        const auto denoising = running.denoise && distributed;
        const auto perSubSample = contextsPerSubSample(subSamplesCount, denoising);
        const auto spectralContexts =
            createSpectralContexts(*running.sceneShader, subSamplesCount, perSubSample, adaptiveRounds);

        // Irradiance caching: the records of each sub-sample of the first round (a set of its contexts), on grids from
        // coarse to fine, at the pixels where the coarser grids' records don't cover the hit yet. A level's rays see
        // only the records of the coarser levels, so that the records don't depend on the threads' order. A record
        // takes one of its sub-sample's contexts by a hash of its pixel (the pixels' turns would give a grid's pixels
        // the same few), so that its neighbours have the irradiance at other wavelengths.
        auto irradianceCache = std::unique_ptr<IrradianceCache>();
        if (running.irradianceCaching && running.diffuseSamples > Zero<ASizeT>())
        {
            irradianceCache = std::make_unique<IrradianceCache>(
                spectralContexts.data(), subSamplesCount * perSubSample, perSubSample, IrradianceAccuracy
            );
            const auto width = static_cast<int>(x(sceneResolution));
            const auto height = static_cast<int>(y(sceneResolution));
            const auto cullingMask = cullingOrientationToMask(running.cullingOrientation);
            // the angle between neighbouring pixels' rays at the image's center, for the radii in pixels
            const auto centerXYf = convert<Float4>(Size2(x(sceneResolution) / 2, y(sceneResolution) / 2));
            const auto rightXYf = centerXYf + Float4(One<Float>(), Zero<Float>(), Zero<Float>(), Zero<Float>());
            const auto rightRay = packets.setupRayOfSampleInPixel(
                0, packets.pixelNearTopLeft(rightXYf), packets.pixelFarTopLeft(rightXYf)
            );
            const auto centerRay = packets.setupRayOfSampleInPixel(
                0, packets.pixelNearTopLeft(centerXYf), packets.pixelFarTopLeft(centerXYf)
            );
            const auto pixelAngle = length3(rightRay.direction - centerRay.direction);
            const auto sets = static_cast<ASizeT>(subSamplesCount);
            for (auto grid = CoarsestRecordGrid; grid >= FinestRecordGrid; grid /= 2)
            {
                auto pixels = std::vector<Size2>();
                for (auto py = 0; py < height; py += grid)
                {
                    for (auto px = 0; px < width; px += grid)
                    {
                        if (grid == CoarsestRecordGrid || px % (2 * grid) != 0 || py % (2 * grid) != 0)
                        {
                            pixels.emplace_back(static_cast<ASizeT>(px), static_cast<ASizeT>(py));
                        }
                    }
                }
                auto slots =
                    std::vector<IrradianceRecord, AlignedAllocator<IrradianceRecord>>(pixels.size() * sets);
                auto origins = std::vector<Float4, AlignedAllocator<Float4>>(slots.size());
#pragma omp parallel
                {
                    auto cache = RaytracerCache(running);
                    cache.irradianceCache = irradianceCache.get();
                    StatisticsCookie::current = &cache.statistics;
#pragma omp for schedule(static, 16)
                    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
                    {
                        const auto slot = static_cast<ASizeT>(i);
                        const auto s = slot % sets;
                        const auto& pixel = pixels[slot / sets];
                        const auto imageIndex = RaytracerPackets::imageIndexOfPixel(pixel, sceneResolution);
                        const auto context = s * perSubSample
                            + deriveSeed(static_cast<UInt_32>(imageIndex), RecordContextSeed) % perSubSample;
                        const auto pixelXYf = convert<Float4>(pixel);
                        const auto ray = packets.setupRayOfSampleInPixel(
                            static_cast<Size2::ValueType>(s), packets.pixelNearTopLeft(pixelXYf),
                            packets.pixelFarTopLeft(pixelXYf)
                        );
                        auto raytrace = Raytrace(
                            RayCast(ray, cullingMask, Zero<Size2>(), running.maxDistance), nullptr, Zero<ASizeT>(),
                            One<Float>(), &spectralContexts[context], primarySeed(imageIndex, s, running.seedOffset)
                        );
                        raytrace.strataSeed = primarySeed(imageIndex, Zero<ASizeT>(), running.seedOffset);
                        raytrace.stratum = static_cast<UInt_32>(s);
                        cache.recording = &slots[slot];
                        trace(raytrace, cache);
                        cache.recording = nullptr;
                        origins[slot] = ray.origin;
                    }
#pragma omp critical
                    {
                        running.statistics.merge(cache.statistics);
                    }
                    StatisticsCookie::current = nullptr;
                }

                // the validity radii from MinRecordPixels to twice the grid, in pixels at the record's distance: the
                // radius from MinRecordPixels on, the grid's cap apart (it bounds the record's validity, but not its
                // neighbours': they would shrink to the cap plus their distance, after the finer levels' candidates
                // took them as covering)
                auto levelRecords = std::vector<std::vector<IrradianceRecord>>(sets);
                for (auto slot = Zero<ASizeT>(); slot < slots.size(); ++slot)
                {
                    auto record = slots[slot];
                    if (!(record.radius > Zero<Float>()))
                    {
                        continue;
                    }
                    const auto pixelSize = length3(record.position - origins[slot]) * pixelAngle / IrradianceAccuracy;
                    record.radius = max(record.radius, MinRecordPixels * pixelSize);
                    record.maxRadius = static_cast<Float>(2 * grid) * pixelSize;
                    levelRecords[slot % sets].push_back(record);
                }
                irradianceCache->addLevel(levelRecords);
            }
            Log.info([records = irradianceCache->size()]
            {
                return "Irradiance cache: " + std::to_string(records) + " records";
            });
        }

        // Adaptive sampling: every pixel takes its first round of sub-samples, then the further rounds its neighbours'
        // spread asks for (AdaptiveRounds), each round on the grid shifted within the sub-samples' cells (Roberts' R2
        // sequence) and at its own hero wavelengths. Its sub-samples continue the pixel's sample sequence
        // (Raytrace::stratum).
        auto adaptive =
            AdaptiveRounds(adaptiveRounds > Zero<ASizeT>() ? running.image->getResolution() : Zero<Size2>());

        // the denoiser's guides per pixel, summed over the hits of its first round's sub-samples: normal (x, y, z) and
        // depth (w), albedo (x, y, z) and the number of hits (w); and with several sub-samples, the moments of their
        // compressed luminance for the variance of the pixel's mean (count, sum, sum of squares), every round's
        const auto guidesResolution = denoising ? running.image->getResolution() : Zero<Size2>();
        auto guides = HDRImage(guidesResolution);
        auto albedos = HDRImage(guidesResolution);
        const auto subSampleMoments = denoising && subSamplesCount > One<Size2::ValueType>();
        auto moments = HDRImage(subSampleMoments ? guidesResolution : Zero<Size2>());
        const auto addMoments = [&moments](const ASizeT imageIndex, const Float4& rgb)
        {
            const auto l = ATrousDenoiser::compressedLuminance(rgb);
            moments[imageIndex] += Float4(One<Float>(), l, l * l, Zero<Float>());
        };
        const auto refinePixel = [&](Float4* const outputPixel, const ASizeT rounds, RaytracerCache& cache)
        {
            const auto& configuration = cache.configuration;
            const auto imageIndex = static_cast<ASizeT>(outputPixel - configuration.image->getData());
            const auto width = x(configuration.image->getResolution());
            const auto pixelXYf = convert<Float4>(Size2(imageIndex % width, imageIndex / width));
            const auto nearTL = packets.pixelNearTopLeft(pixelXYf);
            const auto farTL = packets.pixelFarTopLeft(pixelXYf);
            const auto cullingMask = cullingOrientationToMask(configuration.cullingOrientation);
            const auto strataSeed = primarySeed(imageIndex, Zero<ASizeT>(), configuration.seedOffset);
            const auto context = pixelContext(Size2(imageIndex % width, imageIndex / width), perSubSample);

            auto sum = Zero<Float4>();
            for (auto round = One<ASizeT>(); round <= rounds; ++round)
            {
                const auto shift =
                    sequencePoint2D(Zero<Float4>(), round) - Float4(Half<Float>(), Half<Float>(), Zero<Float>(),
                                                                    Zero<Float>());
                const auto shiftedNearTL =
                    nearTL + packets.ssNearRightDir * x(shift) + packets.ssNearBottomDir * y(shift);
                const auto shiftedFarTL = farTL + packets.ssFarRightDir * x(shift) + packets.ssFarBottomDir * y(shift);
                for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount; ++s)
                {
                    const auto subSample = round * subSamplesCount + s;
                    const auto rayCast = RayCast(
                        packets.setupRayOfSampleInPixel(s, shiftedNearTL, shiftedFarTL), cullingMask, Zero<Size2>(),
                        configuration.maxDistance
                    );
                    auto raytrace = Raytrace(
                        rayCast, nullptr, Zero<ASizeT>(), One<Float>(),
                        &spectralContexts[subSample * perSubSample + context],
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
                    if (subSampleMoments)
                    {
                        addMoments(imageIndex, displayRGB.value);
                    }
                }
            }
            // the pixel holds the mean of the first round
            const auto mean =
                (Float4(outputPixel) + sum * packets.ssAvg) / Float4(static_cast<Float>(rounds + One<ASizeT>()));
            store(mean, outputPixel);
        };

#pragma omp parallel
        {
            // per worker states
            auto cache = RaytracerCache(running);
            cache.irradianceCache = irradianceCache.get();
            StatisticsCookie::current = &cache.statistics;

            // iterate over packets; with adaptive sampling, every pixel's first round before any further one
#pragma omp for schedule(static, 1)
            for (int p = Zero<int>(); p < packets.getPacketCount(); ++p)
            {
                const auto packedRaytraces = constructPackedRaytracesList(
                    p, packets, cache.configuration, spectralContexts, perSubSample
                );
                cache.statistics.primaryRays += packedRaytraces.size();

                // a pixel's sub-samples follow each other in the list
                auto spread = SubSampleSpread();
                for (auto i = Zero<ASizeT>(); i < packedRaytraces.size(); ++i)
                {
                    const auto& packedRaytrace = packedRaytraces[i];
                    // Timing for each pixel: Read start-time
                    const auto start = perPixelTiming();

                    auto features = PixelFeatures();
                    auto raytrace = packedRaytrace.raytrace;
                    if (denoising)
                    {
                        raytrace.features = &features;
                    }
                    const auto hit = trace(raytrace, cache);
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

                    if (subSampleMoments)
                    {
                        addMoments(static_cast<ASizeT>(imagePtrIndex), displayRGB.value);
                    }
                    if (denoising && features.depth > Zero<Float>())
                    {
                        guides[static_cast<ASizeT>(imagePtrIndex)] += blend<false, false, false, true>(
                            features.normal, Float4(features.depth)
                        );
                        albedos[static_cast<ASizeT>(imagePtrIndex)] += blend<false, false, false, true>(
                            features.albedo, One<Float4>()
                        );
                    }

                    if (adaptiveRounds > Zero<ASizeT>())
                    {
                        spread.add(displayRGB.value);
                        if ((i + One<ASizeT>()) % subSamplesCount == Zero<ASizeT>())
                        {
                            adaptive.setFirstRoundError(static_cast<ASizeT>(imagePtrIndex), spread.error());
                            spread = SubSampleSpread();
                        }
                    }
                }
            } // p

            // the further rounds, where the pixels' neighbours spread
            if (adaptiveRounds > Zero<ASizeT>())
            {
                const auto& configuration = cache.configuration;
#pragma omp for schedule(dynamic, 1) nowait
                for (int p = Zero<int>(); p < packets.getPacketCount(); ++p)
                {
                    const auto packetStart = packets.packetStartOf(p);
                    const auto packetLength = packets.clampPacketLength(configuration.resolution, packetStart);
                    const auto pixelsCount = RaytracerPackets::packetPixelsCount(packetLength);
                    for (auto cp = Zero<Size2::ValueType>(); cp < pixelsCount; ++cp)
                    {
                        const auto pixel = RaytracerPackets::coordsOfPixel(cp, packetStart, packetLength);
                        const auto rounds = adaptive.roundsOf(pixel, adaptiveRounds, configuration.adaptiveThreshold);
                        if (rounds == Zero<ASizeT>())
                        {
                            continue;
                        }
                        const auto imageIndex =
                            RaytracerPackets::imageIndexOfPixel(pixel, configuration.image->getResolution());
                        const auto start = perPixelTiming();
                        refinePixel(&configuration.image->getData()[imageIndex], rounds, cache);

                        // the timing of the pixel's last sub-sample and of its further rounds (log(ticks + 1) each)
                        const auto timing = configuration.timingMap->getData() + imageIndex;
                        const auto ticks =
                            vectorization::exp(Float4(timing)) + vectorization::exp(perPixelTiming(start));
                        store(vectorization::log(ticks - One<Float4>()), timing);
                    }
                }
            }

#pragma omp critical
            {
                // merge per worker statistics to one global statistic
                running.statistics.merge(cache.statistics);
            }
            StatisticsCookie::current = nullptr;
        }

        if (denoising)
        {
            // the guides' means over the hits; pixels without any keep depth 0
            const auto count = static_cast<int>(x(guidesResolution) * y(guidesResolution));
#pragma omp parallel for
            for (int i = 0; i < count; ++i)
            {
                const auto index = static_cast<ASizeT>(i);
                const auto hits = wwww(albedos[index]);
                if (!(x(hits) > Zero<Float>()))
                {
                    continue;
                }
                const auto depth = w(guides[index]) / x(hits);
                guides[index] = blend<false, false, false, true>(normalize3(zeroW(guides[index])), Float4(depth));
                albedos[index] = zeroW(albedos[index] / hits);
            }
            ATrousDenoiser(guides, albedos).denoise(*running.image, subSampleMoments ? &moments : nullptr);
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
        // One Fresnel reflectance for the whole specular lobe, its highlights and reflections alike: Schlick's of the
        // specular channel on an opaque surface, the indices' at a dielectric interface (the side the ray comes from
        // first), times the reflectance channel as the lobe's weight
        const auto interfaceEta =
            select(leavingMaterial, yxwz(brdf.surface.refractionEta), brdf.surface.refractionEta);
        const auto transmissive = spectral::spectralMax(brdf.surface.transmittance) > Zero<Float>();
        brdf.fresnel = transmissive
                           ? brdf::SpecularFresnel::dielectricInterface(brdf.surface.reflectance.data, interfaceEta)
                           : brdf::SpecularFresnel::opaque(brdf.surface.reflectance.data, brdf.surface.specular.data);
        brdf.reflectanceCoefficient = brdf.fresnel.reflectance(abs(brdf.intersection.smoothedNdotI));
        const auto maxDistance = raytrace.rayCast.maxDistance - brdf.viewDistance;

        // A rough dielectric whose refracted lobe is wider than a mirror's reflects and refracts through sampled
        // microfacet normals, each with its own Fresnel reflectance (Walter et al. 2007). A narrower refracted lobe
        // refracts as a mirror (as reflections do up to MirrorRoughness), both lobes with the smooth surface's Fresnel.
        const auto roughInterface = !totalInternalReflection
            && cache.configuration.glossySamples > Zero<ASizeT>()
            && transmissive
            && refractedLobeRoughness(x(brdf.surface.roughness), x(interfaceEta)) > brdf::MirrorRoughness;
        const auto* const microfacetEta = roughInterface ? &interfaceEta : nullptr;

        // the diffuse base under the specular lobe (Kelemen and Szirmay-Kalos 2001): it gets what the lobe leaves of
        // the light at the light's direction (sampleLighting, traceDiffuseReflection) and reflects what the lobe leaves
        // at the view direction, per what it leaves of light from all directions, so that both reflect at most the
        // light
        if (brdf.fresnel.reflects() && spectral::spectralMax(brdf.surface.diffusion) > Zero<Float>())
        {
            const auto viewShare = One<spectral::SpectralVector>() - brdf.fresnel.directionalAlbedo(
                x(abs(brdf.intersection.smoothedNdotI)), x(brdf.surface.roughness)
            );
            brdf.diffuseCoupling = viewShare / max(uniformDiffuseShare(brdf), Epsilon<spectral::SpectralVector>());
        }

        // the area lights weigh the shadow rays of their highlights against the glossy rays
        const auto glossyRays = cache.configuration.sphereLightSamples > Zero<ASizeT>()
                                    ? glossyRaysOf(raytrace, maxDistance, cache.configuration, brdf)
                                    : Zero<ASizeT>();
        const auto adapted_visibility_cutoff = SceneShader::adaptedVisibilityCutoff(
            cache.configuration.visibilityCutoff, raytrace.importance);
        // many lights are sampled where a diffuse ray's hit adds to the irradiance of the hit it came from
        const auto pointLightSamples =
            raytrace.diffuseBounce ? cache.configuration.manyLightSamples : Zero<ASizeT>();
        brdf.lighting = cache.configuration.sceneShader->sampleLighting(
            raytrace, adapted_visibility_cutoff, x(brdf.surface.roughness), brdf.fresnel,
            brdf.intersection, cache.configuration.sphereLightSamples, glossyRays, pointLightSamples,
            cache.shadowCache, cache.statistics
        );

        cache.statistics.shadingTicks += __rdtsc() - shadingStart;

        // the denoiser's guides: this hit's, unless a smooth mirror or glass passes them on to the hit whose image it
        // shows (glass: the refracted one)
        auto* reflectedFeatures = static_cast<PixelFeatures*>(nullptr);
        auto* transmittedFeatures = static_cast<PixelFeatures*>(nullptr);
        if (raytrace.features)
        {
            auto& features = *raytrace.features;
            const auto facesViewer = brdf.intersection.smoothedNdotI < Zero<Float4>();
            features.normal =
                zeroW(select(facesViewer, brdf.intersection.smoothedNormal, -brdf.intersection.smoothedNormal));
            // the diffusion's RGB, the same at every hero wavelength (the pixels' wavelengths vary in previews)
            features.albedo = zeroW(objectShader.sampleDiffusion(brdf.intersection));
            features.depth += brdf.viewDistance;
            if (!(spectral::spectralMax(brdf.surface.diffusion) > Zero<Float>()))
            {
                const auto transmits = transmissive && !totalInternalReflection;
                if (transmits && !microfacetEta)
                {
                    transmittedFeatures = raytrace.features;
                }
                else if (!transmits && !(x(brdf.surface.roughness) > brdf::MirrorRoughness))
                {
                    reflectedFeatures = raytrace.features;
                }
            }
        }

        traceDiffuseReflection(raytrace, maxDistance, cache, brdf);
        // the primary ray of an irradiance record needs nothing else
        if (cache.recording && raytrace.traceDepth == Zero<ASizeT>())
        {
            return {brdf.viewDistance, spectral::Spectrum::zero()};
        }
        traceReflection(raytrace, maxDistance, microfacetEta, reflectedFeatures, cache, brdf);
        traceTransmission(
            raytrace, maxDistance, leavingMaterial, transmittedDirection, microfacetEta, transmittedFeatures, cache,
            brdf
        );

        return {brdf.viewDistance, brdf::applyBRDF(brdf)};
    }

    void Raytracer::traceReflection(
        const Raytrace& incidentRaytrace, const Float maxDistance, const Float4* const microfacetEta,
        PixelFeatures* const features, RaytracerCache& cache, BRDFParameters& brdf
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
        reflectedRaytrace.diffuseBounce = incidentRaytrace.diffuseBounce;
        reflectedRaytrace.features = features;
        const auto reflectedHit = trace(reflectedRaytrace, cache);

        brdf.lighting.reflected = reflectedHit.color * brdf.reflectanceCoefficient;

        cache.statistics.secondaryRays += One<ASizeT>();
        cache.statistics.missedSecondaryRays +=
            static_cast<ASizeT>(outOfReach(reflectedRaytrace.rayCast, x(reflectedHit.depth)));
    }

    // Averages reflection rays in directions of microfacet normals drawn from the GGX distribution of visible normals
    // (Heitz 2018), each weighted by Smith's masking of its direction and its microfacet's Fresnel reflectance (the
    // specular lobe's, as the highlights take it). Directions below the surface count as black. With shadow rays to
    // area lights, a ray that finds a light's emitter adds the light to the highlight instead, weighed against the
    // light's shadow rays (multiple importance sampling, SceneShader::sampleLighting). At a dielectric interface, the
    // microfacets' Fresnel reflectance (1 for a microfacet that reflects totally, Walter et al. 2007) holds only where
    // the refraction samples microfacets too (microfacetEta); with a mirror refraction, both lobes take the smooth
    // surface's, so that they keep the energy.
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
        const auto perMicrofacet = !brdf.fresnel.isDielectric() || microfacetEta;

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
            reflectedRaytrace.diffuseBounce = incidentRaytrace.diffuseBounce;
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
                    intersection, direction * x(reflectedHit.depth), N, V, direction, roughness, brdf.fresnel,
                    samples, lightSamples
                );
                continue;
            }
            // the clamp against fireflies scales the whole spectrum, so that the colour stays
            const auto fresnel =
                perMicrofacet ? brdf.fresnel.reflectance(dot3v(V, microfacetNormal)) : brdf.reflectanceCoefficient;
            reflected += spectral::limitPeak(reflectedHit.color, cache.configuration.glossyClamp)
                * (fresnel * spectral::SpectralVector(brdf::smithG1(NoL, roughness)));
        }
        const auto perSample = One<Float>() / static_cast<Float>(samples);
        brdf.lighting.reflected = reflected * perSample;
        if (weighsEmitters)
        {
            brdf.lighting.specular += spectral::Spectrum{highlight * spectral::SpectralVector(perSample)};
        }
    }

    // Gathers the light of other surfaces at a diffuse hit (one bounce of interreflection): up to diffuseSamples rays
    // in directions of Lambert's distribution around the shading normal (cos(theta) / pi, brdf::sampleLambert), fewer
    // the less the diffuse reflection adds to the pixel. Pi times their mean radiance is the irradiance they bring,
    // which adds to the lights' (applyBRDF reflects diffusion / pi of it). A direction below the surface counts as
    // black, an area light's emissive object too (its light is the shadow rays'); a ray that leaves the scene brings
    // the background. The rays replace the ambient light, the stand-in for the light they gather; their hits trace no
    // diffuse rays and keep it, as the stand-in for the further bounces (Ward's ambient value).
    void Raytracer::traceDiffuseReflection(
        const Raytrace& incidentRaytrace, const Float maxDistance, RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        const auto& configuration = cache.configuration;
        const auto diffuseWeight = spectral::spectralMax(brdf.surface.diffusion);
        const auto diffuseImportance = incidentRaytrace.importance * diffuseWeight;
        if (configuration.diffuseSamples == Zero<ASizeT>() || incidentRaytrace.diffuseBounce
            || incidentRaytrace.traceDepth >= configuration.maxTraceDepth || maxDistance <= Zero<Float>()
            || !(diffuseWeight > Zero<Float>()) || diffuseImportance < configuration.visibilityCutoff)
        {
            return;
        }

        const auto& intersection = brdf.intersection;
        // the normals on the side the ray comes from (smoothedNdotI is the same in every lane)
        const auto facesViewer = intersection.smoothedNdotI < Zero<Float4>();
        const auto N = select(facesViewer, intersection.smoothedNormal, -intersection.smoothedNormal);
        const auto geometricNormal = select(facesViewer, intersection.surfaceNormal, -intersection.surfaceNormal);

        // irradiance caching: the records valid here instead of the rays, where they cover the hit fully; where they
        // cover it partly, blended with the rays by their coverage, so that no seam shows between the two. The
        // primary ray of a record records one where the records don't cover the hit fully yet.
        const auto recording = cache.recording && incidentRaytrace.traceDepth == Zero<ASizeT>();
        auto interpolated = spectral::Spectrum::zero();
        auto coverage = Zero<Float>();
        auto context = Zero<ASizeT>();
        if (const auto* const irradianceCache = cache.irradianceCache)
        {
            context = irradianceCache->contextIndex(incidentRaytrace.spectralContext);
            if (context < irradianceCache->getContextCount())
            {
                coverage = min(
                    One<Float>(),
                    irradianceCache->interpolate(context, intersection.vertex, N, interpolated) / FullCoverage
                );
            }
            if (!(coverage < One<Float>()))
            {
                if (!recording)
                {
                    brdf.lighting.diffuse += interpolated * uniformDiffuseShare(brdf);
                    brdf.lighting.ambient = spectral::Spectrum::zero();
                }
                return;
            }
        }

        const auto diffuseVisibilityIndex = incidentRaytrace.visibilityIndex * diffuseWeight;
        const auto samples = recording
                                 ? max(MinRecordRays, configuration.diffuseSamples * RecordRaysPerDiffuseSample)
                                 : diffuseReflectionSamples(configuration.diffuseSamples, diffuseVisibilityIndex);
        const auto sampleVisibilityIndex = diffuseVisibilityIndex / static_cast<Float>(samples);
        const auto sequenceOffset = sequenceOffset2D(deriveSeed(incidentRaytrace.strataSeed, DiffuseSequenceSeed));
        alignas(YMM_ALIGNMENT) UInt_32 diffuseSeeds[UInt8_32::SIZE];

        auto gathered = spectral::Spectrum::zero();
        // the reciprocal distances of the rays' hits, for a record's harmonic mean distance (rays that leave the scene
        // don't count: through a seam, they would make the record look far from everything)
        auto reciprocalDistances = Zero<Float>();
        auto hits = Zero<Float>();
        // a record's rays, for the limit of its outliers
        auto recordRays = std::vector<spectral::Spectrum, AlignedAllocator<spectral::Spectrum>>();
        if (recording)
        {
            recordRays.reserve(samples);
        }
        for (auto sample = Zero<ASizeT>(); sample < samples; ++sample)
        {
            const auto lane = sample % UInt8_32::SIZE;
            if (lane == Zero<ASizeT>())
            {
                store(deriveSeeds(incidentRaytrace.seed, DiffuseSeeds + static_cast<UInt_32>(sample)), diffuseSeeds);
            }
            const auto xi =
                sequencePoint2D(sequenceOffset, stratifiedIndex(incidentRaytrace.stratum, samples, sample));
            const auto direction = brdf::sampleLambert(N, xi);
            if (dot3(geometricNormal, direction) <= Zero<Float>())
            {
                continue;
            }

            const auto diffuseRayCast = RayCast(
                Ray(intersection.vertex, direction), incidentRaytrace.rayCast.cullingMask,
                Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
                maxDistance
            );
            auto diffuseRaytrace = Raytrace(
                diffuseRayCast, &intersection, incidentRaytrace.traceDepth + One<ASizeT>(), sampleVisibilityIndex,
                incidentRaytrace.spectralContext, diffuseSeeds[lane]
            );
            // each ray estimates the whole irradiance: the cutoffs see the diffuse reflection's importance
            diffuseRaytrace.importance = diffuseImportance;
            diffuseRaytrace.glossy = true;
            diffuseRaytrace.diffuseBounce = true;
            const auto diffuseHit = trace(diffuseRaytrace, cache);

            cache.statistics.secondaryRays += One<ASizeT>();
            const auto missed = outOfReach(diffuseRaytrace.rayCast, x(diffuseHit.depth));
            cache.statistics.missedSecondaryRays += static_cast<ASizeT>(missed);
            if (!missed)
            {
                reciprocalDistances += reciprocal(max(x(diffuseHit.depth), std::numeric_limits<Float>::min()));
                hits += One<Float>();
            }

            // the firefly clamp of the glossy rays: a ray that finds a light through a mirror or glass stays bounded
            const auto ray = spectral::limitPeak(diffuseHit.color, configuration.glossyClamp);
            gathered += ray;
            if (recording)
            {
                recordRays.push_back(ray);
            }
        }
        const auto traced = gathered * (Pi<Float>() / static_cast<Float>(samples));
        if (recording)
        {
            // rays that all leave the scene: the largest radius the clamp allows
            auto limit = Zero<Float>();
            for (auto pass = 0; pass < 2; ++pass)
            {
                auto peaks = Zero<Float>();
                for (const auto& ray : recordRays)
                {
                    const auto peak = spectral::spectralMax(ray);
                    peaks += limit > Zero<Float>() ? min(peak, limit) : peak;
                }
                limit = RecordOutlierFactor * peaks / static_cast<Float>(samples);
            }
            auto limited = spectral::Spectrum::zero();
            if (limit > Zero<Float>())
            {
                for (const auto& ray : recordRays)
                {
                    limited += spectral::limitPeak(ray, limit);
                }
            }
            auto& record = *cache.recording;
            record.position = intersection.vertex;
            record.normal = N;
            record.irradiance = limited * (Pi<Float>() / static_cast<Float>(samples));
            record.radius = hits > Zero<Float>() ? hits / reciprocalDistances : std::numeric_limits<Float>::max();
            record.context = context;
        }
        const auto irradiance = recording || !(coverage > Zero<Float>())
                                    ? traced
                                    : spectral::Spectrum{
                                        interpolated.data * spectral::SpectralVector(coverage)
                                        + traced.data * spectral::SpectralVector(One<Float>() - coverage)
                                    };
        brdf.lighting.diffuse += irradiance * uniformDiffuseShare(brdf);
        brdf.lighting.ambient = spectral::Spectrum::zero();
    }

    void Raytracer::traceTransmission(
        const Raytrace& incidentRaytrace, const Float maxDistance, const bool leavingMaterial,
        const Float4& transmittedDirection, const Float4* const microfacetEta, PixelFeatures* const features,
        RaytracerCache& cache, BRDFParameters& brdf
    )
    {
        // would still be visible, but tracing is not wanted any more, set transmitted to the background; a total
        // internal reflection transmits nothing (a lobe of a weight below 1 doesn't reflect all there either)
        if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>()
            || allTrue3(!transmittedDirection))
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

        // check whether it would even make a difference in the image: what the specular lobe and the diffusion leave
        const auto smoothTransmission =
            (One<spectral::SpectralVector>() - brdf.reflectanceCoefficient) * undiffusedShare(brdf);
        const auto transmissionWeight =
            spectral::spectralMax(fractionTransmitted) * spectral::spectralMax(smoothTransmission);
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
        refractedRaytrace.diffuseBounce = incidentRaytrace.diffuseBounce;
        refractedRaytrace.features = features;

        const auto refractedHit = trace(refractedRaytrace, cache);
        brdf.lighting.transmitted = refractedHit.color * smoothTransmission;
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
    // the refracted direction, as for the glossy reflection), what its microfacet's Fresnel reflectance (the specular
    // lobe's, its weight included) leaves and its absorption. A microfacet that reflects totally transmits nothing
    // (its light is in the reflection), and a direction that doesn't cross the surface is masked: both count as black.
    // The refracted lobe is narrower than the reflected one and takes fewer rays (refractedLobeRoughness).
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
        const auto undiffused = undiffusedShare(brdf);

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
            const auto weight = spectral::SpectralVector(brdf::smithG1(NoT, roughness)) * undiffused
                * (One<spectral::SpectralVector>() - brdf.fresnel.reflectance(dot3v(V, microfacetNormal)));

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
            refractedRaytrace.diffuseBounce = incidentRaytrace.diffuseBounce;
            const auto refractedHit = trace(refractedRaytrace, cache);

            const auto absorption = leavingMaterial
                                        ? One<spectral::SpectralVector>()
                                        : beerLambertTransmittance(
                                            Float8(refractedHit.depth), brdf.surface.transmittance.data);
            transmitted += spectral::Spectrum{refractedHit.color.data * absorption * weight};

            cache.statistics.secondaryRays += One<ASizeT>();
            cache.statistics.missedSecondaryRays +=
                static_cast<ASizeT>(outOfReach(refractedRaytrace.rayCast, x(refractedHit.depth)));
        }
        // the absorption is in each ray's share already
        brdf.lighting.transmitted = transmitted * (One<Float>() / static_cast<Float>(samples));
        brdf.fractionTransmitted = One<spectral::SpectralVector>();
    }
}
