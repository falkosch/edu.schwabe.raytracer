#include "raytracing_tests.h"

#include <raytracing/shading/beer_lambert_transmittance.h>

#include <cmath>
#include <limits>

namespace raytracer::test {
  // beerLambertTransmittance, as raytracing/Raytracer.cpp::traceTransmission and SceneShader::transmitShadowRay use it:
  //   fractionTransmitted = exp(-depth / transmittance)
  // where transmittance is a per-wavelength "absorption length" parameter (large = transparent).

  TEST_CLASS(BeerLambertTransmittanceTest) {
  public:
    TEST_METHOD(zeroDistanceGivesUnitTransmittance) {
      const auto t = beerLambertTransmittance(Float8(0.0f), Float8(1.0f));

      Assert::AreEqual(1.0f, x1(t), 1e-6f, L"zero distance lane 0", LINE_INFO());
      Assert::AreEqual(1.0f, x4(t), 1e-6f, L"zero distance lane 3", LINE_INFO());
      Assert::AreEqual(1.0f, x8(t), 1e-6f, L"zero distance lane 7", LINE_INFO());
    }

    TEST_METHOD(largeDistanceApproachesZero) {
      const auto t = beerLambertTransmittance(Float8(50.0f), Float8(1.0f));

      Assert::AreEqual(0.0f, x1(t), 1e-6f, L"large distance lane 0 ~ 0", LINE_INFO());
      Assert::AreEqual(0.0f, x4(t), 1e-6f, L"large distance lane 3 ~ 0", LINE_INFO());
      Assert::AreEqual(0.0f, x8(t), 1e-6f, L"large distance lane 7 ~ 0", LINE_INFO());
    }

    TEST_METHOD(unitDistanceOverUnitTransmittanceGivesEInverse) {
      const auto t = beerLambertTransmittance(Float8(1.0f), Float8(1.0f));

      const Float expected = std::exp(-1.0f);
      Assert::AreEqual(expected, x1(t), 1e-5f, L"lane 0 = 1/e", LINE_INFO());
      Assert::AreEqual(expected, x8(t), 1e-5f, L"lane 7 = 1/e", LINE_INFO());
    }

    TEST_METHOD(perWavelengthIndependence) {
      // Different transmittance per wavelength: lanes 0 and 2 pass through (large), lane 1 absorbs (small)
      const Float8 transmittance{100.0f, 0.5f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
      const auto t = beerLambertTransmittance(Float8(1.0f), transmittance);

      Assert::IsTrue(x1(t) > 0.99f, L"lane 0 transmits", LINE_INFO());
      Assert::IsTrue(x2(t) < 0.2f, L"lane 1 absorbs", LINE_INFO());
      Assert::IsTrue(x3(t) > 0.99f, L"lane 2 transmits", LINE_INFO());
    }

    TEST_METHOD(monotonicDecayWithDistance) {
      const Float8 transmittance(2.0f);
      const auto t1 = beerLambertTransmittance(Float8(0.5f), transmittance);
      const auto t2 = beerLambertTransmittance(Float8(1.0f), transmittance);
      const auto t3 = beerLambertTransmittance(Float8(2.0f), transmittance);

      Assert::IsTrue(x1(t1) > x1(t2), L"transmittance decays with distance (1)", LINE_INFO());
      Assert::IsTrue(x1(t2) > x1(t3), L"transmittance decays with distance (2)", LINE_INFO());
    }

    TEST_METHOD(largeTransmittanceCoefficientGivesUnit) {
      // Very large transmittance means almost no absorption
      const auto t = beerLambertTransmittance(Float8(1.0f), Float8(1e6f));

      Assert::AreEqual(1.0f, x1(t), 1e-5f, L"huge transmittance ~ 1", LINE_INFO());
    }

    TEST_METHOD(transmittanceAlwaysInUnitRange) {
      // Sweep depths and transmittances; result must stay in [0, 1]
      for (Float d : {0.0f, 0.1f, 1.0f, 10.0f, 100.0f}) {
        for (Float k : {0.1f, 1.0f, 10.0f}) {
          const auto t = beerLambertTransmittance(Float8(d), Float8(k));
          Assert::IsTrue(x1(t) >= 0.0f && x1(t) <= 1.0f + 1e-6f, L"transmittance in [0,1]", LINE_INFO());
        }
      }
    }
  };
}
