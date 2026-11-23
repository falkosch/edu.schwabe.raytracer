#include "raytracing/shading/SceneShader.h"
#include "../../stdafx.h"

#include "raytracing/common/StatisticsCookie.h"
#include "raytracing/shading/BoxLightFaces.h"
#include "raytracing/shading/ObjectShader.h"
#include "raytracing/shading/beer_lambert_transmittance.h"
#include "raytracing/shading/brdf/ggx.h"
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
            intersection, containment.sphereLightSamples, *containment.shadowCache, *containment.statistics
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
        const Raytrace& incidentRay, const ASizeT maxSamples, PerLightShadowCache::ShadowCacheType& shadowCache,
        StatisticsCookie& statistics
    ) const
    {
        constexpr UInt_32 SeedOffset = 0x1000u;

        // a basis with w towards the light's center
        const auto toCenter = light.position - intersection.vertex;
        const auto centerDistance = length3(toCenter);
        const auto w = toCenter / centerDistance;
        const auto up = vectorization::abs(y(w)) < 0.999f ? OneY<Float4>() : OneX<Float4>();
        const auto u = normalize3(cross3(up, w));
        const auto v = cross3(w, u);

        // without sampling, one ray to the center of the cap
        const auto sampled = maxSamples > Zero<ASizeT>();
        const auto samplesLimit = static_cast<Float>(max(maxSamples, One<ASizeT>()));
        const auto samples = static_cast<ASizeT>(
            clamp(vectorization::ceil(samplesLimit * incidentRay.visibilityIndex), One<Float>(), samplesLimit)
        );
        const auto seed = deriveSeed(incidentRay.strataSeed, SeedOffset + static_cast<UInt_32>(lightIndex));
        const auto sequenceOffset = sequenceOffset2D(seed);
        // only the cap within acos(radius / distance) of the side facing the intersection is visible from it: the
        // sine of that angle bounds the disk
        const auto visibleCap =
            vectorization::sqrt(max(Zero<Float>(), One<Float>() - sqr(light.radius / centerDistance)));

        auto fraction = Zero<spectral::SpectralVector>();
        for (auto sample = Zero<ASizeT>(); sample < samples; ++sample)
        {
            // a point of the disk (equal areas) lifted onto the visible cap
            const auto xi = sampled
                                ? sequencePoint2D(sequenceOffset, stratifiedIndex(incidentRay.stratum, samples, sample))
                                : Zero<Float4>();
            const auto r = vectorization::sqrt(x(xi)) * visibleCap;
            const auto phi = Two<Float>() * Pi<Float>() * y(xi);
            const auto lift = vectorization::sqrt(max(Zero<Float>(), One<Float>() - sqr(r)));
            const auto onDisk = u * (r * vectorization::cos(phi)) + v * (r * vectorization::sin(phi));
            const auto onSphere = light.position + (onDisk - w * lift) * light.radius;

            // ending just before the sphere, the ray can't hit the light's own emissive sphere
            const auto toSample = onSphere - intersection.vertex;
            const auto distance = length3(toSample);
            shadowRay.ray.setDirection(toSample / distance);
            shadowRay.maxDistance = distance - light.radius * 1e-3f;
            fraction += computeLitAreaFraction(
                shadowRay, intersection, lightIndex, *incidentRay.spectralContext, shadowCache, statistics
            );
        }
        return fraction * spectral::SpectralVector(One<Float>() / static_cast<Float>(samples));
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
        const SceneIntersection& intersection, const ASizeT sphereLightSamples,
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
                    sphereLightSamples, shadowCache, statistics, lighting
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
                                                 shadowRay, intersection, lightIndex, light, incidentRay,
                                                 sphereLightSamples, shadowCache, statistics
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
        const LightInfo& light, const ASizeT maxSamples, PerLightShadowCache::ShadowCacheType& shadowCache,
        StatisticsCookie& statistics, LightShading& lighting
    ) const
    {
        constexpr UInt_32 SeedOffset = 0x1000u;

        const auto& spectralContext = *incidentRay.spectralContext;
        const auto N = intersection.smoothedNormal;
        const BoxLightFaces faces(light, intersection.vertex, N);
        const auto& radiance = spectralContext.lightEmittance[lightIndex];
        // the irradiance without shadows bounds what the light adds
        const auto radianceMax = spectral::spectralMax(radiance.data);
        if (faces.faceCount == Zero<ASizeT>()
            || faces.projectedSolidAngle * radianceMax < x(adaptedVisibilityCutoff))
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

        const auto samplesLimit = static_cast<Float>(maxSamples);
        const auto samples = static_cast<ASizeT>(
            clamp(vectorization::ceil(samplesLimit * incidentRay.visibilityIndex), One<Float>(), samplesLimit)
        );
        const auto seed = deriveSeed(incidentRay.strataSeed, SeedOffset + static_cast<UInt_32>(lightIndex));
        const auto sequenceOffset = sequenceOffset2D(seed);

        // the rays weighed by the irradiance from their points: the lit share of the irradiance. The highlight is
        // their mean BRDF times that irradiance (light sampling, unbiased, exact shadows).
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
            const auto weight = dot3(N, direction) * -dot3(faces.faces[face].normal, direction) / distanceSquared;
            if (!(weight > Zero<Float>()))
            {
                continue;
            }
            const auto litWeight = traceTo(toPoint, distance) * spectral::SpectralVector(weight);
            lit += litWeight;
            weights += weight;
            if (glossy)
            {
                highlight += litWeight * brdfFrom(direction, roughness);
            }
        }
        if (!(weights > Zero<Float>()))
        {
            return;
        }

        const auto litFraction = lit * spectral::SpectralVector(One<Float>() / weights);
        if (spectral::spectralMax(litFraction) * faces.projectedSolidAngle * radianceMax < x(adaptedVisibilityCutoff))
        {
            return;
        }
        lighting.diffuse += radiance * (litFraction * spectral::SpectralVector(faces.projectedSolidAngle));
        if (glossy)
        {
            const auto perSample = spectral::SpectralVector(faces.area / static_cast<Float>(samples));
            lighting.specular += radiance * (highlight * perSample);
        }
    }

    Float4 SceneShader::adaptedVisibilityCutoff(const Float visibilityCutoff, const Float importance)
    {
        return Float4(visibilityCutoff / importance);
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
