#include "raytracing_tests.h"

#include <raytracing/common/noise/SimplexNoiseGenerator.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <utility>

namespace raytracer::test {
  TEST_CLASS(SimplexNoiseGeneratorTest) {
    // The generator with the bit-pattern table of Ken Perlin's reference implementation ("Noise hardware", SIGGRAPH
    // 2002 course notes) instead of a seeded random one, so its values can be compared with the reference.
    class ReferenceSimplexNoiseGenerator final : public SimplexNoiseGenerator {
    public:
      ReferenceSimplexNoiseGenerator() : SimplexNoiseGenerator(0u) {
        static constexpr std::array<Int, N> T{0x15, 0x38, 0x32, 0x2c, 0x0d, 0x13, 0x07, 0x2a};
        std::copy(T.cbegin(), T.cend(), simplexGrid);
      }
    };

    static Float4 gridPoint(const int i) {
      // a lattice that isn't aligned with the simplex grid
      return Float4{0.37f * static_cast<Float>(i % 16), 0.41f * static_cast<Float>((i / 16) % 16),
                    0.43f * static_cast<Float>(i / 256), 1.0f};
    }

  public:
    TEST_METHOD(matchesReferenceImplementation) {
      // expected values computed with the reference implementation (double precision)
      const ReferenceSimplexNoiseGenerator generator{};
      const std::array<std::pair<Float4, Float>, 6> expected{{
          {Float4{0.3f, 0.7f, 0.2f, 1.0f}, 0.133673504f},
          {Float4{1.25f, -0.5f, 2.75f, 1.0f}, -0.0115956308f},
          {Float4{-3.1f, 4.2f, -0.6f, 1.0f}, 0.150845630f},
          {Float4{10.5f, 0.25f, -7.75f, 1.0f}, -0.101813281f},
          {Float4{0.1f, 0.1f, 0.1f, 1.0f}, 0.0857751120f},
          {Float4{5.6f, -2.3f, 8.9f, 1.0f}, 0.0428661143f},
      }};
      for (const auto &[point, value] : expected) {
        Assert::AreEqual(value, generator.noise3(point), 1e-5f, L"reference value", LINE_INFO());
      }
    }

    TEST_METHOD(ignoresTheWComponent) {
      const SimplexNoiseGenerator generator{7u};
      for (int i = 0; i < 64; ++i) {
        const auto point = gridPoint(i * 61);
        Assert::AreEqual(generator.noise3(replaceW(point, 0.0f)), generator.noise3(replaceW(point, 5.0f)), 1e-6f,
                         L"w doesn't change the value", LINE_INFO());
      }
    }

    TEST_METHOD(valuesHaveZeroMeanAndRealVariation) {
      const SimplexNoiseGenerator generator{7u};
      constexpr int count = 4096;
      auto sum = 0.0f, sumOfSquares = 0.0f, maxMagnitude = 0.0f;
      for (int i = 0; i < count; ++i) {
        const auto value = generator.noise3(gridPoint(i));
        sum += value;
        sumOfSquares += value * value;
        maxMagnitude = std::max(maxMagnitude, std::abs(value));
      }
      const auto mean = sum / count;
      const auto deviation = std::sqrt(sumOfSquares / count - mean * mean);
      Assert::IsTrue(std::abs(mean) < 0.02f, L"mean near zero", LINE_INFO());
      Assert::IsTrue(deviation > 0.05f, L"values vary", LINE_INFO());
      Assert::IsTrue(maxMagnitude < 0.5f, L"values in range", LINE_INFO());
    }

    TEST_METHOD(isContinuous) {
      const SimplexNoiseGenerator generator{7u};
      const Float4 direction{0.31f, -0.52f, 0.79f, 0.0f};
      const auto step = 1e-3f;
      auto previous = generator.noise3(Float4{0.2f, 0.3f, 0.4f, 1.0f});
      for (int i = 1; i < 4000; ++i) {
        const auto value = generator.noise3(Float4{0.2f, 0.3f, 0.4f, 1.0f} + direction * (step * static_cast<Float>(i)));
        Assert::IsTrue(std::abs(value - previous) < 0.01f, L"no jumps between neighbours", LINE_INFO());
        previous = value;
      }
    }

    TEST_METHOD(sameSeedSameValuesOtherSeedOtherValues) {
      const SimplexNoiseGenerator a{7u}, b{7u}, c{8u};
      auto differs = false;
      for (int i = 0; i < 256; ++i) {
        const auto point = gridPoint(i * 13);
        Assert::AreEqual(a.noise3(point), b.noise3(point), L"deterministic for a seed", LINE_INFO());
        differs |= a.noise3(point) != c.noise3(point);
      }
      Assert::IsTrue(differs, L"another seed gives other values", LINE_INFO());
    }

    TEST_METHOD(deviationDescribesTheValues) {
      // points of Roberts' R3 sequence in [-8, 8)^3
      const SimplexNoiseGenerator generator{7u};
      constexpr int count = 65536;
      auto sum = 0.0, sumOfSquares = 0.0;
      for (int i = 0; i < count; ++i) {
        const auto t = static_cast<double>(i);
        const Float4 point{static_cast<Float>(16.0 * std::fmod(t * 0.8191725134, 1.0) - 8.0),
                           static_cast<Float>(16.0 * std::fmod(t * 0.6710436067, 1.0) - 8.0),
                           static_cast<Float>(16.0 * std::fmod(t * 0.5497004779, 1.0) - 8.0), 1.0f};
        const auto value = static_cast<double>(generator.noise3(point));
        sum += value;
        sumOfSquares += value * value;
      }
      const auto mean = sum / count;
      const auto measured = static_cast<Float>(std::sqrt(sumOfSquares / count - mean * mean));
      Assert::AreEqual(generator.deviation(), measured, 0.1f * generator.deviation(), L"within 10 %", LINE_INFO());
    }
  };
}
