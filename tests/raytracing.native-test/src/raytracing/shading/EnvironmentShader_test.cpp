#include "raytracing_tests.h"

#include <raytracing/shading/SceneShader.h>
#include <raytracing/shading/shaders/EnvironmentShader.h>

namespace raytracer::test {
  // EnvironmentShader::operator()(sceneShader, rayDirection) returns
  //   image->sampleBilinear(0.5 * (rayDirection.x, rayDirection.z, ...) + 0.5)
  // The mapping squashes [-1, 1] -> [0, 1] on x and z; y is ignored.
  // sampleBilinear wraps modulo resolution at edges.

  // SceneShader is abstract (buildSceneGraph is pure virtual). The shader's operator() does
  // not use the sceneShader argument, but we need a concrete instance to pass.
  class StubSceneShader final : public SceneShader {
  public:
    void buildSceneGraph() override {}
  };

  TEST_CLASS(EnvironmentShaderTest) {
  public:
    TEST_METHOD(singlePixelImagePassesValueThrough) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{0.25f, 0.5f, 0.75f, 1.0f};
      const EnvironmentShader shader{image};
      const StubSceneShader sceneShader{};

      const auto color = shader.sample(sceneShader, Float4{0.0f, 1.0f, 0.0f, 0.0f});

      Assert::AreEqual(0.25f, x(color.value), 1e-6f, L"r", LINE_INFO());
      Assert::AreEqual(0.5f, y(color.value), 1e-6f, L"g", LINE_INFO());
      Assert::AreEqual(0.75f, z(color.value), 1e-6f, L"b", LINE_INFO());
    }

    TEST_METHOD(rayDirectionXZSelectsTexel) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{1.0f, 0.0f, 0.0f, 1.0f};  // (u=0, v=0)
      image[1] = Float4{0.0f, 1.0f, 0.0f, 1.0f};  // (u=0.5, v=0)
      image[2] = Float4{0.0f, 0.0f, 1.0f, 1.0f};  // (u=0, v=0.5)
      image[3] = Float4{1.0f, 1.0f, 0.0f, 1.0f};  // (u=0.5, v=0.5)
      const EnvironmentShader shader{image};
      const StubSceneShader sceneShader{};

      // direction (-1, *, -1) -> texCoords (0, 0) -> data[0]
      auto color = shader.sample(sceneShader, Float4{-1.0f, 0.0f, -1.0f, 0.0f});
      Assert::AreEqual(1.0f, x(color.value), 1e-6f, L"(-1,*,-1) -> data[0] r", LINE_INFO());

      // direction (0, *, -1) -> texCoords (0.5, 0) -> data[1]
      color = shader.sample(sceneShader, Float4{0.0f, 0.0f, -1.0f, 0.0f});
      Assert::AreEqual(1.0f, y(color.value), 1e-6f, L"(0,*,-1) -> data[1] g", LINE_INFO());

      // direction (-1, *, 0) -> texCoords (0, 0.5) -> data[2]
      color = shader.sample(sceneShader, Float4{-1.0f, 0.0f, 0.0f, 0.0f});
      Assert::AreEqual(1.0f, z(color.value), 1e-6f, L"(-1,*,0) -> data[2] b", LINE_INFO());

      // direction (0, *, 0) -> texCoords (0.5, 0.5) -> data[3]
      color = shader.sample(sceneShader, Float4{0.0f, 0.0f, 0.0f, 0.0f});
      Assert::AreEqual(1.0f, x(color.value), 1e-6f, L"(0,*,0) -> data[3] r", LINE_INFO());
      Assert::AreEqual(1.0f, y(color.value), 1e-6f, L"(0,*,0) -> data[3] g", LINE_INFO());
    }

    TEST_METHOD(yComponentIsIgnored) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{0.1f, 0.2f, 0.3f, 1.0f};
      image[1] = Float4{0.4f, 0.5f, 0.6f, 1.0f};
      image[2] = Float4{0.7f, 0.8f, 0.9f, 1.0f};
      image[3] = Float4{1.0f, 0.0f, 0.0f, 1.0f};
      const EnvironmentShader shader{image};
      const StubSceneShader sceneShader{};

      const auto a = shader.sample(sceneShader, Float4{0.3f, -1.0f, 0.7f, 0.0f});
      const auto b = shader.sample(sceneShader, Float4{0.3f, 1.0f, 0.7f, 0.0f});
      const auto c = shader.sample(sceneShader, Float4{0.3f, 100.0f, 0.7f, 0.0f});

      Assert::AreEqual(x(a.value), x(b.value), 1e-6f, L"y=-1 vs y=1 r", LINE_INFO());
      Assert::AreEqual(y(a.value), y(b.value), 1e-6f, L"y=-1 vs y=1 g", LINE_INFO());
      Assert::AreEqual(z(a.value), z(b.value), 1e-6f, L"y=-1 vs y=1 b", LINE_INFO());
      Assert::AreEqual(x(a.value), x(c.value), 1e-6f, L"y=-1 vs y=100 r", LINE_INFO());
    }

    TEST_METHOD(preservesHDRRangeValues) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{8.0f, -0.5f, 250.0f, 1.0f};
      const EnvironmentShader shader{image};
      const StubSceneShader sceneShader{};

      const auto color = shader.sample(sceneShader, Float4{0.0f, 0.0f, 0.0f, 0.0f});

      Assert::AreEqual(8.0f, x(color.value), 1e-6f, L"value > 1 preserved", LINE_INFO());
      Assert::AreEqual(-0.5f, y(color.value), 1e-6f, L"negative preserved", LINE_INFO());
      Assert::AreEqual(250.0f, z(color.value), 1e-6f, L"large value preserved", LINE_INFO());
    }

    TEST_METHOD(operatorMatchesSample) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{0.1f, 0.2f, 0.3f, 1.0f};
      image[1] = Float4{0.4f, 0.5f, 0.6f, 1.0f};
      image[2] = Float4{0.7f, 0.8f, 0.9f, 1.0f};
      image[3] = Float4{1.0f, 0.0f, 0.0f, 1.0f};
      const EnvironmentShader shader{image};
      const StubSceneShader sceneShader{};
      const Float4 direction{-0.4f, 0.6f, 0.2f, 0.0f};

      const auto a = shader.sample(sceneShader, direction);
      const auto b = shader(sceneShader, direction);

      Assert::AreEqual(x(a.value), x(b.value), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(y(a.value), y(b.value), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(z(a.value), z(b.value), 1e-6f, L"z", LINE_INFO());
      Assert::AreEqual(w(a.value), w(b.value), 1e-6f, L"w", LINE_INFO());
    }

    TEST_METHOD(extremeRayDirectionWrapsAtEdges) {
      // sampleBilinear uses `% resolution` on uv, so texCoord 1.0 wraps to 0.
      // direction (1, *, 1) -> texCoords (1, 1) -> wraps -> data[0].
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{0.9f, 0.0f, 0.0f, 1.0f};
      image[1] = Float4{0.0f, 0.9f, 0.0f, 1.0f};
      image[2] = Float4{0.0f, 0.0f, 0.9f, 1.0f};
      image[3] = Float4{0.0f, 0.0f, 0.0f, 1.0f};
      const EnvironmentShader shader{image};
      const StubSceneShader sceneShader{};

      const auto color = shader.sample(sceneShader, Float4{1.0f, 0.0f, 1.0f, 0.0f});

      Assert::AreEqual(0.9f, x(color.value), 1e-6f, L"+x +z wraps to data[0] r", LINE_INFO());
      Assert::AreEqual(0.0f, y(color.value), 1e-6f, L"+x +z wraps to data[0] g", LINE_INFO());
    }
  };
}
