#include "raytracing_tests.h"

#include <raytracing/common/RefractionIndices.h>
#include <raytracing/common/StatisticsCookie.h>
#include <raytracing/geometry/forms/Sphere.h>
#include <raytracing/scene/Resources.h>
#include <raytracing/scene/Scene.h>
#include <raytracing/shading/Raytrace.h>
#include <raytracing/shading/spectral/wavelengths.h>

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

  public:
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
  };
}
