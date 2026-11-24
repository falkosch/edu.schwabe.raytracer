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
#include <raytracing/shading/SphereLightCap.h>
#include <raytracing/shading/brdf/apply_brdf.h>
#include <raytracing/shading/brdf/ggx.h>
#include <raytracing/shading/power_heuristic.h>
#include <raytracing/shading/sampling.h>
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
          incident, Float4(1e-6f), 0.5f, Zero<spectral::SpectralVector>(), floor, sphereLightSamples, 0, shadowCache,
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
        const Float roughness = 0.5f, const Float F0 = 0.0f, const ASizeT glossyRays = 0
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
      floor.smoothedNdotI = Float4(dot3(floor.smoothedNormal, direction));
      const Raytrace incident{
          RayCast{Ray{Float4(0.0f, 0.0f, -2.0f, 1.0f), direction}, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f},
          nullptr, 0, 1.0f, &context
      };
      PerLightShadowCache::ShadowCacheType shadowCache(1);
      StatisticsCookie statistics{};

      const auto lighting = scene.sampleLighting(
          incident, Float4(1e-6f), roughness, spectral::SpectralVector(F0), floor, samples, glossyRays, shadowCache,
          statistics
      );
      const auto white = horizontalSum(context.fromRGB(Float4(1.0f, 1.0f, 1.0f, 0.0f)).data);
      return {horizontalSum(lighting.diffuse.data) / white, horizontalSum(lighting.specular.data) / white};
    }

    // The floor point of floorLighting, seen from (0, 0, -2)
    static SceneIntersection floorPoint() {
      SceneIntersection floor{};
      floor.vertex = Float4(0.0f, -1.0f, 0.0f, 1.0f);
      floor.surfaceNormal = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      floor.smoothedNormal = floor.surfaceNormal;
      floor.smoothedNdotI = Float4(dot3(floor.smoothedNormal, normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f))));
      return floor;
    }

    struct PointDensity {
      SceneShader::LightPointDensity density;
      // the radiance averaged over the wavelengths, divided by the average of upsampled white
      Float radiance;
    };

    // SceneShader::lightPointDensity of the light, alone in a scene, towards its point toPoint from the intersection
    // (seen from (0, 0, -2) as floorLighting's point), with the cutoff
    static PointDensity densityAt(
        const LightInfo &light, const SceneIntersection &intersection, const Float4 &toPoint,
        const Float cutoff = 1e-6f
    ) {
      Scene scene{};
      scene.getSceneObjects().push_back(nothing());
      scene.buildSceneGraph();
      scene.getLights().push_back(std::make_unique<LightInfo>(light));
      const auto context = scene.createSpectralContext(spectral::LAMBDA_MIN + 0.5f * spectral::LAMBDA_RANGE);
      const Raytrace incident{
          RayCast{Ray{Float4(0.0f, 0.0f, -2.0f, 1.0f), normalize3(intersection.vertex - Float4(0, 0, -2, 1))},
                  Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f},
          nullptr, 0, 1.0f, &context
      };
      const auto density =
          scene.lightPointDensity(*scene.getLights().front(), incident, Float4(cutoff), intersection, toPoint);
      const auto white = horizontalSum(context.fromRGB(Float4(1.0f, 1.0f, 1.0f, 0.0f)).data);
      return {density, horizontalSum(density.radiance.data) / white};
    }

    // The glossy rays' part of a light's highlight at the floor point of floorLighting (F0 = 1) with multiple
    // importance sampling, as Raytracer::traceGlossyReflection weighs the emitter: VNDF samples that find the light
    // (distanceTo, negative for a miss) add its radiance times F * G2 / G1(V), weighed by the power heuristic against
    // the light's shadow rays. The estimate of glossyRays rays, from many more.
    template <typename DistanceTo>
    static double glossyPart(
        const LightInfo &light, const Float roughness, const ASizeT glossyRays, const ASizeT lightSamples,
        const DistanceTo &distanceTo
    ) {
      const auto floor = floorPoint();
      const auto N = floor.smoothedNormal;
      const auto V = -normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f));
      const auto NoV = dot3(N, V);
      Scene scene{};
      scene.getSceneObjects().push_back(nothing());
      scene.buildSceneGraph();
      scene.getLights().push_back(std::make_unique<LightInfo>(light));
      const auto context = scene.createSpectralContext(spectral::LAMBDA_MIN + 0.5f * spectral::LAMBDA_RANGE);
      const Raytrace incident{
          RayCast{Ray{Float4(0.0f, 0.0f, -2.0f, 1.0f), -V}, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f},
          nullptr, 0, 1.0f, &context
      };
      const auto white = horizontalSum(context.fromRGB(Float4(1.0f, 1.0f, 1.0f, 0.0f)).data);
      constexpr ASizeT Samples = 1 << 16;
      auto sum = 0.0;
      for (ASizeT i = 0; i < Samples; ++i) {
        const auto L = brdf::sampleGGX(N, V, sequencePoint2D(7u, i), roughness);
        const auto NoL = dot3(N, L);
        const auto distance = distanceTo(L);
        if (!(NoL > 0.0f) || distance < 0.0f) {
          continue;
        }
        const auto density =
            scene.lightPointDensity(*scene.getLights().front(), incident, Float4(1e-6f), floor, L * distance);
        if (!density.emits) {
          continue;
        }
        const auto radiance = horizontalSum(density.radiance.data) / white;
        const auto NoH = dot3(N, normalize3(V + L));
        const auto weight = powerHeuristic(
            static_cast<Float>(glossyRays), brdf::pdfGGX(NoV, NoH, roughness), static_cast<Float>(lightSamples),
            density.pdf
        );
        sum += static_cast<double>(
            radiance * brdf::ggxG_smith(NoV, NoL, roughness) / brdf::smithG1(NoV, roughness) * weight
        );
      }
      return sum / static_cast<double>(Samples);
    }

    // A sphere light of radiance 1 (the intensity pi r^2 with quadratic falloff) and the radius, the distance along the
    // floor point's reflection (0, 1, 2) / sqrt(5)
    static std::unique_ptr<LightInfo> sphereLightInReflection(const Float radius, const Float distance) {
      auto light = std::make_unique<LightInfo>();
      light->position = Float4(0.0f, -1.0f, 0.0f, 1.0f) + normalize3(Float4(0.0f, 1.0f, 2.0f, 0.0f)) * distance;
      light->emittance = RGBS(zeroW(Float4(Pi<Float>() * radius * radius)));
      light->attenuationFactors = Float4(0.0f, 0.0f, 1.0f, 0.0f);
      light->radius = radius;
      return light;
    }

    // The distance from the floor point along L to the sphere light's near side, negative for a miss
    static Float distanceToSphere(const LightInfo &light, const Float4 &L) {
      const auto toCenter = light.position - Float4(0.0f, -1.0f, 0.0f, 1.0f);
      const auto along = dot3(toCenter, L);
      const auto squared = sqr(light.radius) - (dot3(toCenter, toCenter) - sqr(along));
      return squared >= 0.0f ? along - std::sqrt(squared) : -1.0f;
    }

    // The integral of the GGX BRDF (F = 1) times the cosine over the sphere light's solid angle, seen from the floor
    // point, in polar coordinates around the direction to its center
    static double sphereLightHighlightReference(const LightInfo &light, const Float roughness) {
      const auto floor = floorPoint();
      const auto V = -normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f));
      const auto cap = SphereLightCap(light, floor.vertex);
      const auto maxAngle = std::asin(static_cast<double>(light.radius / cap.centerDistance));
      constexpr int Steps = 1000;
      auto sum = 0.0;
      for (auto i = 0; i < Steps; ++i) {
        const auto angle = maxAngle * (i + 0.5) / Steps;
        for (auto j = 0; j < Steps; ++j) {
          const auto around = 2.0 * Pi<double>() * (j + 0.5) / Steps;
          const auto L = normalize3(
              cap.w * static_cast<Float>(std::cos(angle))
              + (cap.u * static_cast<Float>(std::cos(around)) + cap.v * static_cast<Float>(std::sin(around)))
                    * static_cast<Float>(std::sin(angle))
          );
          const auto NoL = y(L);
          if (NoL <= 0.0f) {
            continue;
          }
          const auto H = normalize3(V + L);
          sum += static_cast<double>(brdf::evaluateGGX_DG(y(V), NoL, y(H), roughness)) * NoL * std::sin(angle);
        }
      }
      return sum * maxAngle / Steps * 2.0 * Pi<double>() / Steps;
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

    TEST_METHOD(whiteSurfaceUnderALightFillingItsHemisphereShowsTheLightsRadiance) {
      // a light of radiance 1 over the whole hemisphere of the floor point gives the irradiance pi, which a white
      // Lambert surface (diffusion / pi) reflects as radiance 1: no more light than arrives (white furnace)
      const auto lit = floorLighting(nothing(), squareLight(1000.0f, 2.0f));
      BRDFParameters parameters;
      parameters.surface.diffusion = spectral::Spectrum::constant(1.0f);
      parameters.lighting.diffuse = spectral::Spectrum::constant(lit.diffuse);
      Assert::AreEqual(Pi<Float>(), lit.diffuse, 1e-3f, L"irradiance", LINE_INFO());
      Assert::AreEqual(1.0f, x1(brdf::applyBRDF(parameters).data), 1e-3f, L"reflected radiance", LINE_INFO());
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

    TEST_METHOD(boxLightHighlightWithGlossyRaysIsTheIntegratedBRDF) {
      // the shadow rays' highlight, weighed against 4 glossy rays, plus the glossy rays' weighed part: the integrated
      // BRDF of boxLightHighlightIsTheIntegratedBRDF, also for a lobe much narrower than the light
      const auto direction = normalize3(Float4(0.0f, -1.0f, 2.0f, 0.0f));
      const auto V = -direction;
      for (const auto roughness : {0.05f, 0.2f}) {
        const auto light = [] {
          auto value = std::make_unique<LightInfo>();
          value->position = Float4(0.0f, 2.0f, 6.0f, 1.0f);
          value->emittance = RGBS(1.0f, 1.0f, 1.0f);
          value->halfExtents = Float4(1.0f, 0.0f, 1.0f, 0.0f);
          value->emittingFaces = LightInfo::faceBit(1, false);
          return value;
        };
        constexpr ASizeT GlossyRays = 4;
        constexpr ASizeT ShadowRays = 1024;
        const auto shadowPart = floorLighting(nothing(), light(), ShadowRays, roughness, 1.0f, GlossyRays).specular;
        const auto alone = floorLighting(nothing(), light(), ShadowRays, roughness, 1.0f).specular;
        const auto lightInfo = light();
        const auto glossy = glossyPart(*lightInfo, roughness, GlossyRays, ShadowRays, [](const Float4 &L) {
          // the plane y = 2, 3 above the floor point, within the square
          const auto distance = 3.0f / y(L);
          const auto hit = L * distance;
          return std::abs(x(hit)) <= 1.0f && std::abs(z(hit) - 6.0f) <= 1.0f ? distance : -1.0f;
        });

        constexpr int Steps = 400;
        auto reference = 0.0;
        for (auto i = 0; i < Steps; ++i) {
          for (auto j = 0; j < Steps; ++j) {
            const auto toPoint =
                Float4(-1.0f + (2.0f * i + 1.0f) / Steps, 3.0f, 5.0f + (2.0f * j + 1.0f) / Steps, 0.0f);
            const auto distanceSquared = dot3(toPoint, toPoint);
            const auto L = toPoint / std::sqrt(distanceSquared);
            const auto NoL = y(L);
            reference += static_cast<double>(brdf::evaluateGGX_DG(y(V), NoL, y(normalize3(V + L)), roughness)) * NoL
                         * NoL / distanceSquared;
          }
        }
        reference *= 4.0 / (static_cast<double>(Steps) * Steps);
        Assert::IsTrue(shadowPart < alone, L"the glossy rays take a part", LINE_INFO());
        Assert::AreEqual(reference, shadowPart + glossy, 0.03 * reference, L"both parts", LINE_INFO());
      }
    }

    TEST_METHOD(sphereLightHighlightIsTheIntegratedBRDF) {
      // a sphere light of radius 0.5, 3 away along the reflection: the shadow rays' highlight alone, and weighed
      // against 4 glossy rays plus their part, integrate the BRDF over the sphere's solid angle
      for (const auto roughness : {0.05f, 0.3f}) {
        constexpr ASizeT GlossyRays = 4;
        constexpr ASizeT ShadowRays = 1024;
        const auto reference = sphereLightHighlightReference(*sphereLightInReflection(0.5f, 3.0f), roughness);
        const auto alone =
            floorLighting(nothing(), sphereLightInReflection(0.5f, 3.0f), ShadowRays, roughness, 1.0f).specular;
        const auto shadowPart =
            floorLighting(nothing(), sphereLightInReflection(0.5f, 3.0f), ShadowRays, roughness, 1.0f, GlossyRays)
                .specular;
        const auto light = sphereLightInReflection(0.5f, 3.0f);
        const auto glossy = glossyPart(*light, roughness, GlossyRays, ShadowRays, [&light](const Float4 &L) {
          return distanceToSphere(*light, L);
        });
        Assert::AreEqual(reference, static_cast<double>(alone), 0.03 * reference, L"shadow rays alone", LINE_INFO());
        Assert::AreEqual(reference, shadowPart + glossy, 0.03 * reference, L"both parts", LINE_INFO());
      }
    }

    TEST_METHOD(lightPointDensityOfABoxFaceIsPerSolidAngle) {
      // the square of 2 x 2 3 above the floor point, all above its horizon: the area density 1/4 per solid angle
      const auto light = squareLight(1.0f, 2.0f);
      const auto toPoint = Float4(0.3f, 3.0f, -0.2f, 0.0f);
      const auto distance = length3(toPoint);
      const auto [density, radiance] = densityAt(*light, floorPoint(), toPoint);
      Assert::IsTrue(density.emits, L"the face emits towards the point", LINE_INFO());
      Assert::AreEqual(distance * distance / (3.0f / distance * 4.0f), density.pdf, 1e-4f, L"density", LINE_INFO());
      Assert::AreEqual(1.0f, radiance, 1e-3f, L"the light's radiance", LINE_INFO());
    }

    TEST_METHOD(lightPointDensityOfANonEmittingFaceIsNoLight) {
      // a box of 2 x 1 x 2 whose bottom face (y = 1.5) emits: its side x = 1 seen from the floor point doesn't
      auto light = squareLight(1.0f, 2.0f);
      light->halfExtents = Float4(1.0f, 0.5f, 1.0f, 0.0f);
      const auto bottom = densityAt(*light, floorPoint(), Float4(0.2f, 2.5f, 0.1f, 0.0f)).density;
      const auto side = densityAt(*light, floorPoint(), Float4(1.0f, 3.0f, 0.2f, 0.0f)).density;
      Assert::IsTrue(bottom.emits && bottom.pdf > 0.0f, L"the bottom face", LINE_INFO());
      Assert::IsFalse(side.emits, L"the side face", LINE_INFO());
    }

    TEST_METHOD(lightPointDensityOfASphereLightIsItsCapsDensity) {
      // above the horizon the cap's density; a sphere whose center is below the horizon has no shadow rays
      const auto floor = floorPoint();
      const auto above = sphereLightInReflection(0.5f, 3.0f);
      const auto toPoint = SphereLightCap(*above, floor.vertex).samplePoint(Float4(0.3f, 0.6f, 0.0f, 0.0f));
      const auto [density, radiance] = densityAt(*above, floor, toPoint);
      Assert::IsTrue(density.emits, L"the sphere emits", LINE_INFO());
      Assert::AreEqual(SphereLightCap(*above, floor.vertex).solidAnglePdf(toPoint), density.pdf, 1e-6f, L"density",
                       LINE_INFO());
      Assert::AreEqual(1.0f, radiance, 1e-3f, L"the intensity over the cross section", LINE_INFO());

      auto below = sphereLightInReflection(0.5f, 3.0f);
      below->position = Float4(0.0f, -1.2f, 3.0f, 1.0f);
      const auto onBelow = Float4(0.0f, -1.2f, 3.0f, 1.0f) + normalize3(Float4(0.0f, 0.4f, -1.0f, 0.0f)) * 0.5f;
      const auto hidden = densityAt(*below, floor, onBelow - floor.vertex).density;
      Assert::IsTrue(hidden.emits, L"the sphere still emits", LINE_INFO());
      Assert::AreEqual(0.0f, hidden.pdf, L"but isn't sampled", LINE_INFO());
    }

    TEST_METHOD(lightPointDensityOfAPointOffTheLightIsNoLight) {
      // an emitter that doesn't match its light (moved away from it) isn't the light
      const auto floor = floorPoint();
      const auto light = sphereLightInReflection(0.5f, 3.0f);
      const auto onSphere = SphereLightCap(*light, floor.vertex).samplePoint(Float4(0.3f, 0.6f, 0.0f, 0.0f));
      Assert::IsTrue(densityAt(*light, floor, onSphere).density.emits, L"on the sphere", LINE_INFO());
      Assert::IsFalse(densityAt(*light, floor, onSphere * 0.9f).density.emits, L"in front of it", LINE_INFO());
    }

    TEST_METHOD(lightPointDensityIsZeroWhereTheShadowRaysDontSampleTheHighlight) {
      // below the cutoff, and seen from inside the object, sampleLighting adds no highlight of the light: the glossy
      // rays take the whole weight
      const auto floor = floorPoint();
      const auto sphere = sphereLightInReflection(0.5f, 3.0f);
      const auto onSphere = SphereLightCap(*sphere, floor.vertex).samplePoint(Float4(0.3f, 0.6f, 0.0f, 0.0f));
      const auto square = squareLight(1.0f, 2.0f);
      const auto onSquare = Float4(0.3f, 3.0f, -0.2f, 0.0f);
      Assert::IsTrue(densityAt(*sphere, floor, onSphere).density.pdf > 0.0f, L"sampled", LINE_INFO());
      Assert::AreEqual(0.0f, densityAt(*sphere, floor, onSphere, 1e3f).density.pdf, L"sphere below the cutoff",
                       LINE_INFO());
      Assert::AreEqual(0.0f, densityAt(*square, floor, onSquare, 1e3f).density.pdf, L"box below the cutoff",
                       LINE_INFO());

      auto inside = floor;
      inside.smoothedNdotI = -floor.smoothedNdotI;
      Assert::AreEqual(0.0f, densityAt(*sphere, inside, onSphere).density.pdf, L"sphere from inside", LINE_INFO());
      Assert::AreEqual(0.0f, densityAt(*square, inside, onSquare).density.pdf, L"box from inside", LINE_INFO());
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
