#include "raytracing_tests.h"

#include <raytracing/SubSampleSpread.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(SubSampleSpreadTest) {
  public:
    TEST_METHOD(equalSubSamplesHaveNoError) {
      auto spread = SubSampleSpread();
      for (auto i = 0; i < 9; ++i) {
        spread.add(Float4(0.3f, 0.2f, 0.1f, 0.0f));
      }
      Assert::AreEqual(0.0f, spread.error(), 1e-6f, L"no spread", LINE_INFO());
    }

    TEST_METHOD(errorIsTheStandardErrorOfTheMean) {
      // display values a and b, alternating over 8 sub-samples: the sample variance (a - b)^2 / 4 * 8 / 7, over 8
      const auto dark = Float4(0.05f, 0.05f, 0.05f, 0.0f);
      const auto bright = Float4(2.0f, 2.0f, 2.0f, 0.0f);
      auto spread = SubSampleSpread();
      for (auto i = 0; i < 8; ++i) {
        spread.add(i % 2 == 0 ? dark : bright);
      }
      const auto a = static_cast<double>(SubSampleSpread::displayValue(dark));
      const auto b = static_cast<double>(SubSampleSpread::displayValue(bright));
      const auto expected = std::sqrt((a - b) * (a - b) / 4.0 * 8.0 / 7.0 / 8.0);
      Assert::AreEqual(expected, static_cast<double>(spread.error()), 1e-5, L"standard error", LINE_INFO());
    }

    TEST_METHOD(oneSubSampleHasNoError) {
      auto spread = SubSampleSpread();
      spread.add(Float4(1.0f, 0.0f, 0.0f, 0.0f));
      Assert::AreEqual(0.0f, spread.error(), L"nothing to judge", LINE_INFO());
    }

    TEST_METHOD(displayValuesCompressTheHighlights) {
      // 0 stays 0, luminance 1 is (1/2)^(1/2.2), and no value reaches 1
      Assert::AreEqual(0.0f, SubSampleSpread::displayValue(Zero<Float4>()), L"black", LINE_INFO());
      Assert::AreEqual(std::pow(0.5f, 1.0f / 2.2f), SubSampleSpread::displayValue(Float4(1.0f, 1.0f, 1.0f, 0.0f)),
                       1e-4f, L"white of luminance 1", LINE_INFO());
      Assert::IsTrue(SubSampleSpread::displayValue(Float4(1e6f, 1e6f, 1e6f, 0.0f)) < 1.0f + 1e-6f, L"bounded",
                     LINE_INFO());
      Assert::IsTrue(SubSampleSpread::displayValue(Float4(0.0f, 1.0f, 0.0f, 0.0f))
                         > SubSampleSpread::displayValue(Float4(0.0f, 0.0f, 1.0f, 0.0f)),
                     L"green is brighter than blue", LINE_INFO());
    }
  };
}
