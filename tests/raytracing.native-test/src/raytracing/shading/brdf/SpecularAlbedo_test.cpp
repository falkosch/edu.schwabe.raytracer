#include "raytracing_tests.h"

#include <raytracing/common/RefractionIndices.h>
#include <raytracing/shading/brdf/SpecularAlbedo.h>
#include <raytracing/shading/brdf/SpecularFresnel.h>
#include <raytracing/shading/brdf/fresnel_reflectance.h>
#include <raytracing/shading/brdf/ggx.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(SpecularAlbedoTest) {
    // The directional albedo of the GGX lobe of F = 1 at the cosine, by midpoint quadrature over the hemisphere of
    // the BRDF D G2 / (4 NoV NoL) times NoL: independent of the tables' VNDF samples
    static double integratedAlbedo(const Float cosine, const Float roughness) {
      constexpr int Steps = 400;
      const auto V = Float4(std::sqrt(1.0f - cosine * cosine), 0.0f, cosine, 0.0f);
      auto sum = 0.0;
      for (auto i = 0; i < Steps; ++i) {
        const auto theta = 0.5 * Pi<double>() * (i + 0.5) / Steps;
        for (auto j = 0; j < Steps; ++j) {
          const auto phi = 2.0 * Pi<double>() * (j + 0.5) / Steps;
          const auto L = Float4(static_cast<Float>(std::sin(theta) * std::cos(phi)),
                                static_cast<Float>(std::sin(theta) * std::sin(phi)),
                                static_cast<Float>(std::cos(theta)), 0.0f);
          const auto H = normalize3(V + L);
          const auto NoL = z(L);
          sum += static_cast<double>(brdf::evaluateGGX_DG(cosine, NoL, z(H), roughness) * NoL) * std::sin(theta);
        }
      }
      return sum * (0.5 * Pi<double>() / Steps) * (2.0 * Pi<double>() / Steps);
    }

  public:
    TEST_METHOD(mirrorAlbedoIsItsFresnelReflectance) {
      const auto& tables = brdf::SpecularAlbedo::tables();
      const auto glass = RefractionIndices::refractionEta(1.0f, 1.5f);
      for (const auto cosine : {0.984375f, 0.703125f, 0.296875f, 0.046875f}) {
        const auto schlick = tables.schlick(cosine, 0.0f);
        Assert::AreEqual(1.0f, x(schlick), 1e-3f, L"E_1", LINE_INFO());
        Assert::AreEqual(std::pow(1.0f - cosine, 5.0f), y(schlick), 1e-3f, L"E_5", LINE_INFO());
        const auto dielectric = tables.dielectric(cosine, 0.0f, glass);
        const auto F = x(brdf::fresnelReflectance(Float4(cosine), glass));
        Assert::AreEqual(F, x(dielectric), 1e-3f, L"E_R", LINE_INFO());
        Assert::AreEqual(1.0f - F, y(dielectric), 1e-3f, L"E_T", LINE_INFO());
      }
    }

    TEST_METHOD(roughAlbedoIsTheIntegratedBRDF) {
      // single scattering loses energy, the more the rougher and the more grazing
      const auto& tables = brdf::SpecularAlbedo::tables();
      for (const auto roughness : {0.258064f, 0.516129f, 1.0f}) {
        for (const auto cosine : {0.890625f, 0.484375f, 0.140625f}) {
          const auto reference = static_cast<Float>(integratedAlbedo(cosine, roughness));
          Assert::AreEqual(reference, x(tables.schlick(cosine, roughness)), 0.01f, L"E_1", LINE_INFO());
          Assert::IsTrue(reference < 0.995f, L"loses energy", LINE_INFO());
        }
      }
    }

    TEST_METHOD(averageAlbedoIsTheCosineWeightedMean) {
      // a mirror's Schlick term: 2 integral of (1 - mu)^5 mu dmu = 1 / 21
      const auto& tables = brdf::SpecularAlbedo::tables();
      Assert::AreEqual(1.0f, x(tables.averageSchlick(0.0f)), 1e-3f, L"E_1 avg", LINE_INFO());
      Assert::AreEqual(1.0f / 21.0f, y(tables.averageSchlick(0.0f)), 2e-3f, L"E_5 avg", LINE_INFO());
      Assert::IsTrue(x(tables.averageSchlick(1.0f)) < x(tables.averageSchlick(0.5f)), L"rougher, darker",
                     LINE_INFO());
    }

    TEST_METHOD(dielectricLobesKeepAtMostTheLight) {
      // from either side of glass; leaving it, total reflection keeps the light in the reflected lobe
      const auto& tables = brdf::SpecularAlbedo::tables();
      const auto entering = RefractionIndices::refractionEta(1.0f, 1.5f);
      const auto leavingGlass = RefractionIndices::refractionEta(1.5f, 1.0f);
      for (const auto eta : {entering, leavingGlass}) {
        for (const auto roughness : {0.0f, 0.3f, 0.8f}) {
          for (const auto cosine : {0.9f, 0.5f, 0.1f}) {
            const auto albedo = tables.dielectric(cosine, roughness, eta);
            Assert::IsTrue(x(albedo) + y(albedo) < 1.005f, L"at most the light", LINE_INFO());
            Assert::IsTrue(x(albedo) + y(albedo) > 0.5f, L"most of it", LINE_INFO());
          }
        }
      }
      const auto leaving = tables.dielectric(0.3f, 0.0f, RefractionIndices::refractionEta(1.5f, 1.0f));
      Assert::AreEqual(1.0f, x(leaving), 1e-3f, L"total reflection", LINE_INFO());
    }

    TEST_METHOD(compensatedWhiteLobesReflectAllTheLight) {
      // Turquin 2019: a conductor of F = 1 and both lobes of a dielectric interface keep all the light at every angle
      // and roughness, and on average
      const auto white = brdf::SpecularFresnel::opaque(spectral::SpectralVector(1.0f), spectral::SpectralVector(1.0f));
      const auto glass = brdf::SpecularFresnel::dielectricInterface(
          spectral::SpectralVector(1.0f), RefractionIndices::refractionEta(1.0f, 1.5f)
      );
      const auto& tables = brdf::SpecularAlbedo::tables();
      for (const auto roughness : {0.2f, 0.6f, 1.0f}) {
        for (const auto cosine : {0.9f, 0.5f, 0.1f}) {
          Assert::AreEqual(1.0f, x1(white.directionalAlbedo(cosine, roughness)), 1e-5f, L"conductor", LINE_INFO());
          const auto lobes = tables.dielectric(cosine, roughness, RefractionIndices::refractionEta(1.0f, 1.5f));
          Assert::AreEqual(1.0f, (x(lobes) + y(lobes)) * x1(glass.multipleScattering(cosine, roughness)), 1e-5f,
                           L"dielectric", LINE_INFO());
        }
        Assert::AreEqual(1.0f, x1(white.averageAlbedo(roughness)), 2e-3f, L"conductor's mean", LINE_INFO());
        const auto averages = tables.averageDielectric(roughness, RefractionIndices::refractionEta(1.0f, 1.5f));
        Assert::AreEqual(1.0f, x(averages) + y(averages), 1e-5f, L"dielectric's mean", LINE_INFO());
      }
    }

    TEST_METHOD(lobeAlbedoIsTheWeightTimesItsFresnelsAlbedo) {
      const auto& tables = brdf::SpecularAlbedo::tables();
      const auto lobe = brdf::SpecularFresnel::opaque(spectral::SpectralVector(0.5f), spectral::SpectralVector(0.04f));
      const auto schlick = tables.schlick(0.4f, 0.3f);
      const auto compensation = 1.0f + 0.04f * (1.0f - x(schlick)) / x(schlick);
      Assert::AreEqual(0.5f * (0.04f * x(schlick) + 0.96f * y(schlick)) * compensation,
                       x1(lobe.directionalAlbedo(0.4f, 0.3f)), 1e-6f, L"F0 E_1 + (1 - F0) E_5, compensated",
                       LINE_INFO());
      Assert::AreEqual(compensation, x1(lobe.multipleScattering(0.4f, 0.3f)), 1e-6f, L"1 + F0 (1 - E_1) / E_1",
                       LINE_INFO());
      const auto glass = RefractionIndices::refractionEta(1.0f, 1.5f);
      const auto dielectric = brdf::SpecularFresnel::dielectricInterface(spectral::SpectralVector(1.0f), glass);
      Assert::AreEqual(x(tables.averageDielectric(0.3f, glass)), x8(dielectric.averageAlbedo(0.3f)), 1e-6f, L"E_R avg",
                       LINE_INFO());
      Assert::AreEqual(0.0f, x1(brdf::SpecularFresnel().directionalAlbedo(0.4f, 0.3f)), L"no lobe", LINE_INFO());
    }
  };
}
