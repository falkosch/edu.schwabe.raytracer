#include "raytracing/Raytracer.h"
#include "../stdafx.h"

#include "raytracing/RaytracerPackets.h"

#include "raytracing/shading/ObjectShader.h"

#include "raytracing/common/Tools.h"
#include "raytracing/common/StatisticsCookie.h"

#include <cassert>
#include <iostream>

#include <profileapi.h>
#include <omp.h>

namespace raytracer
{

	Raytracer::Raytracer() :
		runId(),
		running(),
		current()
	{ }

	Raytracer::~Raytracer() { }

	void Raytracer::requestUpdate()
	{
		if (current.runId == running.runId)
			return;

		running = current;
		running.image = new HDRImage(running.resolution);
		running.depthMap = new HDRImage(running.resolution);
		running.timingMap = new HDRImage(running.resolution);

		// start raytracing
		LARGE_INTEGER frequency, start, stop;
		QueryPerformanceFrequency(&frequency);

		std::cout << "Raytrace " << running.runId << " (" << x(running.resolution) << "x" << y(running.resolution) << "):" << std::endl;

		QueryPerformanceCounter(&start);
		trace();
		QueryPerformanceCounter(&stop);

		std::cout << "Duration: " << convert<Float_64>(stop.QuadPart - start.QuadPart) / convert<Float_64>(frequency.QuadPart) << "s" << std::endl;
	}

	void Raytracer::trigger(const RaytraceParameters & parameters)
	{
		assert(parameters.observer);
		assert(parameters.camera);
		assert(parameters.sceneShader);

		const Size2 samplingResolution = max(
			One<Size2>(),
			convert<Size2>(convert<Float4>(parameters.resolution) * parameters.samplingFactor)
		);

		// build raytrace configuration
		current = parameters; // copies parameters into current
		current.maxDistance = select(
			parameters.maxDistance > Zero<Float>(),
			parameters.maxDistance,
			std::numeric_limits<Float>::max()
		);
		current.resolution = max(One<Size2>(), samplingResolution);
		current.state = false;
		current.runId = ++runId;
	}

	const UInt_64 perPixelTiming()
	{
		LARGE_INTEGER start;
		QueryPerformanceCounter(&start);
		return static_cast<UInt_64>(start.QuadPart);
	}

	// Timing for each pixel: Read end-time from clock and calculate differences
	const Float4 perPixelTiming(const UInt_64 start)
	{
		LARGE_INTEGER stop;
		QueryPerformanceCounter(&stop);
		return Float4(
			vectorization::log(
				convert<Float4::ValueType>(stop.QuadPart - start + One<UInt_64>())
			)
		);
		//Float4 t = Float4(vectorization::log(
		//    static_cast<Float4::ValueType>(stop.QuadPart - start + One<LONGLONG>())));
		//// Set the color channel to one that has the index equal to the omp worker number
		//t[omp_get_thread_num()] = One<Float4::ValueType>();
		//return t;
	}

	struct PackedRaytrace
	{
		typedef std::vector<PackedRaytrace, AlignedAllocator<PackedRaytrace>> ListType;
		Raytrace raytrace;
		Float4 * outputPixel;
		ALIGNED_ALLOCATORS(__alignof(PackedRaytrace));
		PackedRaytrace() : raytrace(), outputPixel() {}
		PackedRaytrace(const Raytrace & raytraceIn, Float4 * const outputPixelIn)
			:
			raytrace(raytraceIn),
			outputPixel(outputPixelIn)
		{}
	};

	const PackedRaytrace::ListType constructPackedRaytracesList(const int packetIndex, const RaytracerPackets & packets, const RaytraceConfiguration & configuration)
	{
		// get origin pixel for packet p
		const Size2 pXY = packets.packetStartOf(packetIndex);
		// clamp packet length
		const Size2 minPacketLength = packets.clampedPacketLength(configuration.resolution, pXY);

		// iterate over camera-pixels cp in packet
		const Size2::ValueType pixelsCount = RaytracerPackets::packetPixelsCount(minPacketLength);
		const Size2::ValueType subsamplesCount = packets.pixelSubsamplesCount();

		PackedRaytrace::ListType list = PackedRaytrace::ListType();
		list.reserve(pixelsCount * subsamplesCount);

		for (Size2::ValueType cp = Zero<Size2::ValueType>(); cp < pixelsCount;)
		{
			// address camera-point cp
			const Size2 cpXY = RaytracerPackets::coordsOfPixel(cp++, pXY, minPacketLength);
			const Float4 cpXYf = convert<Float4>(cpXY);
			const Float4 nearTL = packets.pixelNearTopLeft(cpXYf);
			const Float4 farTL = packets.pixelFarTopLeft(cpXYf);

			// account supersampling-rays
			Float4 * const outputPixel = &configuration.image->getData()[
				packets.imageIndexOfPixel(cpXY, configuration.image->getResolution())
			];

			for (Size2::ValueType s = Zero<Size2::ValueType>(); s < subsamplesCount;)
			{
				// interpolate the camera-point's top-left on the view-plane
				list.push_back(
					PackedRaytrace(
						Raytrace(
							Raycast(
								packets.setupRayOfSampleInPixel(s++, nearTL, farTL),
								cullingOrientationToMask(configuration.cullingOrientation),
								Zero<Size2>(),
								configuration.maxDistance
							),
							nullptr,
							Zero<ASizeT>(),
							One<Float>()
						),
						outputPixel
					)
				);
			} // s
		} // cp

		return list;
	}

	// shoots the primary rays
	// It's better to not unproject every camera-pixel, but only the four
	// corners. So we only have to bilinear interpolate between the corner-
	// diretions while iterating. This eliminates many expensive
	// transformations and other calculations. (similar to phong-lighting)
	void Raytracer::trace()
	{
		const RaytracerPackets packets = RaytracerPackets(running);

#pragma omp parallel
		{
			// per worker states
			RaytracerCache cache = RaytracerCache(running);

			// iterate over packets
#pragma omp for schedule(static,1) nowait
			for (int p = Zero<int>(); p < packets.getPacketCount(); ++p)
			{
				const PackedRaytrace::ListType packedRaytraces = constructPackedRaytracesList(p, packets, cache.configuration);
				cache.statistics.primaryRays += packedRaytraces.size();

				for (PackedRaytrace::ListType::const_iterator it = packedRaytraces.cbegin(); it != packedRaytraces.cend(); ++it)
				{
					// Timing for each pixel: Read start-time from clock
					const UInt_64 start = perPixelTiming();

					const IlluminatedIntersection hit = trace(it->raytrace, cache);
					cache.statistics.missedPrimaryRays += static_cast<ASizeT>(outOfReach(it->raytrace.raycast, x(hit.depth)));

					// Sample color into output
					Float4 * const pixelDepth = (it->outputPixel - cache.configuration.image->getData()) + cache.configuration.depthMap->getData();
					Float4 * const pixelTiming = (it->outputPixel - cache.configuration.image->getData()) + cache.configuration.timingMap->getData();

					packets.samplePixel(it->outputPixel, hit.color);

					// Fill in depth information and store it in depthMap
					const Float4 pixelDepthMask = x_yzw(Infinity<Float4>(), NegativeInfinity<Float4>());
					store(
						packets.supersampledPixelDepth(
							RaytracerPackets::samplePixelDepth(pixelDepthMask, hit.depth)
						),
						pixelDepth
					);

					// Store timings
					store(perPixelTiming(start), pixelTiming);
				}

			} // p

#pragma omp critical
			{
				// merge per worker statistics to one global statistic
				running.statistics.merge(cache.statistics);
			}
		}

		const bool hasFinished = (running.runId == runId);
		running.state = hasFinished;
		if (hasFinished)
		{
			running.timingMap->normalizeEachChannel();
			running.depthMap->normalizeEachChannel();

			// notify completion
			running.observer->notifyUpdate(running);
		}
	}

	const Float4 schlickFresnel(const bool enteringLessDense, const Float4 & negNdotI, const Float4 & eta)
	{
		// calculate cosPhiT from snells law (TIR is already excluded)
		Float4 cosPhi;
		if (enteringLessDense)
		{
			const Float4 etaItoT = xxxx(eta);
			const Float4 etaItoTMulNdotI = etaItoT * negNdotI;
			const Float4 cosSqrPhiT = One<v_f32_4>() - (etaItoT * etaItoT - etaItoTMulNdotI * etaItoTMulNdotI);
			assert(!isNegative(cosSqrPhiT));
			cosPhi = sqrt(cosSqrPhiT);
		}
		else
		{
			cosPhi = max(Zero<Float4>(), negNdotI);
		}

		// Schlick approximation for polarized light refraction/reflection model:
		// R0 = ((eta_i - eta_t) / (eta_i + eta_t))²
		const Float4 refractions = subadd(zzzz(eta), wwww(eta));
		const Float4 sqrtR0 = xxxx(refractions) / yyyy(refractions);
		const Float4 R0 = sqrtR0 * sqrtR0;
		const Float4 oneSubCosPhi = One<Float4>() - cosPhi;
		const Float4 sqrOneSubCosPhi = oneSubCosPhi * oneSubCosPhi;
		const Float4 reflectance = R0 + (One<Float4>() - R0) * oneSubCosPhi * sqrOneSubCosPhi * sqrOneSubCosPhi;
		return reflectance;
	}

	const Float4 fresnelReflectance(const bool totalInternalReflection, const Float4 & negNdotI, const Float4 & eta)
	{
		// entering less dense material and TIR is the case
		const bool enteringLessDense = !!isNegative(wwww(eta) - zzzz(eta));
		if (totalInternalReflection & enteringLessDense)
		{
			return One<Float4>();
		}
		return schlickFresnel(enteringLessDense, negNdotI, eta);
	}

	// checks intersections, applies lighting and texturing and shoots secondary rays
	const IlluminatedIntersection Raytracer::trace(const Raytrace & raytrace, RaytracerCache & cache)
	{
		BRDFParameters brdf;

		// Find nearest intersection
		brdf.viewDistance = cache.configuration.sceneShader->findNearestIntersection(raytrace.raycast, raytrace.originIntersection, brdf.intersection);
		if (outOfReach(raytrace.raycast, brdf.viewDistance))
		{
			return IlluminatedIntersection(
				raytrace.raycast.maxDistance,
				cache.configuration.sceneShader->sampleBackground(raytrace.raycast.ray.direction));
		}

		// Sample surface properties at intersection
		assert(brdf.intersection.object);
		const ObjectShader & objectShader = *static_cast<const ObjectShader*const>(brdf.intersection.object);
		brdf.surface = objectShader(*cache.configuration.sceneShader, brdf.intersection);

		// Sample lighting properties at intersection
		brdf.lighting = cache.configuration.sceneShader->sampleLighting(
			raytrace,
			SceneShader::adaptedVisibilityCutoff(cache.configuration.visibilityCutoff, raytrace.visibilityIndex),
			brdf.surface.shininess,
			brdf.intersection,
			cache.shadowCache,
			cache.statistics);

		// Whitted raytracing part, reflection and transmission
		const Float4 transmittedDirection = refract(
			raytrace.raycast.cullingMask,
			raytrace.raycast.ray.direction,
			brdf.intersection.smoothedNormal,
			brdf.intersection.smoothedNdotI,
			brdf.surface.refractionEta);
		const bool leavingMaterial = frontfaceCulled(raytrace.raycast)
			| (notCulled(raytrace.raycast) && isNegative(-brdf.intersection.smoothedNdotI));
		const bool totalInternalReflection = allTrue3(!transmittedDirection);
		brdf.reflectanceCoefficient = fresnelReflectance(
			totalInternalReflection,
			select(leavingMaterial, brdf.intersection.smoothedNdotI, -brdf.intersection.smoothedNdotI),
			select(leavingMaterial, yxwz(brdf.surface.refractionEta), brdf.surface.refractionEta));

		const Float maxDistance = raytrace.raycast.maxDistance - brdf.viewDistance;
		traceReflection(raytrace, maxDistance, cache, brdf);
		traceTransmission(raytrace, maxDistance, leavingMaterial, transmittedDirection, cache, brdf);

		return IlluminatedIntersection(brdf.viewDistance, applyBRDF(brdf));
	}

	void Raytracer::traceReflection(const Raytrace & incidentRaytrace, const Float maxDistance, RaytracerCache & cache, BRDFParameters & brdf)
	{
		// would still be visible but tracing ís not wanted anymore
		if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>())
		{
			brdf.lighting.reflected = Zero<Float4>();
			return;
		}

		// does it still add to the visibility in the image
		const Float4 reflectionVisibilityIndex = incidentRaytrace.visibilityIndex * brdf.reflectanceCoefficient * max3v(brdf.surface.reflectance);
		if (x(reflectionVisibilityIndex) < cache.configuration.visibilityCutoff)
		{
			brdf.lighting.reflected = Zero<Float4>();
			return;
		}

		const Raytrace subRaytrace = Raytrace(Raycast(
			Ray(brdf.intersection.vertex, brdf.intersection.reflectedDirection),
			incidentRaytrace.raycast.cullingMask,
			Size2(reinterpret_cast<ASizeT>(brdf.intersection.object), reinterpret_cast<ASizeT>(brdf.intersection.node)),
			maxDistance),
			&brdf.intersection,
			incidentRaytrace.traceDepth + One<ASizeT>(),
			x(reflectionVisibilityIndex));

		cache.statistics.secondaryRays += One<ASizeT>();
		const IlluminatedIntersection reflectedHit = trace(subRaytrace, cache);
		brdf.lighting.reflected = reflectedHit.color;
		cache.statistics.missedSecondaryRays += static_cast<ASizeT>(outOfReach(
			subRaytrace.raycast,
			x(reflectedHit.depth)));
	}

	void Raytracer::traceTransmission(
		const Raytrace & incidentRaytrace,
		const Float maxDistance,
		const bool leavingMaterial,
		const Float4 & transmittedDirection,
		RaytracerCache & cache,
		BRDFParameters & brdf)
	{
		// would still be visible but tracing ís not wanted anymore, set transmitted to background
		if (incidentRaytrace.traceDepth >= cache.configuration.maxTraceDepth || maxDistance <= Zero<Float>())
		{
			brdf.lighting.transmitted = Zero<Float4>();
			return;
		}

		Float4 absorbtion;
		if (leavingMaterial)
		{
			// vacuum does not absorb
			absorbtion = One<Float4>();
		}
		else
		{
			// min absorbtion of material
			absorbtion = vectorization::exp(
				Float4(std::numeric_limits<Float4::ValueType>::min()) /
				min3(-brdf.surface.absorbtionCoefficient)
			);
		}
		brdf.absorbtionCoefficient = absorbtion;

		// does it still add to the visibility in the image or is it even total internal reflection (transmissionDirection = 0)
		const Float4 transmissionVisibilityIndex = Float4(incidentRaytrace.visibilityIndex) * absorbtion * (One<Float4>() - brdf.reflectanceCoefficient);
		if (x(transmissionVisibilityIndex) < cache.configuration.visibilityCutoff)
		{
			brdf.lighting.transmitted = Zero<Float4>();
			return;
		}

		const Raytrace subRaytrace = Raytrace(Raycast(
			Ray(brdf.intersection.vertex, transmittedDirection),
			cullingOrientationToMask(-cullingOrientation(incidentRaytrace.raycast)),
			Size2(reinterpret_cast<ASizeT>(brdf.intersection.object), reinterpret_cast<ASizeT>(brdf.intersection.node)),
			maxDistance),
			&brdf.intersection,
			incidentRaytrace.traceDepth + One<ASizeT>(),
			x(transmissionVisibilityIndex));

		cache.statistics.secondaryRays += One<ASizeT>();
		const IlluminatedIntersection refractedHit = trace(subRaytrace, cache);
		brdf.lighting.transmitted = refractedHit.color;
		cache.statistics.missedSecondaryRays += static_cast<ASizeT>(outOfReach(subRaytrace.raycast, x(refractedHit.depth)));

		// now that we have the transmission distance through the material, recompute the absorbtion coefficient
		if (!leavingMaterial)
		{
			brdf.absorbtionCoefficient = exp3(refractedHit.depth / -brdf.surface.absorbtionCoefficient);
		}
	}

	const Float4 Raytracer::applyBRDF(const BRDFParameters & brdf)
	{
		// https://en.wikipedia.org/wiki/Phong_reflection_model
		const Float4 reflected = brdf.lighting.reflected * brdf.surface.reflectance + brdf.lighting.specular * brdf.surface.specularity;

		// Transmittance model: Absorbtion coefficient/Beer-Lambert-law
		// http://tog.acm.org/resources/RTNews/html/rtnv10n1.html#art3
		// http://en.wikipedia.org/wiki/Absorption_coefficient
		// http://en.wikipedia.org/wiki/Beer%E2%80%93Lambert_law
		const Float4 diffuse = (brdf.lighting.ambient + brdf.lighting.diffuse) * brdf.surface.diffusion;
		const Float4 transmitted = mix(diffuse, brdf.lighting.transmitted, brdf.absorbtionCoefficient);

		return brdf.surface.emittivity + mix(transmitted, reflected, brdf.reflectanceCoefficient);
	}

}
