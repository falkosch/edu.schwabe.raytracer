#include "raytracing_tests.h"

#include <raytracing/shading/shaders/environment_coordinates.h>

#include <cmath>
#include <string>

namespace raytracer::test {
  TEST_CLASS(EnvironmentCoordinatesTest) {
    static constexpr Float Tolerance = 1e-5f;

    static void assertCoordinates(
        const EnvironmentProjection projection, const Float4 &direction, const Float u, const Float v,
        const wchar_t *message
    ) {
      const auto coordinates = environmentCoordinates(projection, normalize3(direction));
      Assert::AreEqual(u, x(coordinates), Tolerance, (std::wstring(message) + L": u").c_str(), LINE_INFO());
      Assert::AreEqual(v, y(coordinates), Tolerance, (std::wstring(message) + L": v").c_str(), LINE_INFO());
    }

    static Float radiusFromCenter(const Float4 &coordinates) {
      return std::sqrt(sqr(x(coordinates) - 0.5f) + sqr(y(coordinates) - 0.5f));
    }

  public:
    TEST_METHOD(fisheyeDomeUsesXAndZ) {
      constexpr auto dome = EnvironmentProjection::FisheyeDome;
      assertCoordinates(dome, Float4{0.0f, 1.0f, 0.0f, 0.0f}, 0.5f, 0.5f, L"zenith in the center");
      assertCoordinates(dome, Float4{1.0f, 0.0f, 0.0f, 0.0f}, 1.0f, 0.5f, L"+x on the horizon");
      assertCoordinates(dome, Float4{0.0f, 0.0f, -1.0f, 0.0f}, 0.5f, 0.0f, L"-z on the horizon");
      assertCoordinates(dome, Float4{0.0f, -1.0f, 0.0f, 0.0f}, 0.5f, 0.5f, L"nadir mirrors the zenith");
    }

    TEST_METHOD(mirrorBallCenterIsTheViewDirection) {
      constexpr auto ball = EnvironmentProjection::MirrorBall;
      const auto sine45 = 0.5f * std::sqrt(0.5f);
      assertCoordinates(ball, Float4{0.0f, 0.0f, -1.0f, 0.0f}, 0.5f, 0.5f, L"-z in the center");
      // a side direction is 90 degrees off: sin(45 degrees) of the ball's radius 0.5
      assertCoordinates(ball, Float4{0.0f, 1.0f, 0.0f, 0.0f}, 0.5f, 0.5f + sine45, L"up above the center");
      assertCoordinates(ball, Float4{0.0f, -1.0f, 0.0f, 0.0f}, 0.5f, 0.5f - sine45, L"down below the center");
      assertCoordinates(ball, Float4{1.0f, 0.0f, 0.0f, 0.0f}, 0.5f - sine45, 0.5f, L"+x mirrored to the left");
    }

    TEST_METHOD(mirrorBallRimIsBehindTheCamera) {
      const auto nearlyBack = environmentCoordinates(
          EnvironmentProjection::MirrorBall, normalize3(Float4{1e-3f, 0.0f, 1.0f, 0.0f})
      );
      Assert::AreEqual(0.5f, radiusFromCenter(nearlyBack), 1e-3f, L"+z on the rim", LINE_INFO());
      for (int i = 0; i < 100; ++i) {
        const auto angle = 0.0628f * static_cast<Float>(i);
        const Float4 direction{std::cos(angle) * 0.6f, std::sin(angle) * 0.6f, (i % 2 ? 0.8f : -0.8f), 0.0f};
        const auto coordinates = environmentCoordinates(EnvironmentProjection::MirrorBall, direction);
        Assert::IsTrue(radiusFromCenter(coordinates) <= 0.5f + Tolerance, L"inside the ball", LINE_INFO());
      }
    }

    TEST_METHOD(angularMapRadiusIsLinearInTheAngle) {
      constexpr auto map = EnvironmentProjection::AngularMap;
      assertCoordinates(map, Float4{0.0f, 0.0f, -1.0f, 0.0f}, 0.5f, 0.5f, L"-z in the center");
      assertCoordinates(map, Float4{0.0f, 1.0f, 0.0f, 0.0f}, 0.5f, 0.75f, L"up at half the radius");
      assertCoordinates(map, Float4{0.0f, -1.0f, 0.0f, 0.0f}, 0.5f, 0.25f, L"down at half the radius");
      assertCoordinates(map, Float4{1.0f, 0.0f, 0.0f, 0.0f}, 0.25f, 0.5f, L"+x mirrored to the left");
      // 45 degrees up from -z: a quarter of the half radius
      assertCoordinates(map, Float4{0.0f, 1.0f, -1.0f, 0.0f}, 0.5f, 0.625f, L"45 degrees up");
      const auto nearlyBack = environmentCoordinates(map, normalize3(Float4{0.0f, 1e-3f, 1.0f, 0.0f}));
      Assert::AreEqual(0.5f, radiusFromCenter(nearlyBack), 1e-3f, L"+z on the rim", LINE_INFO());
    }

    TEST_METHOD(latitudeLongitudeCoversAzimuthAndElevation) {
      constexpr auto map = EnvironmentProjection::LatitudeLongitude;
      assertCoordinates(map, Float4{0.0f, 0.0f, -1.0f, 0.0f}, 0.5f, 0.5f, L"-z in the center");
      assertCoordinates(map, Float4{1.0f, 0.0f, 0.0f, 0.0f}, 0.75f, 0.5f, L"+x to the right");
      assertCoordinates(map, Float4{-1.0f, 0.0f, 0.0f, 0.0f}, 0.25f, 0.5f, L"-x to the left");
      // the poles have no azimuth: any u is right
      Assert::AreEqual(1.0f, y(environmentCoordinates(map, Float4{0.0f, 1.0f, 0.0f, 0.0f})), Tolerance,
                       L"zenith on the top row", LINE_INFO());
      Assert::AreEqual(0.0f, y(environmentCoordinates(map, Float4{0.0f, -1.0f, 0.0f, 0.0f})), Tolerance,
                       L"nadir on the bottom row", LINE_INFO());
      assertCoordinates(map, Float4{1.0f, 1.0f, 0.0f, 0.0f}, 0.75f, 0.75f, L"45 degrees up");
    }

    TEST_METHOD(latitudeLongitudeSeamIsBehindTheCamera) {
      const auto right = environmentCoordinates(
          EnvironmentProjection::LatitudeLongitude, normalize3(Float4{1e-3f, 0.0f, 1.0f, 0.0f})
      );
      const auto left = environmentCoordinates(
          EnvironmentProjection::LatitudeLongitude, normalize3(Float4{-1e-3f, 0.0f, 1.0f, 0.0f})
      );
      Assert::AreEqual(1.0f, x(right), 1e-3f, L"right of the seam at u = 1", LINE_INFO());
      Assert::AreEqual(0.0f, x(left), 1e-3f, L"left of the seam at u = 0", LINE_INFO());
    }
  };
}
