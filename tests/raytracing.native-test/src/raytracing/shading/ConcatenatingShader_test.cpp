#include "raytracing_tests.h"

#include <raytracing/shading/shaders/ConcatenatingShader.h>
#include <raytracing/shading/shaders/ConstShader.h>
#include <raytracing/shading/shaders/MultiplyByConstShader.h>

#include <functional>

namespace raytracer::test {
  TEST_CLASS(ConcatenatingShaderTest) {
    struct Containment {};

    struct Intersection {};

    using TestShader = Shader<Containment, Intersection, Float4>;
    using TestConstShader = ConstShader<Containment, Intersection, Float4>;

    template <typename Functor>
    using TestConcatenatingShader = ConcatenatingShader<Containment, Intersection, Float4, Functor, TestShader, TestShader>;

  public:
    TEST_METHOD(multipliesTheSamplesOfBothShaders) {
      const TestConstShader left{Float4{0.5f, 2.0f, 1.0f, 1.0f}};
      const TestConstShader right{Float4{0.4f, 0.25f, 3.0f, 1.0f}};
      const TestConcatenatingShader<std::multiplies<Float4>> product{std::multiplies<Float4>{}, left, right};
      const TestShader &shader = product;

      const auto sample = shader.sample(Containment{}, Intersection{});

      Assert::AreEqual(0.2f, x(sample), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(0.5f, y(sample), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(3.0f, z(sample), 1e-6f, L"z", LINE_INFO());
      Assert::IsTrue(&product.getLeft() == &left, L"left operand", LINE_INFO());
      Assert::IsTrue(&product.getRight() == &right, L"right operand", LINE_INFO());
    }

    TEST_METHOD(passesTheLeftSampleFirst) {
      const TestConstShader left{Float4{1.0f, 1.0f, 1.0f, 1.0f}};
      const TestConstShader right{Float4{0.25f, 0.5f, 0.75f, 1.0f}};
      const TestConcatenatingShader<std::minus<Float4>> difference{std::minus<Float4>{}, left, right};

      const auto sample = difference.sample(Containment{}, Intersection{});

      Assert::AreEqual(0.75f, x(sample), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(0.5f, y(sample), 1e-6f, L"y", LINE_INFO());
      Assert::AreEqual(0.25f, z(sample), 1e-6f, L"z", LINE_INFO());
    }

    TEST_METHOD(multiplyByConstShaderScalesTheOtherShader) {
      const TestConstShader other{Float4{0.5f, 1.0f, 2.0f, 1.0f}};
      const MultiplyByConstShader<Containment, Intersection, Float4, TestShader> scaled{Float4{2.0f}, other};

      const auto sample = scaled.sample(Containment{}, Intersection{});

      Assert::AreEqual(1.0f, x(sample), 1e-6f, L"x", LINE_INFO());
      Assert::AreEqual(4.0f, z(sample), 1e-6f, L"z", LINE_INFO());
      Assert::AreEqual(2.0f, x(scaled.getConstant()), L"constant", LINE_INFO());
      Assert::IsTrue(&scaled.getOtherShader() == &other, L"other shader", LINE_INFO());
      Assert::AreEqual(0.5f, x(other.getConstant()), L"const shader's constant", LINE_INFO());
    }
  };
}
