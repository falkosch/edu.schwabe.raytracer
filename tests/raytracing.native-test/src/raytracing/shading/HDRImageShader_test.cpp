#include "raytracing_tests.h"

#include <raytracing/scene/SceneObject.h>
#include <raytracing/shading/shaders/HDRImageShader.h>

namespace raytracer::test {
  // HDRImageShader::operator()(objectShader, intersection) returns
  //   image->sampleBilinear(intersection.texCoords)
  // The objectShader argument is unused.

  TEST_CLASS(HDRImageShaderTest) {
  public:
    TEST_METHOD(singlePixelImagePassesValueThrough) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{0.25f, 0.5f, 0.75f, 1.0f};
      const HDRImageShader shader{image};
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.texCoords = Float4{0.0f, 0.0f, 0.0f, 0.0f};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(0.25f, x(color), 1e-6f, L"r", LINE_INFO());
      Assert::AreEqual(0.5f, y(color), 1e-6f, L"g", LINE_INFO());
      Assert::AreEqual(0.75f, z(color), 1e-6f, L"b", LINE_INFO());
      Assert::AreEqual(1.0f, w(color), 1e-6f, L"a", LINE_INFO());
    }

    TEST_METHOD(samplesDistinctTexelsFromTwoByTwoImage) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{1.0f, 0.0f, 0.0f, 1.0f};  // top-left
      image[1] = Float4{0.0f, 1.0f, 0.0f, 1.0f};  // top-right
      image[2] = Float4{0.0f, 0.0f, 1.0f, 1.0f};  // bottom-left
      image[3] = Float4{1.0f, 1.0f, 0.0f, 1.0f};  // bottom-right
      const HDRImageShader shader{image};
      const SceneObject object{"test"};
      FacetIntersection intersection{};

      // Texel-center samples land on integer floor indices (no interpolation).
      intersection.texCoords = Float4{0.0f, 0.0f, 0.0f, 0.0f};
      auto color = shader.sample(object, intersection);
      Assert::AreEqual(1.0f, x(color), 1e-6f, L"(0,0) -> data[0] r", LINE_INFO());

      intersection.texCoords = Float4{0.5f, 0.0f, 0.0f, 0.0f};
      color = shader.sample(object, intersection);
      Assert::AreEqual(1.0f, y(color), 1e-6f, L"(0.5,0) -> data[1] g", LINE_INFO());

      intersection.texCoords = Float4{0.0f, 0.5f, 0.0f, 0.0f};
      color = shader.sample(object, intersection);
      Assert::AreEqual(1.0f, z(color), 1e-6f, L"(0,0.5) -> data[2] b", LINE_INFO());

      intersection.texCoords = Float4{0.5f, 0.5f, 0.0f, 0.0f};
      color = shader.sample(object, intersection);
      Assert::AreEqual(1.0f, x(color), 1e-6f, L"(0.5,0.5) -> data[3] r", LINE_INFO());
      Assert::AreEqual(1.0f, y(color), 1e-6f, L"(0.5,0.5) -> data[3] g", LINE_INFO());
    }

    TEST_METHOD(preservesHDRRangeValues) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{4.5f, -0.25f, 100.0f, 1.0f};
      const HDRImageShader shader{image};
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.texCoords = Float4{0.0f, 0.0f, 0.0f, 0.0f};

      const auto color = shader.sample(object, intersection);

      Assert::AreEqual(4.5f, x(color), 1e-6f, L"value > 1 preserved", LINE_INFO());
      Assert::AreEqual(-0.25f, y(color), 1e-6f, L"negative preserved", LINE_INFO());
      Assert::AreEqual(100.0f, z(color), 1e-6f, L"large value preserved", LINE_INFO());
    }

    TEST_METHOD(operatorMatchesSample) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{0.1f, 0.2f, 0.3f, 1.0f};
      image[1] = Float4{0.4f, 0.5f, 0.6f, 1.0f};
      image[2] = Float4{0.7f, 0.8f, 0.9f, 1.0f};
      image[3] = Float4{1.0f, 0.0f, 0.0f, 1.0f};
      const HDRImageShader shader{image};
      const SceneObject object{"test"};
      FacetIntersection intersection{};
      intersection.texCoords = Float4{0.25f, 0.25f, 0.0f, 0.0f};

      const auto a = shader.sample(object, intersection);
      const auto b = shader(object, intersection);

      Assert::AreEqual(x(a), x(b), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(y(a), y(b), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(z(a), z(b), 1e-6f, L"z", LINE_INFO());
      Assert::AreEqual(w(a), w(b), 1e-6f, L"w", LINE_INFO());
    }

    TEST_METHOD(objectShaderArgumentIsUnused) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{0.42f, 0.42f, 0.42f, 1.0f};
      const HDRImageShader shader{image};
      const SceneObject objectA{"a"};
      const SceneObject objectB{"b"};
      FacetIntersection intersection{};
      intersection.texCoords = Float4{0.3f, 0.7f, 0.0f, 0.0f};

      const auto colorA = shader.sample(objectA, intersection);
      const auto colorB = shader.sample(objectB, intersection);

      Assert::AreEqual(x(colorA), x(colorB), 1e-6f, L"r same", LINE_INFO());
      Assert::AreEqual(y(colorA), y(colorB), 1e-6f, L"g same", LINE_INFO());
      Assert::AreEqual(z(colorA), z(colorB), 1e-6f, L"b same", LINE_INFO());
    }
  };
}
