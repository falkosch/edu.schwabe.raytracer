#include "raytracing_tests.h"

#include <raytracing/common/RGBS.h>

namespace raytracer::test {
  static RGBS applyBRDFFormula(const SurfaceShading &surface, const LightShading &lighting,
                               const RGBS &reflectanceCoefficient, const RGBS &fractionTransmitted) {
    const auto ambient = surface.diffusion * lighting.ambient;
    const auto diffuse = surface.diffusion * lighting.diffuse;
    const auto specularReflection = surface.specular * lighting.specular;
    const auto phong = ambient + diffuse + specularReflection;

    const auto reflection = surface.reflectance * lighting.reflected;
    const auto transmitted = fractionTransmitted * lighting.transmitted;

    return surface.emittance + phong + mix(transmitted.value, reflection.value, reflectanceCoefficient.value);
  }

  TEST_CLASS(BRDFTest) {
  public:
    TEST_METHOD(perpendicularSurfaceWhiteLight) {
      SurfaceShading surface{
          RGBS{1.0f, 1.0f, 1.0f, 1.0f},       // diffusion
          RGBS{0.0f, 0.0f, 0.0f, 0.0f},       // reflectance
          RGBS{0.0f, 0.0f, 0.0f, 0.0f},       // specular
          Float4{0.0f, 0.0f, 0.0f, 0.0f},     // shininess
          Float4{0.0f, 0.0f, 0.0f, 0.0f},     // transmittance
          Float4{1.0f, 1.5f, 1.0f, 1.5f},     // refractionEta
          RGBS{0.0f, 0.0f, 0.0f, 0.0f}        // emittance
      };
      LightShading lighting{RGBS{1.0f, 1.0f, 1.0f, 1.0f}};
      const auto result = applyBRDFFormula(surface, lighting, RGBS::black(), RGBS::black());

      Assert::AreEqual(1.0f, x(result.value), 1e-6f, L"r should be 1", LINE_INFO());
      Assert::AreEqual(1.0f, y(result.value), 1e-6f, L"g should be 1", LINE_INFO());
      Assert::AreEqual(1.0f, z(result.value), 1e-6f, L"b should be 1", LINE_INFO());
    }

    TEST_METHOD(emittanceAddsDirectly) {
      SurfaceShading surface;
      surface.emittance = RGBS{0.7f, 0.5f, 0.3f, 0.0f};
      LightShading lighting;
      const auto result = applyBRDFFormula(surface, lighting, RGBS::black(), RGBS::black());

      Assert::AreEqual(0.7f, x(result.value), 1e-6f, L"emittance r passes through", LINE_INFO());
      Assert::AreEqual(0.5f, y(result.value), 1e-6f, L"emittance g passes through", LINE_INFO());
      Assert::AreEqual(0.3f, z(result.value), 1e-6f, L"emittance b passes through", LINE_INFO());
    }

    TEST_METHOD(diffusionMultipliesAmbient) {
      SurfaceShading surface;
      surface.diffusion = RGBS{0.5f, 0.5f, 0.5f, 0.0f};
      LightShading lighting{RGBS{0.4f, 0.4f, 0.4f, 0.0f}};
      const auto result = applyBRDFFormula(surface, lighting, RGBS::black(), RGBS::black());

      Assert::AreEqual(0.2f, x(result.value), 1e-6f, L"diffusion * ambient", LINE_INFO());
    }

    TEST_METHOD(specularMultipliesLighting) {
      SurfaceShading surface;
      surface.specular = RGBS{0.8f, 0.0f, 0.0f, 0.0f};
      LightShading lighting;
      lighting.specular = RGBS{0.5f, 0.5f, 0.5f, 0.0f};
      const auto result = applyBRDFFormula(surface, lighting, RGBS::black(), RGBS::black());

      Assert::AreEqual(0.4f, x(result.value), 1e-6f, L"specular * lighting.specular", LINE_INFO());
    }

    TEST_METHOD(reflectionCoefficientOnePicksReflection) {
      SurfaceShading surface;
      surface.reflectance = RGBS{0.6f, 0.6f, 0.6f, 0.0f};
      LightShading lighting;
      lighting.reflected = RGBS{0.5f, 0.5f, 0.5f, 0.0f};
      lighting.transmitted = RGBS{0.9f, 0.9f, 0.9f, 0.0f};
      const auto result = applyBRDFFormula(surface, lighting, RGBS::white(), RGBS{1.0f, 1.0f, 1.0f, 0.0f});

      Assert::AreEqual(0.3f, x(result.value), 1e-6f, L"reflectance dominates at coef=1", LINE_INFO());
    }

    TEST_METHOD(reflectionCoefficientZeroPicksTransmission) {
      SurfaceShading surface;
      surface.reflectance = RGBS{0.6f, 0.6f, 0.6f, 0.0f};
      LightShading lighting;
      lighting.reflected = RGBS{0.5f, 0.5f, 0.5f, 0.0f};
      lighting.transmitted = RGBS{0.9f, 0.9f, 0.9f, 0.0f};
      const auto result = applyBRDFFormula(surface, lighting, RGBS::black(), RGBS{1.0f, 1.0f, 1.0f, 0.0f});

      Assert::AreEqual(0.9f, x(result.value), 1e-6f, L"transmission dominates at coef=0", LINE_INFO());
    }

    TEST_METHOD(allZeroProducesZero) {
      SurfaceShading surface;
      LightShading lighting;
      const auto result = applyBRDFFormula(surface, lighting, RGBS::black(), RGBS::black());

      Assert::AreEqual(0.0f, x(result.value), L"empty inputs -> zero", LINE_INFO());
      Assert::AreEqual(0.0f, y(result.value), L"empty inputs -> zero", LINE_INFO());
    }
  };
}
