#include "raytracing/Raytracer.h"
#include "../stdafx.h"

#include "raytracing/RaytracerPackets.h"

#include "raytracing/shading/ObjectShader.h"

#include "raytracing/common/StatisticsCookie.h"
#include "raytracing/common/Tools.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <omp.h>
#include <profileapi.h>

namespace raytracer {
  Raytracer::Raytracer() : runId(), running(), current() {
  }

  Raytracer::~Raytracer() = default;

  RaytraceConfiguration Raytracer::getRunning() const {
    return running;
  }

  void Raytracer::requestUpdate() {
    if (current.runId == running.runId) {
      return;
    }

    running = current;
    running.image = new HDRImage(running.resolution);
    running.depthMap = new HDRImage(running.resolution);
    running.timingMap = new HDRImage(running.resolution);

    // start raytracing
    LARGE_INTEGER frequency, start, stop;
    QueryPerformanceFrequency(&frequency);

    std::cout << "Raytrace " << running.runId << " (" << x(running.resolution) << "x" << y(running.resolution)
              << "):" << std::endl;

    QueryPerformanceCounter(&start);
    trace();
    QueryPerformanceCounter(&stop);

    auto timeDuration = static_cast<Int_64>(stop.QuadPart - start.QuadPart);
    auto timeFrequency = static_cast<Int_64>(frequency.QuadPart);
    std::cout << "Duration: " << convert<Float_64>(timeDuration) / convert<Float_64>(timeFrequency) << "s" << std::endl;
  }

  void Raytracer::trigger(const RaytraceParameters &parameters) {
    assert(parameters.observer);
    assert(parameters.camera);
    assert(parameters.sceneShader);

    runId += 1;

    auto samplingResolution =
        max(One<Size2>(), convert<Size2>(convert<Float4>(parameters.resolution) * parameters.samplingFactor));

    // build raytrace configuration
    current = parameters; // copies parameters into current
    current.maxDistance =
        select(parameters.maxDistance > Zero<Float>(), parameters.maxDistance, std::numeric_limits<Float>::max());
    current.resolution = max(One<Size2>(), samplingResolution);
    current.state = false;
    current.runId = runId;
  }

  Int_64 perPixelTiming() {
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);
    return static_cast<Int_64>(start.QuadPart);
  }

  // Timing for each pixel: Read end-time from clock and calculate differences
  Float4 perPixelTiming(const Int_64 start) {
    LARGE_INTEGER stop;
    QueryPerformanceCounter(&stop);
    auto timeStop = static_cast<Int_64>(stop.QuadPart);
    return Float4(vectorization::log(convert<Float4::ValueType>(timeStop - start + Int_64{1})));
  }

  struct PackedRaytrace {
    typedef std::vector<PackedRaytrace, AlignedAllocator<PackedRaytrace>> ListType;

    Raytrace raytrace;
    Float4 *outputPixel;

    PackedRaytrace() : raytrace(), outputPixel() {
    }

    PackedRaytrace(const Raytrace &raytraceIn, Float4 *const outputPixelIn)
        : raytrace(raytraceIn), outputPixel(outputPixelIn) {
    }
  };

  PackedRaytrace::ListType constructPackedRaytracesList(
      const int packetIndex, const RaytracerPackets &packets, const RaytraceConfiguration &configuration
  ) {
    auto nearPlanePixel = packets.packetStartOf(packetIndex);
    auto minPacketLength = packets.clampPacketLength(configuration.resolution, nearPlanePixel);

    // iterate over camera-pixels cp in packet
    auto pixelsCount = RaytracerPackets::packetPixelsCount(minPacketLength);
    auto subSamplesCount = packets.pixelSubSamplesCount();

    auto list = PackedRaytrace::ListType();
    list.reserve(pixelsCount * subSamplesCount);

    for (auto cp = Zero<Size2::ValueType>(); cp < pixelsCount; cp++) {
      // address camera-point cp to output pixel
      auto cpXY = RaytracerPackets::coordsOfPixel(cp, nearPlanePixel, minPacketLength);
      auto cpXYf = convert<Float4>(cpXY);
      auto nearTL = packets.pixelNearTopLeft(cpXYf);
      auto farTL = packets.pixelFarTopLeft(cpXYf);
      auto imageIndex = RaytracerPackets::imageIndexOfPixel(cpXY, configuration.image->getResolution());
      auto outputPixel = &configuration.image->getData()[imageIndex];

      // generate super sampling rays
      for (auto s = Zero<Size2::ValueType>(); s < subSamplesCount;) {
        // interpolate the camera-point's top-left on the view-plane
        auto newRay = packets.setupRayOfSampleInPixel(s++, nearTL, farTL);
        auto newRayCast = RayCast(
            newRay, cullingOrientationToMask(configuration.cullingOrientation), Zero<Size2>(), configuration.maxDistance
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
  void Raytracer::trace() {
    auto packets = RaytracerPackets(running);

#pragma omp parallel
    {
      // per worker states
      auto cache = RaytracerCache(running);

      // iterate over packets
#pragma omp for schedule(static, 1) nowait
      for (int p = Zero<int>(); p < packets.getPacketCount(); ++p) {
        auto packedRaytraces = constructPackedRaytracesList(p, packets, cache.configuration);
        cache.statistics.primaryRays += packedRaytraces.size();

        for (const auto &packedRaytrace : packedRaytraces) {
          // Timing for each pixel: Read start-time from clock
          auto start = perPixelTiming();

          auto hit = trace(packedRaytrace.raytrace, cache);
          cache.statistics.missedPrimaryRays +=
              static_cast<ASizeT>(outOfReach(packedRaytrace.raytrace.rayCast, x(hit.depth)));

          // Sample color into output
          packets.samplePixel(packedRaytrace.outputPixel, hit.color);

          auto imagePtrIndex = packedRaytrace.outputPixel - cache.configuration.image->getData();

          // Fill in depth information and store it in depthMap
          auto pixelDepthMask = x_yzw(Infinity<Float4>(), NegativeInfinity<Float4>());
          auto depth = packets.superSampledPixelDepth(RaytracerPackets::samplePixelDepth(pixelDepthMask, hit.depth));
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
    }

    auto hasFinished = (running.runId == runId);
    running.state = hasFinished;
    if (hasFinished) {
      running.timingMap->normalizeEachChannel();
      running.depthMap->normalizeEachChannel();

      // notify completion
      running.observer->notifyUpdate(running);
    }
  }

  Float4 schlickFresnel(const bool enteringLessDense, const Float4 &negNdotI, const Float4 &eta) {
    // calculate cosPhiT from Snell's law (TIR is already excluded)
    Float4 cosPhi;
    if (enteringLessDense) {
      auto etaItoT = xxxx(eta);
      auto etaItoTMulNdotI = etaItoT * negNdotI;
      auto cosSqrPhiT = One<v_f32_4>() - (etaItoT * etaItoT - etaItoTMulNdotI * etaItoTMulNdotI);
      assert(!isNegative(cosSqrPhiT));
      cosPhi = sqrt(cosSqrPhiT);
    } else {
      cosPhi = max(Zero<Float4>(), negNdotI);
    }

    // Schlick approximation for polarized light refraction/reflection model:
    // R0 = ((eta_i - eta_t) / (eta_i + eta_t))²
    auto refractions = subadd(zzzz(eta), wwww(eta));
    auto sqrtR0 = xxxx(refractions) / yyyy(refractions);
    auto R0 = sqrtR0 * sqrtR0;
    auto oneSubCosPhi = One<Float4>() - cosPhi;
    auto sqrOneSubCosPhi = oneSubCosPhi * oneSubCosPhi;
    auto reflectance = R0 + (One<Float4>() - R0) * oneSubCosPhi * sqrOneSubCosPhi * sqrOneSubCosPhi;
    return reflectance;
  }

  Float4 fresnelReflectance(const bool totalInternalReflection, const Float4 &negNdotI, const Float4 &eta) {
    // entering less dense material and TIR is the case
    auto enteringLessDense = !!isNegative(wwww(eta) - zzzz(eta));
    if (totalInternalReflection & enteringLessDense) {
      return One<Float4>();
    }
    return schlickFresnel(enteringLessDense, negNdotI, eta);
  }

  // checks intersections, applies lighting and texturing and shoots secondary rays
  IlluminatedIntersection Raytracer::trace(const Raytrace &raytrace, RaytracerCache &cache) {
    BRDFParameters brdf;

    // Find nearest intersection
    brdf.viewDistance = cache.configuration.sceneShader->findNearestIntersection(
        raytrace.rayCast, raytrace.originIntersection, brdf.intersection
    );
    if (outOfReach(raytrace.rayCast, brdf.viewDistance)) {
      return {
          raytrace.rayCast.maxDistance,
          cache.configuration.sceneShader->sampleBackground(raytrace.rayCast.ray.direction)
      };
    }

    // Sample surface properties at intersection
    assert(brdf.intersection.object);
    auto &objectShader = *dynamic_cast<const ObjectShader *const>(brdf.intersection.object);
    brdf.surface = objectShader(*cache.configuration.sceneShader, brdf.intersection);

    // Sample lighting properties at intersection
    brdf.lighting = cache.configuration.sceneShader->sampleLighting(
        raytrace, SceneShader::adaptedVisibilityCutoff(cache.configuration.visibilityCutoff, raytrace.visibilityIndex),
        brdf.surface.shininess, brdf.intersection, cache.shadowCache, cache.statistics
    );

    // Whitted raytracing part, reflection and transmission
    auto transmittedDirection = refract(
        raytrace.rayCast.cullingMask, raytrace.rayCast.ray.direction, brdf.intersection.smoothedNormal,
        brdf.intersection.smoothedNdotI, brdf.surface.refractionEta
    );
    auto leavingMaterial = frontfaceCulled(raytrace.rayCast)
                           | (notCulled(raytrace.rayCast) && isNegative(-brdf.intersection.smoothedNdotI));
    auto totalInternalReflection = allTrue3(!transmittedDirection);
    brdf.reflectanceCoefficient = fresnelReflectance(
        totalInternalReflection,
        select(leavingMaterial, brdf.intersection.smoothedNdotI, -brdf.intersection.smoothedNdotI),
        select(leavingMaterial, yxwz(brdf.surface.refractionEta), brdf.surface.refractionEta)
    );

    auto maxDistance = raytrace.rayCast.maxDistance - brdf.viewDistance;
    traceReflection(raytrace, maxDistance, cache, brdf);
    traceTransmission(raytrace, maxDistance, leavingMaterial, transmittedDirection, cache, brdf);

    return {brdf.viewDistance, applyBRDF(brdf)};
  }

  void Raytracer::traceReflection(
      const Raytrace &incidentRaytrace, const Float maxDistance, RaytracerCache &cache, BRDFParameters &brdf
  ) {
    // would still be visible but tracing is not wanted anymore
    if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>()) {
      brdf.lighting.reflected = Zero<Float4>();
      return;
    }

    // does it still add to the visibility in the image
    auto reflectionVisibilityIndex =
        incidentRaytrace.visibilityIndex * brdf.reflectanceCoefficient * max3v(brdf.surface.reflectance);
    if (x(reflectionVisibilityIndex) < cache.configuration.visibilityCutoff) {
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
        reflectedRayCast, &brdf.intersection, incidentRaytrace.traceDepth + One<ASizeT>(), x(reflectionVisibilityIndex)
    );
    auto reflectedHit = trace(reflectedRaytrace, cache);

    brdf.lighting.reflected = reflectedHit.color;

    cache.statistics.secondaryRays += One<ASizeT>();
    cache.statistics.missedSecondaryRays +=
        static_cast<ASizeT>(outOfReach(reflectedRaytrace.rayCast, x(reflectedHit.depth)));
  }

  void Raytracer::traceTransmission(
      const Raytrace &incidentRaytrace, const Float maxDistance, const bool leavingMaterial,
      const Float4 &transmittedDirection, RaytracerCache &cache, BRDFParameters &brdf
  ) {
    // would still be visible but tracing is not wanted anymore, set transmitted to background
    if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>()) {
      brdf.lighting.transmitted = Zero<Float4>();
      return;
    }

    Float4 fractionTransmitted;
    if (leavingMaterial) {
      // world vacuum does not absorb
      fractionTransmitted = One<Float4>();
    } else {
      // least possible transmitted fraction of material
      auto minDepth = Float4(std::numeric_limits<Float4::ValueType>::min());
      auto maxTransmittance = max3v(brdf.surface.transmittance);
      fractionTransmitted = vectorization::exp(-minDepth / maxTransmittance);
    }
    brdf.fractionTransmitted = fractionTransmitted;

    // does it still add to the visibility in the image or is it total internal reflection (transmissionDirection = 0)
    auto transmissionVisibilityIndex =
        Float4(incidentRaytrace.visibilityIndex) * fractionTransmitted * (One<Float4>() - brdf.reflectanceCoefficient);
    if (x(transmissionVisibilityIndex) < cache.configuration.visibilityCutoff) {
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
    if (!leavingMaterial) {
      brdf.fractionTransmitted = vectorization::exp(-refractedHit.depth / brdf.surface.transmittance);
    }

    cache.statistics.secondaryRays += One<ASizeT>();
    cache.statistics.missedSecondaryRays +=
        static_cast<ASizeT>(outOfReach(refractedRaytrace.rayCast, x(refractedHit.depth)));
  }

  Float4 Raytracer::applyBRDF(const BRDFParameters &brdf) {
    // https://en.wikipedia.org/wiki/Phong_reflection_model
    auto reflected =
        brdf.lighting.reflected * brdf.surface.reflectance + brdf.lighting.specular * brdf.surface.specular;

    // Transmittance model: Absorption coefficient/Beer-Lambert-law
    // http://tog.acm.org/resources/RTNews/html/rtnv10n1.html#art3
    // http://en.wikipedia.org/wiki/Absorption_coefficient
    // http://en.wikipedia.org/wiki/Beer%E2%80%93Lambert_law
    auto diffuse = (brdf.lighting.ambient + brdf.lighting.diffuse) * brdf.surface.diffusion;
    auto transmitted = mix(diffuse, brdf.lighting.transmitted, brdf.fractionTransmitted);

    return brdf.surface.emittance + mix(transmitted, reflected, brdf.reflectanceCoefficient);
  }
}
