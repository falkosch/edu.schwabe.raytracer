#include "raytracing_tests.h"

#include <raytracing/shading/brdf/ggx.h>
#include <raytracing/shading/sampling.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <set>
#include <vector>

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

    TEST_METHOD(stratifiedIndicesSpreadTheSubSamplesJointly) {
      // the smallest distance on the torus between the points of 9 sub-samples with n samples each
      const auto closest = [](const UInt_32 seed, const ASizeT n, const ASizeT stride) {
        std::vector<Float4> points;
        for (UInt_32 stratum = 0; stratum < 9; ++stratum) {
          for (ASizeT sample = 0; sample < n; ++sample) {
            points.push_back(sequencePoint2D(seed, stratifiedIndex(stratum, stride, sample)));
          }
        }
        auto smallest = 2.0f;
        for (ASizeT i = 0; i < points.size(); ++i) {
          for (ASizeT j = i + 1; j < points.size(); ++j) {
            const auto dx = std::abs(x(points[i]) - x(points[j]));
            const auto dy = std::abs(y(points[i]) - y(points[j]));
            smallest = std::min(smallest, std::hypot(std::min(dx, 1.0f - dx), std::min(dy, 1.0f - dy)));
          }
        }
        return smallest;
      };
      for (const auto seed : {0u, 1u, 7u, 0xdeadbeefu}) {
        // the blocks of 1 or 2 samples keep apart like one sequence
        Assert::IsTrue(closest(seed, 1, 1) > 0.2f, L"1 sample per sub-sample", LINE_INFO());
        Assert::IsTrue(closest(seed, 2, 2) > 0.1f, L"2 samples per sub-sample", LINE_INFO());
        // the most samples (16) as the stride lines the few samples up
        Assert::IsTrue(closest(seed, 1, 16) < 0.2f, L"a fixed stride, 1 sample", LINE_INFO());
        Assert::IsTrue(closest(seed, 2, 16) < 0.1f, L"a fixed stride, 2 samples", LINE_INFO());
      }
    }

    TEST_METHOD(deriveSeedsHashesEachLaneLikeDeriveSeed) {
      for (const auto seed : {0u, 1u, 0x12345678u, 0xffffffffu}) {
        for (const auto first : {0u, 1u, 9u, 0xfffffffcu}) {
          alignas(YMM_ALIGNMENT) UInt_32 lanes[8];
          store(deriveSeeds(seed, first), lanes);
          for (UInt_32 i = 0; i < 8; ++i) {
            Assert::AreEqual(deriveSeed(seed, first + i), lanes[i], L"lane i is deriveSeed(seed, first + i)",
                             LINE_INFO());
          }
        }
      }
    }

    TEST_METHOD(deriveSeedKeepsItsValues) {
      // values of the scalar implementation before the SIMD lanes: the sample patterns of the images stay
      Assert::AreEqual(0xae6f80f1u, deriveSeed(0u, 0u), L"seed 0, value 0", LINE_INFO());
      Assert::AreEqual(0x7f58f66bu, deriveSeed(1u, 1u), L"seed 1, value 1", LINE_INFO());
      Assert::AreEqual(0xa8c4a106u, deriveSeed(0x12345678u, 9u), L"seed 0x12345678, value 9", LINE_INFO());
    }

    TEST_METHOD(sequenceOffsetIsTheScalarHashOfTheSeed) {
      // the upper 24 bits of lowbias32 of the seed (x) and of the seed ^ 0x5bd1e995 (y), from the scalar
      // implementation before the SIMD lanes
      struct Expected {
        UInt_32 seed;
        UInt_32 x;
        UInt_32 y;
      };
      for (const auto &e : {Expected{0u, 0u, 8072180u}, Expected{7u, 9735073u, 15274982u},
                            Expected{0xdeadbeefu, 15083718u, 12088960u}}) {
        const auto offset = sequenceOffset2D(e.seed);
        Assert::AreEqual(static_cast<Float>(e.x) / 16777216.0f, x(offset), L"x", LINE_INFO());
        Assert::AreEqual(static_cast<Float>(e.y) / 16777216.0f, y(offset), L"y", LINE_INFO());
        Assert::AreEqual(0.0f, z(offset), L"z is 0", LINE_INFO());
        Assert::AreEqual(0.0f, w(offset), L"w is 0", LINE_INFO());
        for (ASizeT i = 0; i < 32; ++i) {
          Assert::AreEqual(x(sequencePoint2D(e.seed, i)), x(sequencePoint2D(offset, i)), L"x", LINE_INFO());
          Assert::AreEqual(y(sequencePoint2D(e.seed, i)), y(sequencePoint2D(offset, i)), L"y", LINE_INFO());
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
