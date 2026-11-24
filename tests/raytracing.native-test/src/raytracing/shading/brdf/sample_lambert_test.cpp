#include "raytracing_tests.h"

#include <raytracing/shading/brdf/sample_lambert.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(SampleLambertTest) {
    // The directions of an n x n grid of cell centers in the unit square around N; calls visit(direction) for each
    template <typename Visit>
    static void forGrid(const Float4 &N, const int n, Visit visit) {
      for (auto i = 0; i < n; ++i) {
        for (auto j = 0; j < n; ++j) {
          const auto xi = Float4((static_cast<Float>(i) + 0.5f) / static_cast<Float>(n),
                                 (static_cast<Float>(j) + 0.5f) / static_cast<Float>(n), 0.0f, 0.0f);
          visit(brdf::sampleLambert(N, xi));
        }
      }
    }

  public:
    TEST_METHOD(directionsAreUnitVectorsAboveTheSurface) {
      for (const auto &N : {normalize3(Float4(1.0f, 2.0f, 3.0f, 0.0f)), Float4(0.0f, 1.0f, 0.0f, 0.0f),
                            Float4(0.0f, -1.0f, 0.0f, 0.0f)}) {
        forGrid(N, 32, [&](const Float4 &direction) {
          Assert::AreEqual(1.0f, length3(direction), 1e-5f, L"unit length", LINE_INFO());
          Assert::IsTrue(dot3(N, direction) >= 0.0f, L"in the hemisphere of N", LINE_INFO());
          Assert::AreEqual(0.0f, w(direction), L"a direction", LINE_INFO());
        });
      }
    }

    TEST_METHOD(centerOfTheSquareIsTheNormal) {
      const auto N = normalize3(Float4(-1.0f, 0.5f, 2.0f, 0.0f));
      const auto direction = brdf::sampleLambert(N, Float4(0.5f, 0.5f, 0.0f, 0.0f));
      Assert::AreEqual(1.0f, dot3(N, direction), 1e-6f, L"straight up", LINE_INFO());
    }

    TEST_METHOD(cosinesFollowLambertsDistribution) {
      // with the density cos / pi, the mean cosine is 2/3 and the mean squared cosine 1/2; a cone of the half angle
      // theta holds sin^2 theta of the directions (n: the grid's cells inside 45 degrees, 140 x 140 of 198 x 198, are
      // half of them, and none lies close to the edge)
      const auto N = normalize3(Float4(0.3f, -0.4f, 0.5f, 0.0f));
      constexpr auto n = 198;
      auto cosineSum = 0.0;
      auto squaredSum = 0.0;
      auto withinCone = 0;
      forGrid(N, n, [&](const Float4 &direction) {
        const auto cosine = static_cast<double>(dot3(N, direction));
        cosineSum += cosine;
        squaredSum += cosine * cosine;
        withinCone += cosine > std::cos(3.14159265358979 / 4.0) ? 1 : 0;
      });
      const auto count = static_cast<double>(n * n);
      Assert::AreEqual(2.0 / 3.0, cosineSum / count, 2e-3, L"mean cosine", LINE_INFO());
      Assert::AreEqual(0.5, squaredSum / count, 1e-3, L"mean squared cosine", LINE_INFO());
      Assert::AreEqual(0.5, withinCone / count, 1e-3, L"half within 45 degrees", LINE_INFO());
    }

    TEST_METHOD(azimuthsSpreadEvenly) {
      // the tangent frame's quadrants get a quarter each
      const auto N = Float4(0.0f, 0.0f, 1.0f, 0.0f);
      int quadrants[4]{};
      constexpr auto n = 64;
      forGrid(N, n, [&](const Float4 &direction) {
        const auto index = (x(direction) >= 0.0f ? 0 : 1) + (y(direction) >= 0.0f ? 0 : 2);
        ++quadrants[index];
      });
      for (const auto count : quadrants) {
        Assert::AreEqual(n * n / 4, count, L"a quarter per quadrant", LINE_INFO());
      }
    }
  };
}
