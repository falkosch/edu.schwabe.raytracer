#include "raytracing_tests.h"

#include <raytracing/shading/BoxLightFaces.h>
#include <raytracing/shading/LightInfo.h>
#include <raytracing/shading/sampling.h>

#include <array>
#include <cmath>

namespace raytracer::test {
  TEST_CLASS(BoxLightFacesTest) {
    using Vector = std::array<double, 3>;

    static Vector toVector(const Float4 &v) {
      return {x(v), y(v), z(v)};
    }

    static double dot(const Vector &a, const Vector &b) {
      return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    // The irradiance per radiance at a point under the corner of a parallel rectangle (width x depth) at the height:
    // pi times the form factor of a differential area to the rectangle (Howell's catalog of configuration factors)
    static double cornerIrradiance(const double width, const double depth, const double height) {
      const auto X = width / height;
      const auto Y = depth / height;
      const auto sx = std::sqrt(1.0 + X * X);
      const auto sy = std::sqrt(1.0 + Y * Y);
      return 0.5 * (X / sx * std::atan(Y / sx) + Y / sy * std::atan(X / sy));
    }

    // A rectangle light in the plane y = y(center) of the half sizes in x and z, emitting from both sides
    static LightInfo rectangle(const Float4 &center, const Float halfWidth, const Float halfDepth) {
      LightInfo light;
      light.position = center;
      light.halfExtents = Float4(halfWidth, 0.0f, halfDepth, 0.0f);
      return light;
    }

    // A box light of the half extents around the center, rotated by the angles in degrees about z, y and x
    static LightInfo rotatedBox(const Float4 &center, const Float4 &halfExtents, const Float4 &degrees) {
      LightInfo light;
      light.position = center;
      light.halfExtents = halfExtents;
      light.orientation = rotate(
          rotate(rotate(Identity<MFloat4x4>(), z(degrees), OneZ<Float4>()), y(degrees), OneY<Float4>()), x(degrees),
          OneX<Float4>()
      );
      return light;
    }

    // The irradiance per radiance at the origin from the light's emitting faces that face it, by the midpoint rule over
    // n x n cells per face: the cosines at both ends over the squared distance, nothing below the horizon
    static double integratedIrradiance(const LightInfo &light, const Float4 &normal, const int n) {
      const Vector axes[3] = {
          toVector(light.orientation * OneX<Float4>()), toVector(light.orientation * OneY<Float4>()),
          toVector(light.orientation * OneZ<Float4>())
      };
      const auto halfExtents = toVector(light.halfExtents);
      const auto position = toVector(light.position);
      const auto N = toVector(normal);
      auto sum = 0.0;
      for (auto axis = 0; axis < 3; ++axis) {
        for (const auto side : {-1.0, 1.0}) {
          const auto u = (axis + 1) % 3;
          const auto v = (axis + 2) % 3;
          Vector faceNormal{}, center{};
          for (auto i = 0; i < 3; ++i) {
            faceNormal[i] = side * axes[axis][i];
            center[i] = position[i] + faceNormal[i] * halfExtents[axis];
          }
          if (!(light.emittingFaces & LightInfo::faceBit(axis, side > 0.0)) || dot(center, faceNormal) >= 0.0) {
            continue;
          }
          const auto cellArea = 4.0 * halfExtents[u] * halfExtents[v] / (static_cast<double>(n) * n);
          for (auto i = 0; i < n; ++i) {
            const auto s = halfExtents[u] * (-1.0 + (2.0 * i + 1.0) / n);
            for (auto j = 0; j < n; ++j) {
              const auto t = halfExtents[v] * (-1.0 + (2.0 * j + 1.0) / n);
              Vector point{};
              for (auto k = 0; k < 3; ++k) {
                point[k] = center[k] + axes[u][k] * s + axes[v][k] * t;
              }
              const auto distanceSquared = dot(point, point);
              const auto distance = std::sqrt(distanceSquared);
              const auto cosine = dot(N, point) / distance;
              if (cosine > 0.0) {
                sum += cosine * -dot(faceNormal, point) / distance / distanceSquared * cellArea;
              }
            }
          }
        }
      }
      return sum;
    }

    static void
    assertRelative(const double expected, const double actual, const double tolerance, const wchar_t *message) {
      Assert::AreEqual(expected, actual, tolerance * expected, message, LINE_INFO());
    }

    static const Float4 &up() {
      static const Float4 value(0.0f, 1.0f, 0.0f, 0.0f);
      return value;
    }

  public:
    TEST_METHOD(squareAboveMatchesTheFormFactor) {
      const BoxLightFaces faces(rectangle(Float4(0.0f, 1.0f, 0.0f, 1.0f), 0.5f, 0.5f), Float4(0, 0, 0, 1), up());
      Assert::AreEqual(ASizeT{1}, faces.faceCount, L"only the lower side faces the point", LINE_INFO());
      assertRelative(4.0 * cornerIrradiance(0.5, 0.5, 1.0), faces.projectedSolidAngle, 1e-5, L"four corners");
      assertRelative(1.0, faces.area, 1e-6, L"the whole square");
    }

    TEST_METHOD(rectangleUnderItsCornerMatchesTheFormFactor) {
      const BoxLightFaces faces(rectangle(Float4(0.4f, 0.7f, 0.25f, 1.0f), 0.4f, 0.25f), Float4(0, 0, 0, 1), up());
      assertRelative(cornerIrradiance(0.8, 0.5, 0.7), faces.projectedSolidAngle, 1e-5, L"the point under a corner");
    }

    TEST_METHOD(formFactorReachesItsLimits) {
      const BoxLightFaces small(rectangle(Float4(0.0f, 1.0f, 0.0f, 1.0f), 0.005f, 0.005f), Float4(0, 0, 0, 1), up());
      assertRelative(1e-4, small.projectedSolidAngle, 1e-3, L"a small light: its area over the squared distance");
      const BoxLightFaces huge(rectangle(Float4(0.0f, 1.0f, 0.0f, 1.0f), 1e3f, 1e3f), Float4(0, 0, 0, 1), up());
      assertRelative(Pi<Float>(), huge.projectedSolidAngle, 1e-3, L"a huge light covers the hemisphere: pi");
    }

    TEST_METHOD(horizonClipsTheLight) {
      // the horizon x = -0.1 y crosses the light at y = 1
      const auto light = rectangle(Float4(0.3f, 1.0f, 0.2f, 1.0f), 0.5f, 0.4f);
      const auto normal = normalize3(Float4(1.0f, 0.1f, 0.0f, 0.0f));
      const BoxLightFaces faces(light, Float4(0, 0, 0, 1), normal);
      Assert::AreEqual(ASizeT{4}, faces.faces[0].vertexCount, L"the horizon cuts off one side", LINE_INFO());
      assertRelative(0.9 * 0.8, faces.area, 1e-5, L"the part above the horizon");
      assertRelative(integratedIrradiance(light, normal, 1000), faces.projectedSolidAngle, 1e-3, L"integrated");
    }

    TEST_METHOD(horizonCutsACornerOff) {
      // the horizon x + z = -0.1 y cuts the corner at x = -0.2, z = -0.2
      const auto light = rectangle(Float4(0.3f, 1.0f, 0.2f, 1.0f), 0.5f, 0.4f);
      const auto normal = normalize3(Float4(1.0f, 0.1f, 1.0f, 0.0f));
      const BoxLightFaces faces(light, Float4(0, 0, 0, 1), normal);
      Assert::AreEqual(ASizeT{5}, faces.faces[0].vertexCount, L"a pentagon", LINE_INFO());
      assertRelative(integratedIrradiance(light, normal, 1000), faces.projectedSolidAngle, 1e-3, L"integrated");
    }

    TEST_METHOD(rotatedBoxShowsThreeFaces) {
      const auto light = rotatedBox(
          Float4(1.0f, 1.5f, -1.0f, 1.0f), Float4(0.2f, 0.1f, 0.3f, 0.0f), Float4(30.0f, 40.0f, 10.0f, 0.0f)
      );
      const auto normal = normalize3(Float4(0.2f, 1.0f, 0.1f, 0.0f));
      const BoxLightFaces faces(light, Float4(0, 0, 0, 1), normal);
      Assert::AreEqual(ASizeT{3}, faces.faceCount, L"a face along each axis", LINE_INFO());
      assertRelative(integratedIrradiance(light, normal, 1000), faces.projectedSolidAngle, 1e-3, L"integrated");
    }

    TEST_METHOD(onlyEmittingFacesLight) {
      // a ceiling light 1 cm thick that emits from its bottom face only, and a point beside it within its thickness
      auto light = rotatedBox(Float4(0.0f, 0.994f, 0.0f, 1.0f), Float4(0.25f, 0.005f, 0.2f, 0.0f), Zero<Float4>());
      light.emittingFaces = LightInfo::faceBit(1, false);
      const auto down = Float4(0.0f, -1.0f, 0.0f, 0.0f);
      const BoxLightFaces ceiling(light, Float4(0.5f, 0.995f, 0.0f, 1.0f), down);
      Assert::AreEqual(ASizeT{0}, ceiling.faceCount, L"the ceiling beside it sees only a dark side face", LINE_INFO());

      const BoxLightFaces floor(light, Float4(0.0f, -1.0f, 0.0f, 1.0f), up());
      Assert::AreEqual(ASizeT{1}, floor.faceCount, L"the floor sees the bottom face", LINE_INFO());
      assertRelative(4.0 * cornerIrradiance(0.25, 0.2, 1.989), floor.projectedSolidAngle, 1e-4, L"the bottom face");

      light.emittingFaces = LightInfo::AllFaces;
      const BoxLightFaces glowing(light, Float4(0.5f, 0.995f, 0.0f, 1.0f), down);
      Assert::AreEqual(ASizeT{1}, glowing.faceCount, L"all faces emitting: the side face", LINE_INFO());
      Assert::IsTrue(glowing.projectedSolidAngle > 0.0f, L"lights the ceiling", LINE_INFO());
    }

    TEST_METHOD(samplePointsSpreadEvenlyOverTheArea) {
      const auto light = rotatedBox(
          Float4(1.0f, 1.5f, -1.0f, 1.0f), Float4(0.2f, 0.1f, 0.3f, 0.0f), Float4(30.0f, 40.0f, 10.0f, 0.0f)
      );
      const auto normal = normalize3(Float4(0.2f, 1.0f, 0.1f, 0.0f));
      const BoxLightFaces faces(light, Float4(0, 0, 0, 1), normal);

      // the irradiance estimated from area-uniform points
      constexpr ASizeT Samples = 4096;
      auto sum = 0.0;
      for (ASizeT i = 0; i < Samples; ++i) {
        ASizeT face = 99;
        const auto point = faces.samplePoint(sequencePoint2D(0u, i), face);
        Assert::IsTrue(face < faces.faceCount, L"a visible face", LINE_INFO());
        const auto &f = faces.faces[face];
        const auto offset = point - f.center;
        Assert::AreEqual(0.0f, dot3(offset, f.normal), 1e-5f, L"in the face's plane", LINE_INFO());
        Assert::IsTrue(
            vectorization::abs(dot3(offset, f.tangentU)) <= dot3(f.tangentU, f.tangentU) * 1.0001f
                && vectorization::abs(dot3(offset, f.tangentV)) <= dot3(f.tangentV, f.tangentV) * 1.0001f,
            L"on the face", LINE_INFO()
        );
        Assert::IsTrue(dot3(point, normal) >= -1e-6f, L"above the horizon", LINE_INFO());
        const auto distanceSquared = static_cast<double>(dot3(point, point));
        const auto distance = std::sqrt(distanceSquared);
        sum += dot3(normal, point) * -dot3(f.normal, point) / (distance * distance * distanceSquared);
      }
      assertRelative(faces.projectedSolidAngle, sum * faces.area / Samples, 1e-2, L"the mean matches the irradiance");
    }

    TEST_METHOD(nearestPointIsTheClosestPointOfTheFaces) {
      const auto light = rectangle(Float4(0.0f, 2.0f, 0.0f, 1.0f), 0.5f, 0.4f);
      ASizeT face = 99;
      const BoxLightFaces below(light, Float4(0.1f, 0.0f, 0.2f, 1.0f), up());
      const auto straightUp = below.nearestPoint(face);
      Assert::AreEqual(ASizeT{0}, face, L"the only face", LINE_INFO());
      Assert::AreEqual(0.0f, length3(straightUp - Float4(0.0f, 2.0f, 0.0f, 0.0f)), 1e-6f, L"right above", LINE_INFO());

      const BoxLightFaces beside(light, Float4(2.0f, 0.0f, -1.0f, 1.0f), up());
      const auto corner = beside.nearestPoint(face);
      Assert::AreEqual(0.0f, length3(corner - Float4(-1.5f, 2.0f, 0.6f, 0.0f)), 1e-6f, L"the corner", LINE_INFO());
    }

    TEST_METHOD(representativePointFollowsTheReflection) {
      const auto light = rectangle(Float4(0.0f, 2.0f, 0.0f, 1.0f), 0.5f, 0.4f);
      const BoxLightFaces faces(light, Float4(0, 0, 0, 1), up());
      const auto hit = faces.representativePoint(0, normalize3(Float4(0.1f, 1.0f, 0.05f, 0.0f)));
      Assert::AreEqual(
          0.0f, length3(hit - Float4(0.2f, 2.0f, 0.1f, 0.0f)), 1e-5f, L"where the ray meets it", LINE_INFO()
      );
      const auto edge = faces.representativePoint(0, normalize3(Float4(1.0f, 1.0f, 0.0f, 0.0f)));
      Assert::AreEqual(
          0.0f, length3(edge - Float4(0.5f, 2.0f, 0.0f, 0.0f)), 1e-5f, L"clamped to its edge", LINE_INFO()
      );
      const auto away = faces.representativePoint(0, normalize3(Float4(1.0f, -0.2f, 0.0f, 0.0f)));
      Assert::AreEqual(
          0.0f, length3(away - Float4(0.5f, 2.0f, 0.0f, 0.0f)), 1e-5f, L"the edge it heads for", LINE_INFO()
      );
    }
  };
}
