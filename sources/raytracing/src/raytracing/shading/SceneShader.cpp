#include "raytracing/shading/SceneShader.h"
#include "../../stdafx.h"

#include "raytracing/common/StatisticsCookie.h"
#include "raytracing/shading/EmitterSPD.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/spectral/spectrum.h"

// #define DISABLE_SHADOWING

namespace raytracer
{
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

    spectral::Spectrum SceneShader::sampleBackground(const Float4& rayDirection, const Float heroLambda) const
    {
        return backgroundShader->sample(*this, BackgroundQuery{rayDirection, heroLambda});
    }

    // Computes the lighting of a facet in the scene.
    LightShading
    SceneShader::sample(const SceneShaderContainment& containment, const SceneIntersection& intersection) const
    {
        return sampleLighting(
            containment.incidentRay,
            adaptedVisibilityCutoff(containment.visibilityCutoff, containment.incidentRay.visibilityIndex),
            containment.roughness, containment.F0,
            intersection, *containment.shadowCache, *containment.statistics
        );
    }

    Float4 SceneShader::computeLitAreaFraction(
        const RayCast& shadowRay, const SceneIntersection& intersection, const ASizeT lightIndex,
        PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics
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
                return Zero<Float4>();
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
            // update shadow caching info
            shadowCache[lightIndex] = PerLightShadowCache(*shadowNearest.node, *shadowNearest.object);
            return Zero<Float4>();
        }
        statistics.missedShadowRays += One<ASizeT>();

        return One<Float4>();
    }

    LightShading SceneShader::sampleLighting(
        const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff,
        const Float roughness, const spectral::SpectralVector& F0,
        const SceneIntersection& intersection, PerLightShadowCache::ShadowCacheType& shadowCache,
        StatisticsCookie& statistics
    ) const
    {
#ifndef DISABLE_SHADOWING
        auto shadowRay = RayCast(
            Ray(intersection.vertex), incidentRay.rayCast.cullingMask,
            Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)),
            Zero<Float>()
        );
#endif

        const auto V = -incidentRay.rayCast.ray.direction;
        const auto N = intersection.smoothedNormal;

        const auto wavelengths = spectral::wavelengthsAt(incidentRay.heroLambda);

        auto lighting = LightShading(spectral::fromRGB(wavelengths, ambientLight.value));
        for (auto it = lights.cbegin(); it != lights.cend(); ++it)
        {
            const LightInfo& light = **it;
            const Float4 lightDirection = light.position - intersection.vertex;
            const Float4 normalizedLightDirection = normalize(lightDirection);

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

            const Float4 lightDistance = lengthv(lightDirection);
            const Float4 attenuatedDiffuseIntensity =
                attenuateDiffuseIntensity(light.attenuationFactors, lightDistance, diffuseIntensity);
            if (allTrue(attenuatedDiffuseIntensity < adaptedVisibilityCutoff))
            {
                continue;
            }

#ifdef DISABLE_SHADOWING
            const Float4 litAreaFraction = One<Float4>();
#else
            shadowRay.ray.setDirection(normalizedLightDirection);
            shadowRay.maxDistance = x(lightDistance);
            const Float4 litAreaFraction = computeLitAreaFraction(
                shadowRay, intersection, static_cast<ASizeT>(it - lights.cbegin()), shadowCache, statistics
            );
            if (allTrue(litAreaFraction < adaptedVisibilityCutoff))
            {
                continue;
            }
#endif

            const auto intensity = Float8(litAreaFraction * attenuatedDiffuseIntensity);
            const auto spectralEmittance = sampleEmitter(light, wavelengths);
            const auto lightContribution = spectralEmittance * intensity;

            lighting.diffuse += lightContribution;
            if (spectral::spectralMax(F0) > Zero<Float>())
            {
                const auto H = normalize3(V + normalizedLightDirection);
                const auto VoH = Float8(max(dot3v(V, H), Zero<Float4>()));
                const auto DG = brdf::evaluateGGX_DG(N, V, normalizedLightDirection, H, roughness);
                const auto F = brdf::schlickFresnelSpectral(F0, VoH);
                lighting.specular += lightContribution * (F * DG);
            }
        }

        return lighting;
    }

    Float4 SceneShader::adaptedVisibilityCutoff(const Float visibilityCutoff, const Float visibilityIndex)
    {
        return Float4(visibilityCutoff / visibilityIndex);
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

    Float4 SceneShader::phongSpecularIntensityPerReflectedIncident(
        const Float4& reflectedIncidentDirection, const Float4& lightDirection, const Float4& shininess
    )
    {
        return pow3(max(dotv(lightDirection, reflectedIncidentDirection), Zero<Float4>()), shininess);
    }
}
