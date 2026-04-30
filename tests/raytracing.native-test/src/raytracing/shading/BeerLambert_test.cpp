#include "raytracing_tests.h"

#include <cmath>
#include <limits>

namespace raytracer::test {
  // Beer-Lambert transmittance reproduced from raytracing/Raytracer.cpp::traceTransmission:
  //   fractionTransmitted = exp(-depth / transmittance)
  // where transmittance is a per-channel "absorption length" parameter (large = transparent).
  // These tests lock in the formula via the public vectorization::exp().

  TEST_CLASS(BeerLambertTest) {
  public:
    TEST_METHOD(zeroDistanceGivesUnitTransmittance) {
      const Float4 transmittance{1.0f, 1.0f, 1.0f, 1.0f};
      const Float4 depth{0.0f, 0.0f, 0.0f, 0.0f};
      const auto t = vectorization::exp(-depth / transmittance);

      Assert::AreEqual(1.0f, x(t), 1e-6f, L"zero distance r", LINE_INFO());
      Assert::AreEqual(1.0f, y(t), 1e-6f, L"zero distance g", LINE_INFO());
      Assert::AreEqual(1.0f, z(t), 1e-6f, L"zero distance b", LINE_INFO());
    }

    TEST_METHOD(largeDistanceApproachesZero) {
      const Float4 transmittance{1.0f, 1.0f, 1.0f, 1.0f};
      const Float4 depth{50.0f, 50.0f, 50.0f, 50.0f};
      const auto t = vectorization::exp(-depth / transmittance);

      Assert::AreEqual(0.0f, x(t), 1e-6f, L"large distance r ~ 0", LINE_INFO());
      Assert::AreEqual(0.0f, y(t), 1e-6f, L"large distance g ~ 0", LINE_INFO());
      Assert::AreEqual(0.0f, z(t), 1e-6f, L"large distance b ~ 0", LINE_INFO());
    }

    TEST_METHOD(unitDistanceOverUnitTransmittanceGivesEInverse) {
      const Float4 transmittance{1.0f, 1.0f, 1.0f, 1.0f};
      const Float4 depth{1.0f, 1.0f, 1.0f, 1.0f};
      const auto t = vectorization::exp(-depth / transmittance);

      const Float expected = std::exp(-1.0f);
      Assert::AreEqual(expected, x(t), 1e-5f, L"r = 1/e", LINE_INFO());
      Assert::AreEqual(expected, y(t), 1e-5f, L"g = 1/e", LINE_INFO());
    }

    TEST_METHOD(perChannelIndependence) {
      // Different transmittance per channel: red passes through (large), green absorbs (small)
      const Float4 transmittance{100.0f, 0.5f, 100.0f, 1.0f};
      const Float4 depth{1.0f, 1.0f, 1.0f, 1.0f};
      const auto t = vectorization::exp(-depth / transmittance);

      Assert::IsTrue(x(t) > 0.99f, L"red channel transmits", LINE_INFO());
      Assert::IsTrue(y(t) < 0.2f, L"green channel absorbs", LINE_INFO());
      Assert::IsTrue(z(t) > 0.99f, L"blue channel transmits", LINE_INFO());
    }

    TEST_METHOD(monotonicDecayWithDistance) {
      const Float4 transmittance{2.0f, 2.0f, 2.0f, 2.0f};
      const Float4 d1{0.5f, 0.5f, 0.5f, 0.5f};
      const Float4 d2{1.0f, 1.0f, 1.0f, 1.0f};
      const Float4 d3{2.0f, 2.0f, 2.0f, 2.0f};
      const auto t1 = vectorization::exp(-d1 / transmittance);
      const auto t2 = vectorization::exp(-d2 / transmittance);
      const auto t3 = vectorization::exp(-d3 / transmittance);

      Assert::IsTrue(x(t1) > x(t2), L"transmittance decays with distance (1)", LINE_INFO());
      Assert::IsTrue(x(t2) > x(t3), L"transmittance decays with distance (2)", LINE_INFO());
    }

    TEST_METHOD(largeTransmittanceCoefficientGivesUnit) {
      // Very large transmittance means almost no absorption
      const Float4 transmittance{1e6f, 1e6f, 1e6f, 1e6f};
      const Float4 depth{1.0f, 1.0f, 1.0f, 1.0f};
      const auto t = vectorization::exp(-depth / transmittance);

      Assert::AreEqual(1.0f, x(t), 1e-5f, L"huge transmittance ~ 1", LINE_INFO());
    }

    TEST_METHOD(transmittanceAlwaysInUnitRange) {
      // Sweep depths and transmittances; result must stay in [0, 1]
      for (Float d : {0.0f, 0.1f, 1.0f, 10.0f, 100.0f}) {
        for (Float k : {0.1f, 1.0f, 10.0f}) {
          const Float4 transmittance{k, k, k, k};
          const Float4 depth{d, d, d, d};
          const auto t = vectorization::exp(-depth / transmittance);
          Assert::IsTrue(x(t) >= 0.0f && x(t) <= 1.0f + 1e-6f, L"transmittance in [0,1]", LINE_INFO());
        }
      }
    }
  };
}
