#include "raytracing_tests.h"

#include <raytracing/common/noise/PerlinNoiseGenerator.h>

#include <algorithm>
#include <cmath>

namespace raytracer::test {
  TEST_CLASS(PerlinNoiseGeneratorTest) {
  public:
    TEST_METHOD(isZeroAtTheLatticePoints) {
      // gradient noise vanishes at the corners of its cells, also at negative ones and in every dimension count
      const PerlinNoiseGenerator generator{7u};
      for (const auto &point : {Float4{1.0f, 2.0f, 3.0f, 4.0f}, Float4{-3.0f, 0.0f, -7.0f, -2.0f}}) {
        Assert::AreEqual(0.0f, generator.noise(x(point)), 1e-6f, L"1D", LINE_INFO());
        Assert::AreEqual(0.0f, generator.noise2(point), 1e-6f, L"2D", LINE_INFO());
        Assert::AreEqual(0.0f, generator.noise3(point), 1e-6f, L"3D", LINE_INFO());
        Assert::AreEqual(0.0f, generator.noise4(point), 1e-6f, L"4D", LINE_INFO());
      }
    }

    TEST_METHOD(negativeCoordinatesDontRepeatTheCellAboveZero) {
      const PerlinNoiseGenerator generator{7u};
      auto differs = false;
      for (int i = 1; i < 10; ++i) {
        const auto t = 0.1f * static_cast<Float>(i);
        differs |= generator.noise3(Float4{t - 1.0f, 0.4f, 0.6f, 1.0f}) != generator.noise3(Float4{t, 0.4f, 0.6f, 1.0f});
      }
      Assert::IsTrue(differs, L"the cell [-1, 0] has its own gradients", LINE_INFO());
    }

    TEST_METHOD(isContinuousAcrossZero) {
      const PerlinNoiseGenerator generator{7u};
      const auto below = generator.noise3(Float4{-1e-4f, 0.4f, 0.6f, 1.0f});
      const auto above = generator.noise3(Float4{1e-4f, 0.4f, 0.6f, 1.0f});
      Assert::AreEqual(below, above, 1e-3f, L"no jump at zero", LINE_INFO());
    }

    TEST_METHOD(noise4VariesAlongW) {
      const PerlinNoiseGenerator generator{7u};
      const Float4 point{0.3f, 0.6f, 0.2f, 0.0f};
      auto previous = generator.noise4(point);
      auto differs = false;
      for (int i = 1; i <= 100; ++i) {
        const auto value = generator.noise4(replaceW(point, 0.01f * static_cast<Float>(i)));
        Assert::IsTrue(std::abs(value - previous) < 0.05f, L"continuous along w", LINE_INFO());
        differs |= std::abs(value - generator.noise4(point)) > 0.01f;
        previous = value;
      }
      Assert::IsTrue(differs, L"w is a noise dimension", LINE_INFO());
    }

    TEST_METHOD(valuesHaveZeroMeanAndRealVariation) {
      const PerlinNoiseGenerator generator{7u};
      constexpr int count = 4096;
      auto sum = 0.0f, sumOfSquares = 0.0f, maxMagnitude = 0.0f;
      for (int i = 0; i < count; ++i) {
        const Float4 point{0.37f * static_cast<Float>(i % 16) - 3.0f, 0.41f * static_cast<Float>((i / 16) % 16) - 3.0f,
                           0.43f * static_cast<Float>(i / 256) - 3.0f, 1.0f};
        const auto value = generator.noise3(point);
        sum += value;
        sumOfSquares += value * value;
        maxMagnitude = std::max(maxMagnitude, std::abs(value));
      }
      const auto mean = sum / count;
      const auto deviation = std::sqrt(sumOfSquares / count - mean * mean);
      Assert::IsTrue(std::abs(mean) < 0.03f, L"mean near zero", LINE_INFO());
      Assert::IsTrue(deviation > 0.05f, L"values vary", LINE_INFO());
      Assert::IsTrue(maxMagnitude <= 1.0f, L"values in range", LINE_INFO());
    }
  };
}
