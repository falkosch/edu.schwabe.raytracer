#include "raytracing_tests.h"

#include <raytracing/common/RefractionIndices.h>
#include <raytracing/shading/brdf/SpecularFresnel.h>
#include <raytracing/shading/brdf/fresnel_reflectance.h>
#include <raytracing/shading/brdf/ggx.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(SpecularFresnelTest) {
    static spectral::SpectralVector constant(const Float value) {
      return spectral::SpectralVector(value);
    }

  public:
    TEST_METHOD(opaqueLobeIsTheWeightTimesSchlicksApproximationOfF0) {
      const auto lobe = brdf::SpecularFresnel::opaque(constant(0.5f), constant(0.04f));
      const auto expected = 0.5f * (0.04f + 0.96f * std::pow(0.4f, 5.0f));
      Assert::AreEqual(expected, x1(lobe.reflectance(Float4(0.6f))), 1e-6f, L"lane 0", LINE_INFO());
      Assert::AreEqual(expected, x8(lobe.reflectance(Float4(0.6f))), 1e-6f, L"lane 7", LINE_INFO());
    }

    TEST_METHOD(dielectricLobeIsTheWeightTimesTheIndicesFresnelReflectance) {
      const auto eta = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass);
      const auto lobe = brdf::SpecularFresnel::dielectricInterface(constant(0.5f), eta);
      for (const auto cosine : {1.0f, 0.7f, 0.2f}) {
        const auto expected = 0.5f * x(brdf::fresnelReflectance(false, Float4(cosine), eta));
        Assert::AreEqual(expected, x1(lobe.reflectance(Float4(cosine))), 1e-6f, L"cosine", LINE_INFO());
      }
    }

    TEST_METHOD(dielectricLobeReflectsItsWeightBeyondTheCriticalAngle) {
      // leaving glass at 60 degrees, beyond the critical angle of 41.8 degrees; at 30 degrees it transmits
      const auto eta = RefractionIndices::refractionEta(RefractionIndices::Glass, RefractionIndices::Vacuum);
      const auto lobe = brdf::SpecularFresnel::dielectricInterface(constant(0.8f), eta);
      Assert::AreEqual(0.8f, x1(lobe.reflectance(Float4(0.5f))), 1e-6f, L"total reflection", LINE_INFO());
      Assert::IsTrue(x1(lobe.reflectance(Float4(0.866f))) < 0.1f, L"transmits", LINE_INFO());
    }

    TEST_METHOD(weightIsClampedToOne) {
      // as an upsampled white reflectance channel gives it (above 1 at some wavelengths)
      const auto lobe = brdf::SpecularFresnel::opaque(constant(1.06f), constant(1.0f));
      Assert::AreEqual(1.0f, x1(lobe.reflectance(Float4(0.5f))), 1e-6f, L"at most 1", LINE_INFO());
    }

    TEST_METHOD(weightAndF0ArePerWavelength) {
      const auto weight = Float8{0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f};
      const auto f0 = Float8{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f, 0.0f};
      const auto reflectance = brdf::SpecularFresnel::opaque(weight, f0).reflectance(One<Float4>());
      Assert::AreEqual(0.0f, x1(reflectance), 1e-6f, L"lane 0", LINE_INFO());
      Assert::AreEqual(0.5f, x3(reflectance), 1e-6f, L"lane 2", LINE_INFO());
      Assert::AreEqual(0.5f, x6(reflectance), 1e-6f, L"lane 5", LINE_INFO());
      Assert::AreEqual(0.0f, x8(reflectance), 1e-6f, L"lane 7", LINE_INFO());
    }

    TEST_METHOD(cosinesOutsideTheUnitRangeAreClamped) {
      // a smoothed normal can face away from the viewer: no reflectance above the weight
      const auto lobe = brdf::SpecularFresnel::opaque(constant(1.0f), constant(0.04f));
      Assert::AreEqual(1.0f, x1(lobe.reflectance(Float4(-0.1f))), 1e-6f, L"below 0", LINE_INFO());
      Assert::AreEqual(0.04f, x1(lobe.reflectance(Float4(1.0001f))), 1e-6f, L"above 1", LINE_INFO());
    }

    TEST_METHOD(lobeReflectsWithAWeightAndAReflectance) {
      const auto glass = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass);
      const auto vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum);
      Assert::IsFalse(brdf::SpecularFresnel().reflects(), L"default", LINE_INFO());
      Assert::IsTrue(brdf::SpecularFresnel::opaque(constant(1.0f), constant(0.04f)).reflects(), L"F0", LINE_INFO());
      Assert::IsFalse(brdf::SpecularFresnel::opaque(constant(1.0f), constant(0.0f)).reflects(), L"F0 0", LINE_INFO());
      Assert::IsFalse(brdf::SpecularFresnel::opaque(constant(0.0f), constant(0.5f)).reflects(), L"weight 0",
                      LINE_INFO());
      Assert::IsTrue(brdf::SpecularFresnel::dielectricInterface(constant(1.0f), glass).reflects(), L"glass",
                     LINE_INFO());
      Assert::IsFalse(brdf::SpecularFresnel::dielectricInterface(constant(1.0f), vacuum).reflects(), L"vacuum",
                      LINE_INFO());
      Assert::IsTrue(brdf::SpecularFresnel::dielectricInterface(constant(1.0f), glass).isDielectric(),
                     L"dielectric", LINE_INFO());
    }
  };
}
