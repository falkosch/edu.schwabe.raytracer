#include "raytracing_tests.h"

#include <raytracing/shading/LightInfo.h>
#include <raytracing/shading/SphereLightCap.h>
#include <raytracing/shading/sampling.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(SphereLightCapTest) {
    // A sphere light of radius 0.5 centered 2 away from the origin, off the axes
    static LightInfo light() {
      LightInfo value{};
      value.position = Float4(1.2f, 1.0f, -1.3f, 1.0f);
      value.radius = 0.5f;
      return value;
    }

  public:
    TEST_METHOD(samplePointsLieOnTheVisibleCap) {
      const auto sphere = light();
      const auto cap = SphereLightCap(sphere, Float4(0.0f, 0.0f, 0.0f, 1.0f));
      for (ASizeT i = 0; i < 256; ++i) {
        const auto toPoint = cap.samplePoint(sequencePoint2D(3u, i));
        const auto normal = (toPoint - (sphere.position - Float4(0.0f, 0.0f, 0.0f, 1.0f))) / sphere.radius;
        Assert::AreEqual(1.0f, length3(normal), 1e-4f, L"on the sphere", LINE_INFO());
        // the cap's rim is where the direction grazes the sphere
        Assert::IsTrue(dot3(normal, toPoint) <= 1e-4f, L"on the side facing the point", LINE_INFO());
      }
    }

    TEST_METHOD(xiZeroIsTheNearestPoint) {
      const auto sphere = light();
      const auto cap = SphereLightCap(sphere, Float4(0.0f, 0.0f, 0.0f, 1.0f));
      const auto toPoint = cap.samplePoint(Zero<Float4>());
      const auto toCenter = sphere.position - Float4(0.0f, 0.0f, 0.0f, 1.0f);
      const auto expected = toCenter - normalize3(toCenter) * sphere.radius;
      Assert::AreEqual(0.0f, length3(toPoint - expected), 1e-5f, L"the point closest to the origin", LINE_INFO());
    }

    TEST_METHOD(solidAnglePdfIntegratesToOne) {
      // over the directions within asin(r / d) of the center, each to the near side of the sphere
      const auto sphere = light();
      const auto cap = SphereLightCap(sphere, Float4(0.0f, 0.0f, 0.0f, 1.0f));
      const auto toCenter = sphere.position - Float4(0.0f, 0.0f, 0.0f, 1.0f);
      const auto maxAngle = std::asin(static_cast<double>(sphere.radius / cap.centerDistance));
      constexpr int Steps = 2000;
      auto integral = 0.0;
      for (auto i = 0; i < Steps; ++i) {
        const auto angle = maxAngle * (i + 0.5) / Steps;
        for (auto j = 0; j < 64; ++j) {
          const auto around = 2.0 * Pi<double>() * (j + 0.5) / 64;
          const auto L = normalize3(
              cap.w * static_cast<Float>(std::cos(angle))
              + (cap.u * static_cast<Float>(std::cos(around)) + cap.v * static_cast<Float>(std::sin(around)))
                    * static_cast<Float>(std::sin(angle))
          );
          const auto along = dot3(toCenter, L);
          const auto squared = sqr(sphere.radius) - (dot3(toCenter, toCenter) - sqr(along));
          const auto distance = along - std::sqrt(std::max(squared, 0.0f));
          integral += static_cast<double>(cap.solidAnglePdf(L * distance)) * std::sin(angle);
        }
      }
      integral *= maxAngle / Steps * 2.0 * Pi<double>() / 64;
      Assert::AreEqual(1.0, integral, 0.01, L"a density", LINE_INFO());
    }

    TEST_METHOD(solidAnglePdfIsTheDensityOfThePoints) {
      // the mean of 1 / pdf over the sampled points is the solid angle they cover, the cap's 2 pi (1 - cos)
      const auto sphere = light();
      const auto cap = SphereLightCap(sphere, Float4(0.0f, 0.0f, 0.0f, 1.0f));
      constexpr ASizeT Samples = 1 << 14;
      auto sum = 0.0;
      for (ASizeT i = 0; i < Samples; ++i) {
        sum += 1.0 / static_cast<double>(cap.solidAnglePdf(cap.samplePoint(sequencePoint2D(9u, i))));
      }
      const auto cosine = std::sqrt(1.0 - sqr(static_cast<double>(sphere.radius / cap.centerDistance)));
      Assert::AreEqual(2.0 * Pi<double>() * (1.0 - cosine), sum / Samples, 0.01 * 2.0 * Pi<double>() * (1.0 - cosine),
                       L"the cap's solid angle", LINE_INFO());
    }

    TEST_METHOD(solidAnglePdfIsZeroOffTheCap) {
      // the sphere's far side
      const auto sphere = light();
      const auto cap = SphereLightCap(sphere, Float4(0.0f, 0.0f, 0.0f, 1.0f));
      const auto toCenter = sphere.position - Float4(0.0f, 0.0f, 0.0f, 1.0f);
      const auto farSide = toCenter + normalize3(toCenter) * sphere.radius;
      Assert::AreEqual(0.0f, cap.solidAnglePdf(farSide), L"not sampled", LINE_INFO());
    }
  };
}
