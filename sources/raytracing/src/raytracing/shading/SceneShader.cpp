#include "raytracing/shading/SceneShader.h"
#include "../../stdafx.h"

#include "raytracing/common/StatisticsCookie.h"

// #define DISABLE_SHADOWING

namespace raytracer {
  SceneShader::SceneShader() : SceneGeometry(), backgroundShader(), ambientLight(), lights() {
  }

  SceneShader::SceneShader(const KDTreeTraverser<SceneIntersection> *const treeTraverserIn)
      : SceneGeometry(treeTraverserIn), backgroundShader(), ambientLight(), lights() {
  }

  SceneShader::~SceneShader() = default;

  const SceneShader::BackgroundShader *const SceneShader::getBackgroundShader() const {
    return backgroundShader;
  }

  void SceneShader::setBackgroundShader(const BackgroundShader *const value) {
    backgroundShader = value;
  }

  const Float4 SceneShader::getAmbientLight() const {
    return ambientLight;
  }

  void SceneShader::setAmbientLight(const Float4 &value) {
    ambientLight = value;
  }

  const SceneShader::LightsCollection &SceneShader::getLights() const {
    return lights;
  }

  SceneShader::LightsCollection &SceneShader::getLights() {
    return lights;
  }

  const Float4 SceneShader::sampleBackground(const Float4 &rayDirection) const {
    const Float4 t = backgroundShader->sample(*this, rayDirection);
    return t * wwww(t);
  }

  // Computes the lighting of a facet in the scene.
  const LightShading
  SceneShader::sample(const SceneShaderContainment &containment, const SceneIntersection &intersection) const {
    return sampleLighting(
        containment.incidentRay,
        adaptedVisibilityCutoff(containment.visibilityCutoff, containment.incidentRay.visibilityIndex),
        containment.shininess, intersection, *containment.shadowCache, *containment.statistics
    );
  }

  const Float4 SceneShader::computeLitAreaFraction(
      const RayCast &shadowRay, const SceneIntersection &intersection, const ASizeT lightIndex,
      PerLightShadowCache::ShadowCacheType &shadowCache, StatisticsCookie &statistics
  ) const {
    SceneIntersection shadowNearest = intersection;

    // Shadow caching: test last known occluding object second
    if (const auto lastShadowedByObject = shadowCache[lightIndex].lastShadowedByObject) {
      statistics.objectShadowRays += One<ASizeT>();
      if (!outOfReach(shadowRay, lastShadowedByObject->findAnyIntersection(shadowRay, &intersection, shadowNearest))) {
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
    if (!outOfReach(shadowRay, findAnyIntersection(shadowRay, &intersection, shadowNearest))) {
      // update shadow caching info
      shadowCache[lightIndex] = PerLightShadowCache(*shadowNearest.node, *shadowNearest.object);
      return Zero<Float4>();
    }
    statistics.missedShadowRays += One<ASizeT>();

    return One<Float4>();
  }

  const LightShading SceneShader::sampleLighting(
      const Raytrace &incidentRay, const Float4 &adaptedVisibilityCutoffIn, const Float4 &shininess,
      const SceneIntersection &intersection, PerLightShadowCache::ShadowCacheType &shadowCache,
      StatisticsCookie &statistics
  ) const {
#ifndef DISABLE_SHADOWING
    auto shadowRay = RayCast(
        Ray(intersection.vertex), incidentRay.rayCast.cullingMask,
        Size2(reinterpret_cast<ASizeT>(intersection.object), reinterpret_cast<ASizeT>(intersection.node)), Zero<Float>()
    );
#endif

    auto lighting = LightShading(ambientLight);
    for (auto it = lights.cbegin(); it != lights.cend(); ++it) {
      // ray with the direction from the facet to the light
      const LightInfo &light = **it;
      const Float4 lightDirection = light.position - intersection.vertex;
      const Float4 normalizedLightDirection = normalize(lightDirection);

      // facet intersection's orientation facing away from light source cannot be lit by it
      if (isNegative(dotv(normalizedLightDirection, intersection.surfaceNormal)))
        continue;

      const Float4 diffuseIntensity = lambertDiffuseIntensity(normalizedLightDirection, intersection.smoothedNormal);
      if (allTrue(diffuseIntensity < adaptedVisibilityCutoffIn))
        continue;

      const Float4 lightDistance = lengthv(lightDirection);
      const Float4 attenuatedDiffuseIntensity =
          attenuateDiffuseIntensity(light.attenuationFactors, lightDistance, diffuseIntensity);
      if (allTrue(attenuatedDiffuseIntensity < adaptedVisibilityCutoffIn))
        continue;

#ifdef DISABLE_SHADOWING
      const Float4 litAreaFraction = One<Float4>();
#else
      shadowRay.ray.setDirection(normalizedLightDirection);
      shadowRay.maxDistance = x(lightDistance);
      const Float4 litAreaFraction = computeLitAreaFraction(
          shadowRay, intersection, static_cast<ASizeT>(it - lights.cbegin()), shadowCache, statistics
      );
      if (allTrue(litAreaFraction < adaptedVisibilityCutoffIn))
        continue;
#endif

      // Diffuse and specular reflection inbound intensity of this light source
      lighting.diffuse += litAreaFraction * attenuatedDiffuseIntensity * light.emittance;
      lighting.specular += litAreaFraction
                           * phongSpecularIntensityPerReflectedIncident(
                               intersection.reflectedDirection, normalizedLightDirection, shininess
                           );
    }

    return lighting;
  }

  const Float4 SceneShader::adaptedVisibilityCutoff(const Float visibilityCutoff, const Float visibilityIndex) {
    return Float4(visibilityCutoff / visibilityIndex);
  }

  const Float4 SceneShader::lambertDiffuseIntensity(const Float4 &lightDirection, const Float4 &normal) {
    return dotv(normal, lightDirection);
  }

  const Float4 SceneShader::attenuateDiffuseIntensity(
      const Float4 &attenuationFactors, const Float4 &lightDistance, const Float4 &diffuseIntensity
  ) {
    return diffuseIntensity
           / (xxxx(attenuationFactors)
              + lightDistance * (yyyy(attenuationFactors) + lightDistance * zzzz(attenuationFactors)));
  }

  const Float4 SceneShader::phongSpecularIntensityPerReflectedIncident(
      const Float4 &reflectedIncidentDirection, const Float4 &lightDirection, const Float4 &shininess
  ) {
    return pow3(max(dotv(lightDirection, reflectedIncidentDirection), Zero<Float4>()), shininess);
  }
}
