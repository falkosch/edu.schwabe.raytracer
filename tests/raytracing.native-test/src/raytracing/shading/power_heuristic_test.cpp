#include "raytracing_tests.h"

#include <raytracing/shading/power_heuristic.h>

namespace raytracer::test {
  TEST_CLASS(PowerHeuristicTest) {
  public:
    TEST_METHOD(theWeightsOfBothStrategiesSumToOne) {
      struct Case {
        Float samples, pdf, otherSamples, otherPdf;
      };
      for (const auto &c : {Case{16, 0.3f, 4, 2.5f}, Case{1, 100.0f, 16, 0.01f}, Case{4, 1.0f, 4, 1.0f}}) {
        const auto weight = powerHeuristic(c.samples, c.pdf, c.otherSamples, c.otherPdf);
        const auto other = powerHeuristic(c.otherSamples, c.otherPdf, c.samples, c.pdf);
        Assert::AreEqual(1.0f, weight + other, 1e-6f, L"the weights sum to 1", LINE_INFO());
      }
    }

    TEST_METHOD(theWeightIsThePowerHeuristic) {
      // (n p)^2 / ((n p)^2 + (m q)^2): 16 * 0.5 = 8 against 4 * 1 = 4 gives 64 / 80
      Assert::AreEqual(0.8f, powerHeuristic(16, 0.5f, 4, 1.0f), 1e-6f, L"power heuristic", LINE_INFO());
      Assert::AreEqual(0.5f, powerHeuristic(4, 1.0f, 4, 1.0f), 1e-6f, L"equal strategies", LINE_INFO());
    }

    TEST_METHOD(aStrategyAloneGetsTheWholeWeight) {
      Assert::AreEqual(1.0f, powerHeuristic(4, 0.7f, 16, 0.0f), L"the other doesn't sample there", LINE_INFO());
      Assert::AreEqual(1.0f, powerHeuristic(4, 0.7f, 0, 3.0f), L"the other has no samples", LINE_INFO());
    }

    TEST_METHOD(noDensityGetsNoWeight) {
      Assert::AreEqual(0.0f, powerHeuristic(16, 0.0f, 4, 1.0f), L"doesn't sample there", LINE_INFO());
      Assert::AreEqual(0.0f, powerHeuristic(16, 0.0f, 4, 0.0f), L"neither does", LINE_INFO());
    }
  };
}
