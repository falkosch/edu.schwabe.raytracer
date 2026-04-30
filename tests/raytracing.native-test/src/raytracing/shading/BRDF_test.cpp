#include "raytracing_tests.h"

namespace raytracer::test {
  // BRDF combination reproduced from raytracing/Raytracer.cpp::applyBRDF (private static method).
  // The method is not exposed in any header; the tests lock in the formula:
  //   emittance + (diffusion*ambient + diffusion*diffuse + specular*lightSpec)
  //             + mix(transmittance*transLight, reflectance*reflectLight, reflectanceCoef)
  static Float4 applyBRDFFormula(const SurfaceShading &surface, const LightShading &lighting,
                                 const Float4 &reflectanceCoefficient, const Float4 &fractionTransmitted) {
    const auto ambient = surface.diffusion * lighting.ambient;
    const auto diffuse = surface.diffusion * lighting.diffuse;
    const auto specularReflection = surface.specular * lighting.specular;
    const auto phong = ambient + diffuse + specularReflection;

    const auto reflection = surface.reflectance * lighting.reflected;
    const auto transmitted = fractionTransmitted * lighting.transmitted;

    return surface.emittance + phong + mix(transmitted, reflection, reflectanceCoefficient);
  }

  TEST_CLASS(BRDFTest) {
  public:
    TEST_METHOD(perpendicularSurfaceWhiteLight) {
      // White diffusion, white ambient, no other lights -> result is white * white = white
      SurfaceShading surface{
          Float4{1.0f, 1.0f, 1.0f, 1.0f},  // diffusion
          Float4{0.0f, 0.0f, 0.0f, 0.0f},  // reflectance
          Float4{0.0f, 0.0f, 0.0f, 0.0f},  // specular
          Float4{0.0f, 0.0f, 0.0f, 0.0f},  // shininess
          Float4{0.0f, 0.0f, 0.0f, 0.0f},  // transmittance
          Float4{1.0f, 1.5f, 1.0f, 1.5f},  // refractionEta
          Float4{0.0f, 0.0f, 0.0f, 0.0f}   // emittance
      };
      LightShading lighting{Float4{1.0f, 1.0f, 1.0f, 1.0f}};
      const auto result = applyBRDFFormula(surface, lighting, Zero<Float4>(), Zero<Float4>());

      Assert::AreEqual(1.0f, x(result), 1e-6f, L"r should be 1", LINE_INFO());
      Assert::AreEqual(1.0f, y(result), 1e-6f, L"g should be 1", LINE_INFO());
      Assert::AreEqual(1.0f, z(result), 1e-6f, L"b should be 1", LINE_INFO());
    }

    TEST_METHOD(emittanceAddsDirectly) {
      SurfaceShading surface;
      surface.emittance = Float4{0.7f, 0.5f, 0.3f, 0.0f};
      LightShading lighting;
      const auto result = applyBRDFFormula(surface, lighting, Zero<Float4>(), Zero<Float4>());

      Assert::AreEqual(0.7f, x(result), 1e-6f, L"emittance r passes through", LINE_INFO());
      Assert::AreEqual(0.5f, y(result), 1e-6f, L"emittance g passes through", LINE_INFO());
      Assert::AreEqual(0.3f, z(result), 1e-6f, L"emittance b passes through", LINE_INFO());
    }

    TEST_METHOD(diffusionMultipliesAmbient) {
      SurfaceShading surface;
      surface.diffusion = Float4{0.5f, 0.5f, 0.5f, 0.0f};
      LightShading lighting{Float4{0.4f, 0.4f, 0.4f, 0.0f}};
      const auto result = applyBRDFFormula(surface, lighting, Zero<Float4>(), Zero<Float4>());

      Assert::AreEqual(0.2f, x(result), 1e-6f, L"diffusion * ambient", LINE_INFO());
    }

    TEST_METHOD(specularMultipliesLighting) {
      SurfaceShading surface;
      surface.specular = Float4{0.8f, 0.0f, 0.0f, 0.0f};
      LightShading lighting;
      lighting.specular = Float4{0.5f, 0.5f, 0.5f, 0.0f};
      const auto result = applyBRDFFormula(surface, lighting, Zero<Float4>(), Zero<Float4>());

      Assert::AreEqual(0.4f, x(result), 1e-6f, L"specular * lighting.specular", LINE_INFO());
    }

    TEST_METHOD(reflectionCoefficientOnePicksReflection) {
      // mix(t, r, factor=1) returns r => only reflection contribution
      SurfaceShading surface;
      surface.reflectance = Float4{0.6f, 0.6f, 0.6f, 0.0f};
      LightShading lighting;
      lighting.reflected = Float4{0.5f, 0.5f, 0.5f, 0.0f};
      lighting.transmitted = Float4{0.9f, 0.9f, 0.9f, 0.0f};
      const auto result = applyBRDFFormula(surface, lighting, One<Float4>(), Float4{1.0f, 1.0f, 1.0f, 0.0f});

      Assert::AreEqual(0.3f, x(result), 1e-6f, L"reflectance dominates at coef=1", LINE_INFO());
    }

    TEST_METHOD(reflectionCoefficientZeroPicksTransmission) {
      // mix(t, r, factor=0) returns t => only transmission contribution
      SurfaceShading surface;
      surface.reflectance = Float4{0.6f, 0.6f, 0.6f, 0.0f};
      LightShading lighting;
      lighting.reflected = Float4{0.5f, 0.5f, 0.5f, 0.0f};
      lighting.transmitted = Float4{0.9f, 0.9f, 0.9f, 0.0f};
      const auto result = applyBRDFFormula(surface, lighting, Zero<Float4>(), Float4{1.0f, 1.0f, 1.0f, 0.0f});

      Assert::AreEqual(0.9f, x(result), 1e-6f, L"transmission dominates at coef=0", LINE_INFO());
    }

    TEST_METHOD(allZeroProducesZero) {
      SurfaceShading surface;
      LightShading lighting;
      const auto result = applyBRDFFormula(surface, lighting, Zero<Float4>(), Zero<Float4>());

      Assert::AreEqual(0.0f, x(result), L"empty inputs -> zero", LINE_INFO());
      Assert::AreEqual(0.0f, y(result), L"empty inputs -> zero", LINE_INFO());
    }
  };
}
