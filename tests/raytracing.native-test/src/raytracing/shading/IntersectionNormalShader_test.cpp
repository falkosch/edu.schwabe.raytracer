#include "raytracing_tests.h"

#include <raytracing/scene/SceneObject.h>
#include <raytracing/shading/shaders/IntersectionNormalShader.h>

namespace raytracer::test {
  // IntersectionNormalShader formula: (surfaceNormal * 0.5 + 0.5), with w forced to 1.
  // Maps unit-length normals from [-1,1] into [0,1] for visualization. The objectShader
  // argument is unused, but ObjectShader is abstract so SceneObject is the simplest concrete.

  TEST_CLASS(IntersectionNormalShaderTest) {
  public:
    TEST_METHOD(samplesNormalToColorRange) {
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.surfaceNormal = Float4{0.0f, 1.0f, 0.0f, 0.0f};
      const IntersectionNormalShader shader{};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(0.5f, x(color), 1e-6f, L"normal x=0 -> 0.5", LINE_INFO());
      Assert::AreEqual(1.0f, y(color), 1e-6f, L"normal y=1 -> 1.0", LINE_INFO());
      Assert::AreEqual(0.5f, z(color), 1e-6f, L"normal z=0 -> 0.5", LINE_INFO());
      Assert::AreEqual(1.0f, w(color), 1e-6f, L"alpha forced to 1", LINE_INFO());
    }

    TEST_METHOD(negativeNormalMapsToZero) {
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.surfaceNormal = Float4{-1.0f, -1.0f, -1.0f, 0.0f};
      const IntersectionNormalShader shader{};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(0.0f, x(color), 1e-6f, L"-1 -> 0", LINE_INFO());
      Assert::AreEqual(0.0f, y(color), 1e-6f, L"-1 -> 0", LINE_INFO());
      Assert::AreEqual(0.0f, z(color), 1e-6f, L"-1 -> 0", LINE_INFO());
      Assert::AreEqual(1.0f, w(color), 1e-6f, L"alpha forced to 1", LINE_INFO());
    }

    TEST_METHOD(positiveOneNormalMapsToOne) {
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.surfaceNormal = Float4{1.0f, 1.0f, 1.0f, 0.0f};
      const IntersectionNormalShader shader{};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(1.0f, x(color), 1e-6f, L"1 -> 1", LINE_INFO());
      Assert::AreEqual(1.0f, y(color), 1e-6f, L"1 -> 1", LINE_INFO());
      Assert::AreEqual(1.0f, z(color), 1e-6f, L"1 -> 1", LINE_INFO());
    }

    TEST_METHOD(zeroNormalMapsToHalf) {
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.surfaceNormal = Float4{0.0f, 0.0f, 0.0f, 0.0f};
      const IntersectionNormalShader shader{};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(0.5f, x(color), 1e-6f, L"0 -> 0.5", LINE_INFO());
      Assert::AreEqual(0.5f, y(color), 1e-6f, L"0 -> 0.5", LINE_INFO());
      Assert::AreEqual(0.5f, z(color), 1e-6f, L"0 -> 0.5", LINE_INFO());
    }

    TEST_METHOD(operatorMatchesSample) {
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.surfaceNormal = Float4{0.5f, -0.3f, 0.8f, 0.0f};
      const IntersectionNormalShader shader{};

      const auto a = shader.sample(object, intersection);
      const auto b = shader(object, intersection);

      Assert::AreEqual(x(a), x(b), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(y(a), y(b), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(z(a), z(b), 1e-6f, L"z", LINE_INFO());
      Assert::AreEqual(w(a), w(b), 1e-6f, L"w", LINE_INFO());
    }

    TEST_METHOD(arbitraryNormalLinearMapping) {
      // For an arbitrary unit-length normal, each component should map to (n*0.5 + 0.5)
      const Float4 n = normalize3(Float4{0.6f, 0.8f, 0.0f, 0.0f});
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.surfaceNormal = n;
      const IntersectionNormalShader shader{};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(x(n) * 0.5f + 0.5f, x(color), 1e-6f, L"x mapping", LINE_INFO());
      Assert::AreEqual(y(n) * 0.5f + 0.5f, y(color), 1e-6f, L"y mapping", LINE_INFO());
      Assert::AreEqual(z(n) * 0.5f + 0.5f, z(color), 1e-6f, L"z mapping", LINE_INFO());
    }
  };
}
