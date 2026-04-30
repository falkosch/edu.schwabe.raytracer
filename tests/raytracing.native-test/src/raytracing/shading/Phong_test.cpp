#include "raytracing_tests.h"

namespace raytracer::test {
  // Phong specular intensity reproduced from raytracing/shading/SceneShader.cpp:
  //   pow3(max(dot(L, R), 0), shininess)
  // The production helper is private; these tests lock in the formula via public primitives
  // (max, dotv, pow3, reflect).

  static Float4 phongSpecular(const Float4 &reflectedIncident, const Float4 &lightDirection, const Float4 &shininess) {
    return pow3(max(dotv(lightDirection, reflectedIncident), Zero<Float4>()), shininess);
  }

  TEST_CLASS(PhongTest) {
  public:
    TEST_METHOD(alignedReflectionGivesMaximum) {
      // Light direction == reflected incident => dot = 1 => pow(1,n) = 1
      const Float4 lightDir{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 shininess{32.0f, 32.0f, 32.0f, 32.0f};
      const auto s = phongSpecular(reflected, lightDir, shininess);

      Assert::AreEqual(1.0f, x(s), 1e-5f, L"aligned should give max 1.0", LINE_INFO());
    }

    TEST_METHOD(perpendicularGivesZero) {
      // Light perpendicular to reflected => dot = 0 => 0^n = 0
      const Float4 lightDir{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 shininess{32.0f, 32.0f, 32.0f, 32.0f};
      const auto s = phongSpecular(reflected, lightDir, shininess);

      Assert::AreEqual(0.0f, x(s), 1e-5f, L"perpendicular should give 0", LINE_INFO());
    }

    TEST_METHOD(oppositeDirectionClampedToZero) {
      // Light opposite to reflected => dot = -1, max(-1, 0) = 0 => 0
      const Float4 lightDir{0.0f, -1.0f, 0.0f, 0.0f};
      const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 shininess{32.0f, 32.0f, 32.0f, 32.0f};
      const auto s = phongSpecular(reflected, lightDir, shininess);

      Assert::AreEqual(0.0f, x(s), 1e-5f, L"opposite clamped to zero", LINE_INFO());
    }

    TEST_METHOD(higherShininessNarrowsLobe) {
      // 60-degree off => dot = 0.5; higher shininess => smaller intensity
      const Float4 lightDir = normalize3(Float4{0.866f, 0.5f, 0.0f, 0.0f});
      const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
      // dot ~ 0.5
      const auto sLow = phongSpecular(reflected, lightDir, Float4{2.0f, 2.0f, 2.0f, 2.0f});
      const auto sMid = phongSpecular(reflected, lightDir, Float4{32.0f, 32.0f, 32.0f, 32.0f});
      const auto sHigh = phongSpecular(reflected, lightDir, Float4{128.0f, 128.0f, 128.0f, 128.0f});

      Assert::IsTrue(x(sLow) > x(sMid), L"low exponent has wider lobe", LINE_INFO());
      Assert::IsTrue(x(sMid) > x(sHigh), L"mid > high exponent", LINE_INFO());
    }

    TEST_METHOD(intensityNonNegative) {
      // Sweep angles; specular must never be negative
      for (Float angle = -1.0f; angle <= 1.0f; angle += 0.1f) {
        const Float4 lightDir = normalize3(Float4{angle, 1.0f, 0.0f, 0.0f});
        const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
        const auto s = phongSpecular(reflected, lightDir, Float4{16.0f, 16.0f, 16.0f, 16.0f});
        Assert::IsTrue(x(s) >= 0.0f, L"specular always non-negative", LINE_INFO());
      }
    }

    TEST_METHOD(intensityInUnitRangeForUnitVectors) {
      // Unit-vector dot products land in [-1,1], clamped to [0,1] => pow stays in [0,1]
      for (Float angle = 0.0f; angle <= 1.0f; angle += 0.1f) {
        const Float4 lightDir = normalize3(Float4{angle, 1.0f, 0.0f, 0.0f});
        const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
        const auto s = phongSpecular(reflected, lightDir, Float4{16.0f, 16.0f, 16.0f, 16.0f});
        Assert::IsTrue(x(s) >= 0.0f && x(s) <= 1.0f + 1e-5f, L"specular in [0,1]", LINE_INFO());
      }
    }

    TEST_METHOD(reflectedDirectionFromIncidentIsConsistent) {
      // For a light along +y and an incident -y, reflected becomes +y, so light.dot(reflected) = 1
      const Float4 incident{0.0f, -1.0f, 0.0f, 0.0f};
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto reflected = reflect(incident, normal);
      const Float4 lightDir{0.0f, 1.0f, 0.0f, 0.0f};
      const auto s = phongSpecular(reflected, lightDir, Float4{32.0f, 32.0f, 32.0f, 32.0f});

      Assert::AreEqual(1.0f, x(s), 1e-5f, L"perfect mirror direction gives max", LINE_INFO());
    }

    TEST_METHOD(lowShininessIsBroad) {
      // shininess = 1 => dot itself
      const Float4 lightDir = normalize3(Float4{1.0f, 1.0f, 0.0f, 0.0f});
      const Float4 reflected{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 shininess{1.0f, 1.0f, 1.0f, 1.0f};
      const auto s = phongSpecular(reflected, lightDir, shininess);

      // dot = sqrt(2)/2 ~ 0.707
      Assert::AreEqual(0.7071f, x(s), 0.001f, L"shininess=1 returns dot value", LINE_INFO());
    }
  };
}
