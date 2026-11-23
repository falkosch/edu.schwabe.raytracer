#include "raytracing_tests.h"

#include <raytracing/common/RefractionIndices.h>
#include <raytracing/common/StatisticsCookie.h>
#include <raytracing/geometry/forms/Box.h>
#include <raytracing/geometry/forms/Sphere.h>
#include <raytracing/scene/Resources.h>
#include <raytracing/scene/Scene.h>
#include <raytracing/shading/BoxLightFaces.h>
#include <raytracing/shading/Raytrace.h>
#include <raytracing/shading/SceneShaderContainment.h>
#include <raytracing/shading/brdf/ggx.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <algorithm>
#include <cmath>

namespace raytracer::test {
  TEST_CLASS(SceneShaderTest) {
    // A point on the floor (y = -1) lit by a white light above it (y = 2, a point light or a sphere light of the
    // radius), with a sphere in between at the origin. Returns the diffuse lighting of the point, averaged over the
    // wavelengths.
    static Float diffuseBelowSphere(
        const Float4 &transmittance, const Float refractionIndex, const Float sphereRadius = 0.5f,
        const Float lightRadius = 0.0f, const ASizeT sphereLightSamples = 16
    ) {
      Scene scene{};
      auto sphere = std::make_unique<SceneObject>("sphere");
      sphere->setForm(std::make_unique<Sphere>());
      sphere->scale(Float3(sphereRadius, sphereRadius, sphereRadius));
      const auto none = Zero<Float4>();
      sphere->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(none));
      sphere->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
      sphere->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(none));
      sphere->setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(none));
      sphere->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
      sphere->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(transmittance));
      sphere->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
          RefractionIndices::refractionEta(RefractionIndices::Vacuum, refractionIndex)
      ));
      scene.getSceneObjects().push_back(std::move(sphere));
      scene.buildSceneGraph();

      auto light = std::make_unique<LightInfo>();
      light->position = Float4(0.0f, 2.0f, 0.0f, 1.0f);
      light->emittance = RGBS(1.0f, 1.0f, 1.0f);
      light->attenuationFactors = Float4(1.0f, 0.0f, 0.0f, 0.0f);
      light->radius = lightRadius;
      scene.getLights().push_back(std::move(light));

      const auto context = scene.createSpectralContext(spectral::LAMBDA_MIN + 0.5f * spectral::LAMBDA_RANGE);
      SceneIntersection floor{};
      floor.vertex = Float4(0.0f, -1.0f, 0.0f, 1.0f);
      floor.surfaceNormal = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      floor.smoothedNormal = floor.surfaceNormal;
      const Raytrace incident{
          RayCast{Ray{Float4(0.0f, 0.0f, -2.0f, 1.0f), normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f))},
                  Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f},
          nullptr, 0, 1.0f, &context
      };
      PerLightShadowCache::ShadowCacheType shadowCache(1);
      StatisticsCookie statistics{};

      const auto lighting = scene.sampleLighting(
          incident, Float4(1e-6f), 0.5f, Zero<spectral::SpectralVector>(), floor, sphereLightSamples, shadowCache,
          statistics
      );
      return horizontalSum(lighting.diffuse.data) / 8.0f;
    }

    // A white light 3 above a floor point, dimmed to 1/16 by its attenuation, as SceneShader::sample lights the point
    // for a ray of the visibility index and the importance, with the cutoff 1/512. Returns the diffuse lighting.
    static Float dimLightFor(const Float visibilityIndex, const Float importance) {
      Scene scene{};
      auto aside = std::make_unique<SceneObject>("sphere aside");
      aside->setForm(std::make_unique<Sphere>());
      aside->translate(Float3(5.0f, 0.0f, 0.0f));
      scene.getSceneObjects().push_back(std::move(aside));
      scene.buildSceneGraph();

      auto light = std::make_unique<LightInfo>();
      light->position = Float4(0.0f, 2.0f, 0.0f, 1.0f);
      light->emittance = RGBS(1.0f, 1.0f, 1.0f);
      light->attenuationFactors = Float4(16.0f, 0.0f, 0.0f, 0.0f);
      scene.getLights().push_back(std::move(light));

      const auto context = scene.createSpectralContext(spectral::LAMBDA_MIN + 0.5f * spectral::LAMBDA_RANGE);
      SceneIntersection floor{};
      floor.vertex = Float4(0.0f, -1.0f, 0.0f, 1.0f);
      floor.surfaceNormal = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      floor.smoothedNormal = floor.surfaceNormal;
      PerLightShadowCache::ShadowCacheType shadowCache(1);
      StatisticsCookie statistics{};
      SceneShaderContainment containment{shadowCache, statistics};
      containment.incidentRay = Raytrace{
          RayCast{Ray{Float4(0.0f, 0.0f, -2.0f, 1.0f), normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f))},
                  Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f},
          nullptr, 1, visibilityIndex, &context
      };
      containment.incidentRay.importance = importance;
      containment.F0 = Zero<spectral::SpectralVector>();
      containment.roughness = 0.5f;
      containment.visibilityCutoff = 1.0f / 512.0f;

      const auto lighting = scene.sample(containment, floor);
      return horizontalSum(lighting.diffuse.data) / 8.0f;
    }

    // An object of the form with the transmittance and refraction index, nothing else
    static std::unique_ptr<SceneObject> occluder(
        std::unique_ptr<const Form> form, const Float3 &scale, const Float3 &translation,
        const Float4 &transmittance = Zero<Float4>(), const Float refractionIndex = RefractionIndices::Glass
    ) {
      auto object = std::make_unique<SceneObject>("occluder");
      object->setForm(std::move(form));
      object->scale(scale);
      object->translate(translation);
      const auto none = Zero<Float4>();
      object->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(transmittance));
      object->setRefractionEtaShader(
          std::make_unique<Resources::ConstMaterialShader>(
              RefractionIndices::refractionEta(RefractionIndices::Vacuum, refractionIndex)
          )
      );
      return object;
    }

    // An opaque sphere of the radius at the origin
    static std::unique_ptr<SceneObject> sphereOccluder(const Float radius) {
      return occluder(std::make_unique<Sphere>(), Float3(radius, radius, radius), Float3(0.0f, 0.0f, 0.0f));
    }

    // A sphere far off to the side, which hides nothing
    static std::unique_ptr<SceneObject> nothing() {
      return occluder(std::make_unique<Sphere>(), Float3(1.0f, 1.0f, 1.0f), Float3(50.0f, 0.0f, 0.0f));
    }

    // A white square light of radiance 1 and the half size at the height, emitting downwards
    static std::unique_ptr<LightInfo> squareLight(const Float halfSize, const Float height) {
      auto light = std::make_unique<LightInfo>();
      light->position = Float4(0.0f, height, 0.0f, 1.0f);
      light->emittance = RGBS(1.0f, 1.0f, 1.0f);
      light->halfExtents = Float4(halfSize, 0.0f, halfSize, 0.0f);
      light->emittingFaces = LightInfo::faceBit(1, false);
      return light;
    }

    struct FloorLighting {
      Float diffuse;
      Float specular;
    };

    // The lighting of a point on the floor (y = -1, seen from (0, 0, -2)) by the light, with the object in the scene
    // and up to the samples shadow rays: diffuse and specular, averaged over the wavelengths and divided by the
    // average of upsampled white
    static FloorLighting floorLighting(
        std::unique_ptr<SceneObject> object, std::unique_ptr<LightInfo> light, const ASizeT samples = 16,
        const Float roughness = 0.5f, const Float F0 = 0.0f
    ) {
      Scene scene{};
      scene.getSceneObjects().push_back(std::move(object));
      scene.buildSceneGraph();
      scene.getLights().push_back(std::move(light));

      const auto context = scene.createSpectralContext(spectral::LAMBDA_MIN + 0.5f * spectral::LAMBDA_RANGE);
      SceneIntersection floor{};
      floor.vertex = Float4(0.0f, -1.0f, 0.0f, 1.0f);
      floor.surfaceNormal = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      floor.smoothedNormal = floor.surfaceNormal;
      const auto direction = normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f));
      floor.reflectedDirection = Float4(0.0f, -y(direction), z(direction), 0.0f);
      const Raytrace incident{
          RayCast{Ray{Float4(0.0f, 0.0f, -2.0f, 1.0f), direction}, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f},
          nullptr, 0, 1.0f, &context
      };
      PerLightShadowCache::ShadowCacheType shadowCache(1);
      StatisticsCookie statistics{};

      const auto lighting = scene.sampleLighting(
          incident, Float4(1e-6f), roughness, spectral::SpectralVector(F0), floor, samples, shadowCache, statistics
      );
      const auto white = horizontalSum(context.fromRGB(Float4(1.0f, 1.0f, 1.0f, 0.0f)).data);
      return {horizontalSum(lighting.diffuse.data) / white, horizontalSum(lighting.specular.data) / white};
    }

    // The irradiance per radiance of the square light at the floor point, without shadows
    static Float squareLightIrradiance(const Float halfSize, const Float height) {
      return BoxLightFaces(*squareLight(halfSize, height), Float4(0.0f, -1.0f, 0.0f, 1.0f), Float4(0, 1, 0, 0))
          .projectedSolidAngle;
    }

  public:
    TEST_METHOD(lightCutoffReadsTheImportanceNotTheVisibilityIndex) {
      // One of 64 glossy rays has the visibility index 1/64 but estimates the whole reflection (importance 1): the
      // light of 1/16 stays above its cutoff. A ray of the importance 1/64 has the cutoff 64/512 and skips it.
      const auto glossyRay = dimLightFor(1.0f / 64.0f, 1.0f);
      const auto dimRay = dimLightFor(1.0f / 64.0f, 1.0f / 64.0f);
      Assert::IsTrue(glossyRay > 0.01f, L"one of 64 glossy rays gets the dim light", LINE_INFO());
      Assert::AreEqual(0.0f, dimRay, 1e-6f, L"a ray that dim skips it", LINE_INFO());
    }

    TEST_METHOD(opaqueSphereCastsFullShadow) {
      Assert::AreEqual(0.0f, diffuseBelowSphere(Zero<Float4>(), RefractionIndices::Glass), 1e-6f, L"no light",
                       LINE_INFO());
    }

    TEST_METHOD(clearGlassSphereLetsTheFresnelTransmissionThrough) {
      // the shadow ray passes the sphere's center at normal incidence: (1 - R0)^2 with R0 = (0.5 / 2.5)^2 for glass
      const auto clear = diffuseBelowSphere(Float4(1e6f, 1e6f, 1e6f, 0.0f), 1.5f);
      const auto lit = diffuseBelowSphere(Float4(1e6f, 1e6f, 1e6f, 0.0f), RefractionIndices::Vacuum);
      Assert::IsTrue(lit > 0.0f, L"a vacuum sphere lets all light through", LINE_INFO());
      Assert::AreEqual(0.96f * 0.96f, clear / lit, 1e-3f, L"Fresnel transmission of entry and exit", LINE_INFO());
    }

    TEST_METHOD(sphereLightCastsSoftShadows) {
      // seen from the floor point, the occluder of radius 0.25 covers 14.5 degrees around the light's center, the
      // light of radius 1 reaches out to 19.5 degrees: a penumbra
      const auto clear = Float4(1e6f, 1e6f, 1e6f, 0.0f);
      const auto lit = diffuseBelowSphere(clear, RefractionIndices::Vacuum, 0.25f, 1.0f);
      const auto penumbra = diffuseBelowSphere(Zero<Float4>(), RefractionIndices::Glass, 0.25f, 1.0f);
      const auto umbra = diffuseBelowSphere(Zero<Float4>(), RefractionIndices::Glass, 0.25f, 0.0f);
      Assert::AreEqual(0.0f, umbra, 1e-6f, L"a point light leaves a hard shadow", LINE_INFO());
      Assert::IsTrue(penumbra > 0.1f * lit && penumbra < 0.9f * lit, L"the sphere light is partly hidden",
                     LINE_INFO());
    }

    TEST_METHOD(sphereLightWithoutSamplesCastsHardShadows) {
      // one shadow ray to the light's nearest point, which the occluder hides
      const auto lit = diffuseBelowSphere(Float4(1e6f, 1e6f, 1e6f, 0.0f), RefractionIndices::Vacuum, 0.25f, 1.0f, 0);
      const auto shadowed = diffuseBelowSphere(Zero<Float4>(), RefractionIndices::Glass, 0.25f, 1.0f, 0);
      Assert::IsTrue(lit > 0.0f, L"lit without an occluder", LINE_INFO());
      Assert::AreEqual(0.0f, shadowed, 1e-6f, L"a hard shadow", LINE_INFO());
    }

    TEST_METHOD(absorbingGlassSphereDimsAlongTheChord) {
      // absorption length 1 along the chord of length 1 (the sphere's diameter): exp(-1), with white upsampled to about
      // 1 per wavelength
      const auto lit = diffuseBelowSphere(Float4(1e6f, 1e6f, 1e6f, 0.0f), RefractionIndices::Vacuum);
      const auto absorbed = diffuseBelowSphere(Float4(1.0f, 1.0f, 1.0f, 0.0f), RefractionIndices::Vacuum);
      Assert::AreEqual(0.3679f, absorbed / lit, 0.04f, L"Beer-Lambert over the chord", LINE_INFO());
    }
    TEST_METHOD(boxLightIrradianceIsItsFormFactor) {
      // unshadowed, every shadow ray reaches the light: the exact irradiance (BoxLightFaces_test checks it)
      const auto lit = floorLighting(nothing(), squareLight(0.5f, 2.0f));
      Assert::AreEqual(squareLightIrradiance(0.5f, 2.0f), lit.diffuse, 1e-3f * lit.diffuse, L"irradiance", LINE_INFO());
    }

    TEST_METHOD(distantBoxLightActsAsAPointLight) {
      // an area of 0.01 at a distance of 20: a point light of the intensity radiance * area
      const auto box = floorLighting(nothing(), squareLight(0.05f, 19.0f));
      auto point = std::make_unique<LightInfo>();
      point->position = Float4(0.0f, 19.0f, 0.0f, 1.0f);
      point->emittance = RGBS(0.01f, 0.01f, 0.01f);
      point->attenuationFactors = Float4(0.0f, 0.0f, 1.0f, 0.0f);
      const auto pointLit = floorLighting(nothing(), std::move(point));
      Assert::AreEqual(pointLit.diffuse, box.diffuse, 1e-3f * pointLit.diffuse, L"far field", LINE_INFO());
    }

    TEST_METHOD(boxLightCastsSoftShadows) {
      // seen from the floor point, the light of half size 1 reaches out to 18.4 degrees at its edges' centers and
      // 25.2 degrees at its corners. An occluder of radius 0.25 covers 14.5 degrees around the vertical, one of radius
      // 0.5 covers 30 degrees.
      const auto lit = floorLighting(nothing(), squareLight(1.0f, 2.0f)).diffuse;
      const auto penumbra = floorLighting(sphereOccluder(0.25f), squareLight(1.0f, 2.0f)).diffuse;
      const auto umbra = floorLighting(sphereOccluder(0.5f), squareLight(1.0f, 2.0f)).diffuse;
      Assert::IsTrue(penumbra > 0.1f * lit && penumbra < 0.9f * lit, L"the light is partly hidden", LINE_INFO());
      Assert::AreEqual(0.0f, umbra, 1e-6f, L"the light is hidden", LINE_INFO());
    }

    TEST_METHOD(halfHiddenBoxLightIsHalfLit) {
      // a board over x < 0 (or x > 0) halfway up hides the half of the light that gives half the irradiance
      const auto board = [](const Float side) {
        return occluder(std::make_unique<Box>(), Float3(5.0f, 0.01f, 5.0f), Float3(5.0f * side, 0.5f, 0.0f));
      };
      for (const ASizeT samples : {ASizeT{64}, ASizeT{1024}}) {
        const auto left = floorLighting(board(-1.0f), squareLight(1.0f, 2.0f), samples).diffuse;
        const auto right = floorLighting(board(1.0f), squareLight(1.0f, 2.0f), samples).diffuse;
        const auto lit = floorLighting(nothing(), squareLight(1.0f, 2.0f), samples).diffuse;
        Assert::AreEqual(lit, left + right, 1e-4f * lit, L"the same rays: the halves add up", LINE_INFO());
        Assert::AreEqual(0.5f, left / lit, samples < 1024 ? 0.05f : 0.01f, L"half of the irradiance", LINE_INFO());
      }
    }

    TEST_METHOD(boxLightWithoutSamplesCastsHardShadows) {
      // one shadow ray to the light's nearest point, which the occluder hides; without it the exact irradiance
      const auto lit = floorLighting(nothing(), squareLight(1.0f, 2.0f), 0).diffuse;
      const auto shadowed = floorLighting(sphereOccluder(0.25f), squareLight(1.0f, 2.0f), 0).diffuse;
      Assert::AreEqual(squareLightIrradiance(1.0f, 2.0f), lit, 1e-3f * lit, L"irradiance", LINE_INFO());
      Assert::AreEqual(0.0f, shadowed, 1e-6f, L"a hard shadow", LINE_INFO());
    }

    TEST_METHOD(roughSurfacesGetTheBoxLightsHighlight) {
      const auto rough = floorLighting(nothing(), squareLight(1.0f, 2.0f), 16, 0.3f, 0.04f);
      const auto mirror = floorLighting(nothing(), squareLight(1.0f, 2.0f), 16, 0.01f, 0.04f);
      Assert::IsTrue(rough.specular > 0.0f, L"a rough surface gets a highlight", LINE_INFO());
      Assert::AreEqual(0.0f, mirror.specular, 1e-9f, L"a mirror reflects the emitter instead", LINE_INFO());
    }

    TEST_METHOD(boxLightHighlightIsTheIntegratedBRDF) {
      // the floor point's reflection (0, 1, 2) / sqrt(5) meets the plane y = 2 at z = 6: a light of 2 x 2 there, and
      // one off to the side of the reflection. The reference integrates the GGX BRDF (F = 1) over the light's area.
      const auto direction = normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f));
      const auto V = -direction;
      for (const auto lightZ : {6.0f, 0.0f}) {
        for (const auto roughness : {0.2f, 0.5f}) {
          const auto light = [lightZ] {
            auto value = std::make_unique<LightInfo>();
            value->position = Float4(0.0f, 2.0f, lightZ, 1.0f);
            value->emittance = RGBS(1.0f, 1.0f, 1.0f);
            value->halfExtents = Float4(1.0f, 0.0f, 1.0f, 0.0f);
            value->emittingFaces = LightInfo::faceBit(1, false);
            return value;
          };
          const auto sampled = floorLighting(nothing(), light(), 64, roughness, 1.0f).specular;
          const auto representative = floorLighting(nothing(), light(), 0, roughness, 1.0f).specular;

          constexpr int Steps = 400;
          auto reference = 0.0;
          for (auto i = 0; i < Steps; ++i) {
            for (auto j = 0; j < Steps; ++j) {
              const auto toPoint =
                  Float4(-1.0f + (2.0f * i + 1.0f) / Steps, 3.0f, lightZ - 1.0f + (2.0f * j + 1.0f) / Steps, 0.0f);
              const auto distanceSquared = dot3(toPoint, toPoint);
              const auto L = toPoint / std::sqrt(distanceSquared);
              const auto H = normalize3(V + L);
              const auto NoL = y(L);
              // the cosine at the light is NoL too
              reference += static_cast<double>(brdf::evaluateGGX_DG(y(V), NoL, y(H), roughness)) * NoL * NoL
                           / distanceSquared;
            }
          }
          reference *= 4.0 / (static_cast<double>(Steps) * Steps);
          Assert::AreEqual(reference, static_cast<double>(sampled), 0.05 * reference, L"64 rays", LINE_INFO());
          // the representative point (Karis 2013) without samples is a rough approximation for so large a light
          Assert::IsTrue(
              representative > 0.25 * reference && representative < 2.0 * reference, L"without samples", LINE_INFO()
          );
        }
      }
    }

    TEST_METHOD(sphereLightIrradianceIsExactAboveTheHorizon) {
      // a sphere light of radiance 1 close by (center 1.41 away, radius 0.9), entirely above the floor point's
      // horizon: the light from its center, of the intensity pi r^2 with quadratic falloff, is its exact irradiance
      const auto radius = 0.9f;
      auto light = std::make_unique<LightInfo>();
      light->position = Float4(1.0f, 0.0f, 0.0f, 1.0f);
      light->emittance = RGBS(zeroW(Float4(Pi<Float>() * radius * radius)));
      light->attenuationFactors = Float4(0.0f, 0.0f, 1.0f, 0.0f);
      light->radius = radius;
      const auto lit = floorLighting(nothing(), std::move(light)).diffuse;

      // the integral of the cosine over the sphere's solid angle, in polar coordinates around its center w
      const auto distance = std::sqrt(2.0);
      const auto maxAngle = std::asin(radius / distance);
      const auto wy = 1.0 / distance;
      const auto vy = 1.0 / distance; // v = w x u with u = z
      constexpr int Steps = 1000;
      auto irradiance = 0.0;
      for (auto i = 0; i < Steps; ++i) {
        const auto angle = maxAngle * (i + 0.5) / Steps;
        for (auto j = 0; j < Steps; ++j) {
          const auto around = 2.0 * Pi<double>() * (j + 0.5) / Steps;
          const auto cosine = std::cos(angle) * wy - std::sin(angle) * std::sin(around) * vy;
          irradiance += std::max(cosine, 0.0) * std::sin(angle);
        }
      }
      irradiance *= maxAngle / Steps * 2.0 * Pi<double>() / Steps;
      Assert::AreEqual(irradiance, static_cast<double>(lit), 1e-3 * irradiance, L"exact irradiance", LINE_INFO());
    }
  };
}
