#include "raytracing/shading/SceneShader.h"
#include "../../stdafx.h"

#include "raytracing/common/StatisticsCookie.h"
#include "raytracing/shading/BoxLightFaces.h"
#include "raytracing/shading/ObjectShader.h"
#include "raytracing/shading/SphereLightCap.h"
#include "raytracing/shading/beer_lambert_transmittance.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/power_heuristic.h"
#include "raytracing/shading/sampling.h"
#include "raytracing/shading/spectral/spectrum.h"

// #define DISABLE_SHADOWING

namespace raytracer
{
    namespace
    {
        // The shader of a transmissive occluder, or nullptr: objects without an ObjectShader block the light
        const ObjectShader* transmissiveShader(const SceneIntersection& intersection)
        {
            const auto* shader = dynamic_cast<const ObjectShader*>(intersection.object);
            return shader && anyTrue3(shader->sampleTransmittance(intersection) > Zero<Float4>()) ? shader : nullptr;
        }

        // The Fresnel transmission (Schlick) through an interface between the refraction indices z and w of eta
        spectral::SpectralVector fresnelTransmission(const Float4& eta, const Float cosine)
        {
            const auto sqrtR0 = (z(eta) - w(eta)) / (z(eta) + w(eta));
            const auto R0 = spectral::SpectralVector(sqrtR0 * sqrtR0);
            const auto cosTheta = spectral::SpectralVector(min(vectorization::abs(cosine), One<Float>()));
            return One<spectral::SpectralVector>() - brdf::schlickFresnelSpectral(R0, cosTheta);
        }

        // The density of the glossy reflection rays (Raytracer::traceGlossyReflection, GGX VNDF sampling of the
        // roughness) in the direction L: 0 where they don't contribute, below the surface, and for an intersection seen
        // from inside its object, whose rays go to the other side
        Float glossyRayPdf(
            const SceneIntersection& intersection, const Float4& V, const Float NoV, const Float4& L,
            const Float roughness
        )
        {
            if (!(x(intersection.smoothedNdotI) < Zero<Float>()) || !(NoV > Zero<Float>())
                || !(dot3(intersection.smoothedNormal, L) > Zero<Float>())
                || !(dot3(intersection.surfaceNormal, L) > Zero<Float>()))
            {
                return Zero<Float>();
            }
            const auto H = normalize3(V + L);
            return brdf::pdfGGX(NoV, max(dot3(intersection.smoothedNormal, H), Zero<Float>()), roughness);
        }

        // A sphere light's radiance per its emittance: the intensity (with the attenuation at the center's distance)
        // spread over the sphere's cross section
        Float sphereLightRadianceScale(const LightInfo& light, const Float centerDistance)
        {
            const auto& factors = light.attenuationFactors;
            const auto attenuation = x(factors) + centerDistance * (y(factors) + centerDistance * z(factors));
            return sqr(centerDistance) / (attenuation * Pi<Float>() * sqr(light.radius));
        }
    }

    SceneShader::SceneShader() : SceneGeometry(), backgroundShader(), ambientLight(), lights()
    {
    }

    SceneShader::SceneShader(std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverser)
        : SceneGeometry(std::move(treeTraverser)), backgroundShader(), ambientLight(), lights()
    {
    }

    SceneShader::~SceneShader() = default;

    const SceneShader::BackgroundShader* SceneShader::getBackgroundShader() const
    {
        return backgroundShader.get();
    }

    void SceneShader::setBackgroundShader(std::unique_ptr<const BackgroundShader> value)
    {
        backgroundShader = std::move(value);
    }

    RGBS SceneShader::getAmbientLight() const
    {
        return ambientLight;
    }

    void SceneShader::setAmbientLight(const RGBS& value)
    {
        ambientLight = value;
    }

    const SceneShader::LightsCollection& SceneShader::getLights() const
    {
        return lights;
    }

    SceneShader::LightsCollection& SceneShader::getLights()
    {
        return lights;
    }

    SpectralContext SceneShader::createSpectralContext(const Float heroLambda) const
    {
        return SpectralContext(heroLambda, ambientLight, lights);
    }

    spectral::Spectrum SceneShader::sampleBackground(
        const Float4& rayDirection, const SpectralContext& spectralContext
    ) const
    {
        return backgroundShader->sample(*this, BackgroundQuery{rayDirection, spectralContext});
    }

    // Computes the lighting of a facet in the scene.
    LightShading
    SceneShader::sample(const SceneShaderContainment& containment, const SceneIntersection& intersection) const
    {
        return sampleLighting(
            containment.incidentRay,
            adaptedVisibilityCutoff(containment.visibilityCutoff, containment.incidentRay.importance),
            containment.roughness, containment.F0,
            intersection, containment.sphereLightSamples, containment.glossyRays, *containment.shadowCache,
            *containment.statistics
        );
    }

    spectral::SpectralVector SceneShader::computeLitAreaFraction(
        const RayCast& shadowRay, const SceneIntersection& intersection, const ASizeT lightIndex,
        const SpectralContext& spectralContext, PerLightShadowCache::ShadowCacheType& shadowCache,
        StatisticsCookie& statistics
    ) const
    {
        SceneIntersection shadowNearest = intersection;

        // Shadow caching: test last known occluding object second
        if (const auto lastShadowedByObject = shadowCache[lightIndex].lastShadowedByObject)
        {
            statistics.objectShadowRays += One<ASizeT>();
            if (!outOfReach(
                shadowRay, lastShadowedByObject->findAnyIntersection(shadowRay, &intersection, shadowNearest)))
            {
                // Update shadow caching info, maybe intersection node changed
                shadowCache[lightIndex] = PerLightShadowCache(*shadowNearest.node, *lastShadowedByObject);
                return Zero<spectral::SpectralVector>();
            }
            statistics.objectMissedShadowRays += One<ASizeT>();

            // shadow test failed for cached hint for shadowing object, reset info of intersection
            shadowNearest.node = intersection.node;
            shadowNearest.object = intersection.object;
        }

        // shadow-caching fallback: test whole scene to find occlusions
        statistics.shadowRays += One<ASizeT>();
        if (!outOfReach(shadowRay, findAnyIntersection(shadowRay, &intersection, shadowNearest)))
        {
            // any hit may be transmissive, with an opaque occluder behind it
            if (transmissiveShader(shadowNearest))
            {
                return transmitShadowRay(shadowRay, intersection, lightIndex, spectralContext, shadowCache);
            }

            // update shadow caching info: only opaque occluders block the light for sure
            shadowCache[lightIndex] = PerLightShadowCache(*shadowNearest.node, *shadowNearest.object);
            return Zero<spectral::SpectralVector>();
        }
        statistics.missedShadowRays += One<ASizeT>();

        return One<spectral::SpectralVector>();
    }

    spectral::SpectralVector SceneShader::computeLitSphereFraction(
        RayCast shadowRay, const SceneIntersection& intersection, const ASizeT lightIndex, const LightInfo& light,
        const SpectralContext& spectralContext, PerLightShadowCache::ShadowCacheType& shadowCache,
        StatisticsCookie& statistics
    ) const
    {
        // ending just before the sphere, the ray can't hit the light's own emissive sphere
        const auto toCenter = SphereLightCap(light, intersection.vertex).samplePoint(Zero<Float4>());
        const auto distance = length3(toCenter);
        shadowRay.ray.setDirection(toCenter / distance);
        shadowRay.maxDistance = distance - light.radius * 1e-3f;
        return computeLitAreaFraction(shadowRay, intersection, lightIndex, spectralContext, shadowCache, statistics);
    }

    void SceneShader::sampleSphereLight(
        const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff, const Float roughness,
        const spectral::SpectralVector& F0, const SceneIntersection& intersection, const ASizeT lightIndex,
        const LightInfo& light, const ASizeT maxSamples, const ASizeT glossyRays,
        PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics, LightShading& lighting
    ) const
    {
        constexpr UInt_32 SeedOffset = 0x1000u;

        const auto& spectralContext = *incidentRay.spectralContext;
        const Float4 lightDirection = light.position - intersection.vertex;
        const Float4 lightDistance = lengthv(lightDirection);
        const Float4 normalizedLightDirection = lightDirection / lightDistance;
        if (isNegative(dotv(normalizedLightDirection, intersection.surfaceNormal)))
        {
            return;
        }
        const Float4 diffuseIntensity = lambertDiffuseIntensity(normalizedLightDirection, intersection.smoothedNormal);
        if (allTrue(diffuseIntensity < adaptedVisibilityCutoff))
        {
            return;
        }
        const Float4 attenuatedDiffuseIntensity =
            attenuateDiffuseIntensity(light.attenuationFactors, lightDistance, diffuseIntensity);
        if (allTrue(attenuatedDiffuseIntensity < adaptedVisibilityCutoff))
        {
            return;
        }

        // the highlight of a rough surface; a mirror reflects the light's emissive sphere instead
        const auto glossy = spectral::spectralMax(F0) > Zero<Float>() && roughness > brdf::MirrorRoughness;
        const auto V = -incidentRay.rayCast.ray.direction;
        const auto N = intersection.smoothedNormal;
        const auto NoV = max(dot3(N, V), Zero<Float>());

#ifndef DISABLE_SHADOWING
        auto shadowRay = RayCast(
            Ray(intersection.vertex), incidentRay.rayCast.cullingMask,
            Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
            Zero<Float>()
        );
#endif

        const auto cap = SphereLightCap(light, intersection.vertex);
        const auto samples = areaLightSamples(maxSamples, incidentRay.visibilityIndex);
        const auto seed = deriveSeed(incidentRay.strataSeed, SeedOffset + static_cast<UInt_32>(lightIndex));
        const auto sequenceOffset = sequenceOffset2D(seed);

        auto fraction = Zero<spectral::SpectralVector>();
        auto highlight = Zero<spectral::SpectralVector>();
        for (auto sample = Zero<ASizeT>(); sample < samples; ++sample)
        {
            const auto xi = sequencePoint2D(sequenceOffset, stratifiedIndex(incidentRay.stratum, samples, sample));
            const auto toSample = cap.samplePoint(xi);
            const auto distance = length3(toSample);
#ifdef DISABLE_SHADOWING
            const auto sampleFraction = One<spectral::SpectralVector>();
#else
            // ending just before the sphere, the ray can't hit the light's own emissive sphere
            shadowRay.ray.setDirection(toSample / distance);
            shadowRay.maxDistance = distance - light.radius * 1e-3f;
            const auto sampleFraction = computeLitAreaFraction(
                shadowRay, intersection, lightIndex, spectralContext, shadowCache, statistics
            );
#endif
            fraction += sampleFraction;

            // the BRDF towards the point per density of its direction, weighed against the glossy rays
            const auto L = toSample / distance;
            const auto NoL = dot3(N, L);
            if (!glossy || !(NoL > Zero<Float>()) || !(spectral::spectralMax(sampleFraction) > Zero<Float>()))
            {
                continue;
            }
            const auto lightPdf = cap.solidAnglePdf(toSample);
            if (!(lightPdf > Zero<Float>()))
            {
                continue;
            }
            const auto weight = glossyRays > Zero<ASizeT>()
                                    ? powerHeuristic(
                                        static_cast<Float>(samples), lightPdf, static_cast<Float>(glossyRays),
                                        glossyRayPdf(intersection, V, NoV, L, roughness)
                                    )
                                    : One<Float>();
            const auto H = normalize3(V + L);
            const auto NoH = max(dot3(N, H), Zero<Float>());
            const auto VoH = Float8(max(dot3v(V, H), Zero<Float4>()));
            const auto DG = brdf::evaluateGGX_DG(NoV, NoL, NoH, roughness);
            highlight += sampleFraction * brdf::schlickFresnelSpectral(F0, VoH)
                * spectral::SpectralVector(DG * NoL * weight / lightPdf);
        }
        const auto& emittance = spectralContext.lightEmittance[lightIndex];
        // the highlight stays even where the cutoff drops the diffuse light: the glossy rays left this light's share
        // to it (lightPointDensity)
        if (glossy)
        {
            const auto radianceScale = sphereLightRadianceScale(light, cap.centerDistance);
            lighting.specular +=
                emittance * (highlight * spectral::SpectralVector(radianceScale / static_cast<Float>(samples)));
        }

        const auto litAreaFraction = fraction * spectral::SpectralVector(One<Float>() / static_cast<Float>(samples));
        if (spectral::spectralMax(litAreaFraction) < x(adaptedVisibilityCutoff))
        {
            return;
        }
        const auto intensity = litAreaFraction * Float8(attenuatedDiffuseIntensity);
        lighting.diffuse += emittance * intensity;
    }

    spectral::SpectralVector SceneShader::transmitShadowRay(
        const RayCast& shadowRay, const SceneIntersection& intersection, const ASizeT lightIndex,
        const SpectralContext& spectralContext, PerLightShadowCache::ShadowCacheType& shadowCache
    ) const
    {
        constexpr ASizeT MaxObjects = 8;
        const auto& direction = shadowRay.ray.direction;
        const auto insideCulling = cullingOrientationToMask(-cullingOrientation(shadowRay));
        // behind an object, the next one is entered through a front face: culling the back faces also keeps the ray
        // from finding the exit it starts at
        const auto outsideCulling = cullingOrientationToMask(-One<Int>());

        auto fraction = One<spectral::SpectralVector>();
        auto ray = shadowRay;
        auto origin = intersection;
        for (auto object = Zero<ASizeT>(); object < MaxObjects; ++object)
        {
            auto entry = origin;
            const auto entryDistance = findNearestIntersection(ray, &origin, entry);
            if (outOfReach(ray, entryDistance))
            {
                return fraction;
            }

            const auto* shader = transmissiveShader(entry);
            if (!shader)
            {
                shadowCache[lightIndex] = PerLightShadowCache(*entry.node, *entry.object);
                return Zero<spectral::SpectralVector>();
            }
            const auto eta = shader->sampleRefractionEta(entry);
            const auto absorptionLengths = spectralContext.fromRGB(shader->sampleTransmittance(entry)).data;
            fraction *= fresnelTransmission(eta, x(entry.smoothedNdotI));

            // the exit through the same object, seen from inside
            const auto inside = RayCast(
                Ray(entry.vertex, direction), insideCulling,
                Size2(reinterpret_cast<ASizeT>(entry.object), reinterpret_cast<ASizeT>(entry.node)),
                ray.maxDistance - entryDistance
            );
            auto exit = entry;
            const auto chord = entry.object->findNearestIntersection(inside, &entry, exit);
            const auto absorbingDistance = spectral::SpectralVector(min(chord, inside.maxDistance));
            fraction *= beerLambertTransmittance(absorbingDistance, absorptionLengths);
            if (outOfReach(inside, chord))
            {
                // the light is inside the object
                return fraction;
            }
            fraction *= fresnelTransmission(eta, dot3(exit.smoothedNormal, direction));

            // continue behind the object
            ray = RayCast(
                Ray(exit.vertex, direction), outsideCulling,
                Size2(reinterpret_cast<ASizeT>(exit.object), reinterpret_cast<ASizeT>(exit.node)),
                inside.maxDistance - chord
            );
            origin = exit;
        }

        // too many objects to pass
        return Zero<spectral::SpectralVector>();
    }

    LightShading SceneShader::sampleLighting(
        const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff,
        const Float roughness, const spectral::SpectralVector& F0,
        const SceneIntersection& intersection, const ASizeT sphereLightSamples, const ASizeT glossyRays,
        PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics
    ) const
    {
#ifndef DISABLE_SHADOWING
        auto shadowRay = RayCast(
            Ray(intersection.vertex), incidentRay.rayCast.cullingMask,
            Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
            Zero<Float>()
        );
#endif

        const auto& spectralContext = *incidentRay.spectralContext;
        const auto V = -incidentRay.rayCast.ray.direction;
        const auto N = intersection.smoothedNormal;
        const auto NoV = max(dot3(N, V), Zero<Float>());
        const auto hasSpecular = spectral::spectralMax(F0) > Zero<Float>();

        auto lighting = LightShading(spectralContext.ambient);
        for (auto it = lights.cbegin(); it != lights.cend(); ++it)
        {
            const LightInfo& light = **it;
            const auto lightIndex = static_cast<ASizeT>(it - lights.cbegin());
            // the light's center and its attenuation don't describe a box light up close
            if (light.isBox())
            {
                sampleBoxLight(
                    incidentRay, adaptedVisibilityCutoff, roughness, F0, intersection, lightIndex, light,
                    sphereLightSamples, glossyRays, shadowCache, statistics, lighting
                );
                continue;
            }
            // a sphere light with shadow rays gets its highlight from them; without, from its center
            if (light.radius > Zero<Float>() && sphereLightSamples > Zero<ASizeT>())
            {
                sampleSphereLight(
                    incidentRay, adaptedVisibilityCutoff, roughness, F0, intersection, lightIndex, light,
                    sphereLightSamples, glossyRays, shadowCache, statistics, lighting
                );
                continue;
            }

            const Float4 lightDirection = light.position - intersection.vertex;
            const Float4 lightDistance = lengthv(lightDirection);
            const Float4 normalizedLightDirection = lightDirection / lightDistance;

            if (isNegative(dotv(normalizedLightDirection, intersection.surfaceNormal)))
            {
                continue;
            }

            const Float4 diffuseIntensity = lambertDiffuseIntensity(normalizedLightDirection,
                                                                    intersection.smoothedNormal);
            if (allTrue(diffuseIntensity < adaptedVisibilityCutoff))
            {
                continue;
            }

            const Float4 attenuatedDiffuseIntensity =
                attenuateDiffuseIntensity(light.attenuationFactors, lightDistance, diffuseIntensity);
            if (allTrue(attenuatedDiffuseIntensity < adaptedVisibilityCutoff))
            {
                continue;
            }

#ifdef DISABLE_SHADOWING
            const auto litAreaFraction = One<spectral::SpectralVector>();
#else
            shadowRay.ray.setDirection(normalizedLightDirection);
            shadowRay.maxDistance = x(lightDistance);
            const auto litAreaFraction = light.radius > Zero<Float>()
                                             ? computeLitSphereFraction(
                                                 shadowRay, intersection, lightIndex, light, spectralContext,
                                                 shadowCache, statistics
                                             )
                                             : computeLitAreaFraction(
                                                 shadowRay, intersection, lightIndex, spectralContext, shadowCache,
                                                 statistics
                                             );
            if (spectral::spectralMax(litAreaFraction) < x(adaptedVisibilityCutoff))
            {
                continue;
            }
#endif

            const auto intensity = litAreaFraction * Float8(attenuatedDiffuseIntensity);
            const auto lightContribution = spectralContext.lightEmittance[lightIndex] * intensity;

            lighting.diffuse += lightContribution;
            // a mirror reflects a sphere light's emissive sphere instead
            const auto isSphereLight = light.radius > Zero<Float>();
            if (hasSpecular && (!isSphereLight || roughness > brdf::MirrorRoughness))
            {
                const auto H = normalize3(V + normalizedLightDirection);
                const auto NoL = max(x(diffuseIntensity), Zero<Float>());
                const auto NoH = max(dot3(N, H), Zero<Float>());
                const auto VoH = Float8(max(dot3v(V, H), Zero<Float4>()));
                // a sphere light widens the lobe by its size, the normalization keeps the energy (Karis 2013)
                const auto alpha = isSphereLight
                                       ? min(One<Float>(), roughness + light.radius / (Two<Float>() * x(lightDistance)))
                                       : roughness;
                const auto normalization = isSphereLight ? sqr(roughness / alpha) : One<Float>();
                const auto DG = brdf::evaluateGGX_DG(NoV, NoL, NoH, alpha) * normalization;
                const auto F = brdf::schlickFresnelSpectral(F0, VoH);
                lighting.specular += lightContribution * (F * DG);
            }
        }

        return lighting;
    }

    void SceneShader::sampleBoxLight(
        const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff, const Float roughness,
        const spectral::SpectralVector& F0, const SceneIntersection& intersection, const ASizeT lightIndex,
        const LightInfo& light, const ASizeT maxSamples, const ASizeT glossyRays,
        PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics, LightShading& lighting
    ) const
    {
        constexpr UInt_32 SeedOffset = 0x1000u;

        const auto& spectralContext = *incidentRay.spectralContext;
        const auto N = intersection.smoothedNormal;
        const BoxLightFaces faces(light, intersection.vertex, N);
        const auto& radiance = spectralContext.lightEmittance[lightIndex];
        // the diffuse reflection without shadows (Lambert: the irradiance / pi) bounds what the light adds
        const auto radianceMax = spectral::spectralMax(radiance.data);
        if (faces.faceCount == Zero<ASizeT>()
            || faces.projectedSolidAngle * ReciprocalPi<Float>() * radianceMax < x(adaptedVisibilityCutoff))
        {
            return;
        }

        // the highlight of light from the direction L: the GGX BRDF of the lobe width alpha with its Fresnel; a mirror
        // reflects the light's emissive box instead
        const auto glossy = spectral::spectralMax(F0) > Zero<Float>() && roughness > brdf::MirrorRoughness;
        const auto V = -incidentRay.rayCast.ray.direction;
        const auto NoV = max(dot3(N, V), Zero<Float>());
        const auto brdfFrom = [&](const Float4& L, const Float alpha)
        {
            const auto H = normalize3(V + L);
            const auto NoH = max(dot3(N, H), Zero<Float>());
            const auto VoH = Float8(max(dot3v(V, H), Zero<Float4>()));
            const auto DG = brdf::evaluateGGX_DG(NoV, max(dot3(N, L), Zero<Float>()), NoH, alpha);
            return brdf::schlickFresnelSpectral(F0, VoH) * spectral::SpectralVector(DG);
        };

#ifndef DISABLE_SHADOWING
        auto shadowRay = RayCast(
            Ray(intersection.vertex), incidentRay.rayCast.cullingMask,
            Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
            Zero<Float>()
        );
#endif
        // ending just before the face, the ray can't hit the light's own emissive box
        const auto traceTo = [&](const Float4& toPoint, const Float distance)
        {
#ifdef DISABLE_SHADOWING
            return One<spectral::SpectralVector>();
#else
            shadowRay.ray.setDirection(toPoint / distance);
            shadowRay.maxDistance = distance - distance * 1e-3f;
            return computeLitAreaFraction(
                shadowRay, intersection, lightIndex, spectralContext, shadowCache, statistics
            );
#endif
        };

        if (maxSamples == Zero<ASizeT>())
        {
            ASizeT nearestFace;
            const auto toNearest = faces.nearestPoint(nearestFace);
            const auto litFraction = traceTo(toNearest, length3(toNearest));
            lighting.diffuse += radiance * (litFraction * spectral::SpectralVector(faces.projectedSolidAngle));
            if (!glossy)
            {
                return;
            }

            // without samples, each face's highlight from its representative point, with the face as a sphere of the
            // same solid angle (seen from its polygon's mean corner) widening the lobe as a sphere light does and the
            // normalization keeping the energy (Karis 2013)
            for (auto face = Zero<ASizeT>(); face < faces.faceCount; ++face)
            {
                const auto& f = faces.faces[face];
                const auto L = normalize3(faces.representativePoint(face, intersection.reflectedDirection));
                auto toCenter = Zero<Float4>();
                for (auto i = Zero<ASizeT>(); i < f.vertexCount; ++i)
                {
                    toCenter += f.vertices[i];
                }
                toCenter = toCenter / static_cast<Float>(f.vertexCount);
                const auto centerDistanceSquared = dot3(toCenter, toCenter);
                const auto solidAngle = f.area * vectorization::abs(dot3(f.normal, toCenter))
                    / (centerDistanceSquared * vectorization::sqrt(centerDistanceSquared));
                const auto alpha = min(
                    One<Float>(), roughness + Half<Float>() * vectorization::sqrt(solidAngle / Pi<Float>())
                );
                const auto normalization = spectral::SpectralVector(sqr(roughness / alpha) * f.projectedSolidAngle);
                lighting.specular += radiance * (litFraction * normalization * brdfFrom(L, alpha));
            }
            return;
        }

        const auto samples = areaLightSamples(maxSamples, incidentRay.visibilityIndex);
        const auto seed = deriveSeed(incidentRay.strataSeed, SeedOffset + static_cast<UInt_32>(lightIndex));
        const auto sequenceOffset = sequenceOffset2D(seed);

        // the rays weighed by the irradiance from their points: the lit share of the irradiance. The highlight is
        // their mean BRDF times that irradiance (light sampling, unbiased, exact shadows), each weighed against the
        // glossy rays by the density of its direction: the distance squared per cosine at the face and visible area.
        auto lit = Zero<spectral::SpectralVector>();
        auto weights = Zero<Float>();
        auto highlight = Zero<spectral::SpectralVector>();
        for (auto sample = Zero<ASizeT>(); sample < samples; ++sample)
        {
            const auto xi = sequencePoint2D(sequenceOffset, stratifiedIndex(incidentRay.stratum, samples, sample));
            ASizeT face;
            const auto toPoint = faces.samplePoint(xi, face);
            const auto distanceSquared = dot3(toPoint, toPoint);
            const auto distance = vectorization::sqrt(distanceSquared);
            const auto direction = toPoint / distance;
            const auto faceCosine = -dot3(faces.faces[face].normal, direction);
            const auto weight = dot3(N, direction) * faceCosine / distanceSquared;
            if (!(weight > Zero<Float>()))
            {
                continue;
            }
            const auto litWeight = traceTo(toPoint, distance) * spectral::SpectralVector(weight);
            lit += litWeight;
            weights += weight;
            if (glossy && glossyRays > Zero<ASizeT>())
            {
                const auto lightPdf = distanceSquared / (faceCosine * faces.area);
                const auto misWeight = powerHeuristic(
                    static_cast<Float>(samples), lightPdf, static_cast<Float>(glossyRays),
                    glossyRayPdf(intersection, V, NoV, direction, roughness)
                );
                highlight += litWeight * brdfFrom(direction, roughness) * spectral::SpectralVector(misWeight);
            }
            else if (glossy)
            {
                highlight += litWeight * brdfFrom(direction, roughness);
            }
        }
        if (!(weights > Zero<Float>()))
        {
            return;
        }

        // the highlight stays even where the cutoff drops the diffuse light: the glossy rays left this light's share
        // to it (lightPointDensity)
        if (glossy)
        {
            const auto perSample = spectral::SpectralVector(faces.area / static_cast<Float>(samples));
            lighting.specular += radiance * (highlight * perSample);
        }
        const auto litFraction = lit * spectral::SpectralVector(One<Float>() / weights);
        if (spectral::spectralMax(litFraction) * faces.projectedSolidAngle * ReciprocalPi<Float>() * radianceMax
            < x(adaptedVisibilityCutoff))
        {
            return;
        }
        lighting.diffuse += radiance * (litFraction * spectral::SpectralVector(faces.projectedSolidAngle));
    }

    Float4 SceneShader::adaptedVisibilityCutoff(const Float visibilityCutoff, const Float importance)
    {
        return Float4(visibilityCutoff / importance);
    }

    ASizeT SceneShader::areaLightSamples(const ASizeT maxSamples, const Float visibilityIndex)
    {
        const auto samplesLimit = static_cast<Float>(maxSamples);
        return static_cast<ASizeT>(
            clamp(vectorization::ceil(samplesLimit * visibilityIndex), One<Float>(), samplesLimit)
        );
    }

    SceneShader::LightPointDensity SceneShader::lightPointDensity(
        const LightInfo& light, const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff,
        const SceneIntersection& intersection, const Float4& toPoint
    ) const
    {
        const auto noLight = LightPointDensity{false, Zero<Float>(), spectral::Spectrum::zero()};
        auto lightIndex = Zero<ASizeT>();
        while (lightIndex < lights.size() && lights[lightIndex].get() != &light)
        {
            ++lightIndex;
        }
        // a point light has no emitter that lights
        if (lightIndex == lights.size() || !(light.isBox() || light.radius > Zero<Float>()))
        {
            return noLight;
        }
        const auto& emittance = incidentRay.spectralContext->lightEmittance[lightIndex];
        // seen from inside its object, the intersection gets no highlight (its normal faces away from the viewer)
        const auto sampled = x(intersection.smoothedNdotI) < Zero<Float>();

        if (!light.isBox())
        {
            // on the sphere, which an emitter moved away from its light isn't
            const auto cap = SphereLightCap(light, intersection.vertex);
            const auto fromCenter = length3(intersection.vertex + toPoint - light.position);
            if (vectorization::abs(fromCenter - light.radius) > 1e-3f * light.radius + 1e-4f)
            {
                return noLight;
            }
            const auto radiance = emittance * sphereLightRadianceScale(light, cap.centerDistance);

            // sampleSphereLight's tests whether to sample the light
            const Float4 lightDirection = light.position - intersection.vertex;
            const Float4 lightDistance = lengthv(lightDirection);
            const Float4 normalizedLightDirection = lightDirection / lightDistance;
            const Float4 diffuseIntensity =
                lambertDiffuseIntensity(normalizedLightDirection, intersection.smoothedNormal);
            if (!sampled || isNegative(dotv(normalizedLightDirection, intersection.surfaceNormal))
                || allTrue(diffuseIntensity < adaptedVisibilityCutoff)
                || allTrue(
                    attenuateDiffuseIntensity(light.attenuationFactors, lightDistance, diffuseIntensity)
                    < adaptedVisibilityCutoff
                ))
            {
                return {true, Zero<Float>(), radiance};
            }
            return {true, cap.solidAnglePdf(toPoint), radiance};
        }

        // the face of the point among those that emit towards the intersection: on its plane, inside its rectangle
        const BoxLightFaces faces(light, intersection.vertex, intersection.smoothedNormal);
        const auto distanceSquared = dot3(toPoint, toPoint);
        const auto distance = vectorization::sqrt(distanceSquared);
        const auto tolerance = 1e-3f * max(distance, One<Float>());
        for (auto face = Zero<ASizeT>(); face < faces.faceCount; ++face)
        {
            const auto& f = faces.faces[face];
            const auto offset = toPoint - f.center;
            const auto uLength = length3(f.tangentU);
            const auto vLength = length3(f.tangentV);
            if (vectorization::abs(dot3(offset, f.normal)) > tolerance
                || vectorization::abs(dot3(offset, f.tangentU)) > uLength * (uLength + tolerance)
                || vectorization::abs(dot3(offset, f.tangentV)) > vLength * (vLength + tolerance))
            {
                continue;
            }
            // sampleBoxLight's test whether to sample the light
            const auto cosine = -dot3(f.normal, toPoint) / distance;
            if (!sampled || !(cosine > Zero<Float>()) || !(faces.area > Zero<Float>())
                || faces.projectedSolidAngle * ReciprocalPi<Float>() * spectral::spectralMax(emittance.data)
                       < x(adaptedVisibilityCutoff))
            {
                return {true, Zero<Float>(), emittance};
            }
            return {true, distanceSquared / (cosine * faces.area), emittance};
        }
        return noLight;
    }

    Float4 SceneShader::lambertDiffuseIntensity(const Float4& lightDirection, const Float4& normal)
    {
        return dotv(normal, lightDirection);
    }

    Float4 SceneShader::attenuateDiffuseIntensity(
        const Float4& attenuationFactors, const Float4& lightDistance, const Float4& diffuseIntensity
    )
    {
        return diffuseIntensity / multiplyAdd(
            lightDistance,
            multiplyAdd(lightDistance, zzzz(attenuationFactors), yyyy(attenuationFactors)),
            xxxx(attenuationFactors));
    }
}
