#include "raytracing_tests.h"

#include <raytracing/shading/brdf/ggx.h>
#include <raytracing/shading/sampling.h>

#include <array>
#include <set>

namespace raytracer::test {
  TEST_CLASS(SamplingTest) {
  public:
    TEST_METHOD(deriveSeedIsDeterministicAndSpreads) {
      Assert::AreEqual(deriveSeed(7u, 3u), deriveSeed(7u, 3u), L"deterministic", LINE_INFO());
      std::set<UInt_32> seeds;
      for (UInt_32 pixel = 0; pixel < 64; ++pixel) {
        for (UInt_32 sample = 0; sample < 16; ++sample) {
          seeds.insert(deriveSeed(pixel, sample));
        }
      }
      Assert::AreEqual(size_t{64 * 16}, seeds.size(), L"neighbouring pixels and samples get their own seeds", LINE_INFO());
    }

    TEST_METHOD(sequencePointsStayInTheUnitSquareAndSpreadEvenly) {
      for (const auto seed : {0u, 1u, 0xdeadbeefu}) {
        std::array<int, 4> quadrants{};
        for (ASizeT i = 0; i < 16; ++i) {
          const auto point = sequencePoint2D(seed, i);
          Assert::IsTrue(x(point) >= 0.0f && x(point) < 1.0f, L"x in [0, 1)", LINE_INFO());
          Assert::IsTrue(y(point) >= 0.0f && y(point) < 1.0f, L"y in [0, 1)", LINE_INFO());
          ++quadrants[(x(point) < 0.5f ? 0 : 1) + (y(point) < 0.5f ? 0 : 2)];
        }
        for (const auto count : quadrants) {
          Assert::IsTrue(count >= 2 && count <= 6, L"16 points leave no quadrant empty or crowded", LINE_INFO());
        }
      }
    }

    TEST_METHOD(seedsShiftTheSequence) {
      Assert::AreNotEqual(x(sequencePoint2D(1u, 0)), x(sequencePoint2D(2u, 0)), L"own pattern per seed", LINE_INFO());
    }

    TEST_METHOD(ggxReflectionsWidenWithTheRoughness) {
      // seen from straight above, the reflected directions gather around the normal, the more the smoother; a
      // microfacet tilted by more than 45 degrees reflects below the surface, which counts as 0
      const Float4 N{0.0f, 1.0f, 0.0f, 0.0f};
      const auto meanCosine = [&N](const Float alpha) {
        auto sum = 0.0f;
        for (ASizeT i = 0; i < 64; ++i) {
          const auto L = brdf::sampleGGX(N, N, sequencePoint2D(5u, i), alpha);
          Assert::AreEqual(1.0f, length3(L), 1e-4f, L"a unit direction", LINE_INFO());
          sum += max(dot3(L, N), 0.0f);
        }
        return sum / 64.0f;
      };
      const auto smooth = meanCosine(0.05f);
      const auto rough = meanCosine(0.5f);
      // the GGX tail leaves a mean cosine of about 0.975 for the alpha 0.05
      Assert::IsTrue(smooth > 0.95f, L"a smooth surface reflects nearly straight back", LINE_INFO());
      Assert::IsTrue(rough < smooth - 0.2f, L"a rough surface spreads the reflections", LINE_INFO());
    }
  };
}
