#include "raytracing_tests.h"

#include <raytracing/common/RGBS.h>

namespace raytracer::test {
  TEST_CLASS(RGBSTest) {
  public:
    TEST_METHOD(sizeMatchesFloat4) {
      Assert::AreEqual(sizeof(Float4), sizeof(RGBS), L"RGBS must have same size as Float4", LINE_INFO());
    }

    TEST_METHOD(defaultConstructsToZero) {
      const RGBS c{};
      Assert::AreEqual(0.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(0.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(0.0f, z(c.value), L"b", LINE_INFO());
      Assert::AreEqual(0.0f, w(c.value), L"w", LINE_INFO());
    }

    TEST_METHOD(constructsFromComponents) {
      const RGBS c(1.0f, 2.0f, 3.0f);
      Assert::AreEqual(1.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(2.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(3.0f, z(c.value), L"b", LINE_INFO());
      Assert::AreEqual(0.0f, w(c.value), L"w is zero", LINE_INFO());
    }

    TEST_METHOD(implicitConversionFromFloat4) {
      const Float4 v(1.0f, 2.0f, 3.0f, 4.0f);
      const RGBS c = v;
      Assert::AreEqual(1.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(4.0f, w(c.value), L"w", LINE_INFO());
    }

    TEST_METHOD(explicitConversionToFloat4ViaValue) {
      const RGBS c(1.0f, 2.0f, 3.0f);
      const Float4 v = c.value;
      Assert::AreEqual(1.0f, x(v), L"r", LINE_INFO());
      Assert::AreEqual(0.0f, w(v), L"w", LINE_INFO());
    }

    TEST_METHOD(blackIsAllZero) {
      const auto c = RGBS::black();
      Assert::IsTrue(allTrue(c.value == Zero<Float4>()), L"black is zero", LINE_INFO());
    }

    TEST_METHOD(whiteHasRGBOneAndWZero) {
      const auto c = RGBS::white();
      Assert::AreEqual(1.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(1.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(1.0f, z(c.value), L"b", LINE_INFO());
      Assert::AreEqual(0.0f, w(c.value), L"w", LINE_INFO());
    }

    TEST_METHOD(rgbZeroesW) {
      const RGBS c{Float4{1.0f, 2.0f, 3.0f, 4.0f}};
      const Float4 rgb = c.rgb();
      Assert::AreEqual(1.0f, x(rgb), L"r", LINE_INFO());
      Assert::AreEqual(2.0f, y(rgb), L"g", LINE_INFO());
      Assert::AreEqual(3.0f, z(rgb), L"b", LINE_INFO());
      Assert::AreEqual(0.0f, w(rgb), L"w zeroed", LINE_INFO());
    }

    TEST_METHOD(addition) {
      const RGBS a(1.0f, 2.0f, 3.0f);
      const RGBS b(4.0f, 5.0f, 6.0f);
      const RGBS c = a + b;
      Assert::AreEqual(5.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(7.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(9.0f, z(c.value), L"b", LINE_INFO());
      Assert::AreEqual(0.0f, w(c.value), L"w", LINE_INFO());
    }

    TEST_METHOD(subtraction) {
      const RGBS a(5.0f, 7.0f, 9.0f);
      const RGBS b(1.0f, 2.0f, 3.0f);
      const RGBS c = a - b;
      Assert::AreEqual(4.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(5.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(6.0f, z(c.value), L"b", LINE_INFO());
    }

    TEST_METHOD(elementWiseMultiplication) {
      const RGBS a(2.0f, 3.0f, 4.0f);
      const RGBS b(3.0f, 2.0f, 1.0f);
      const RGBS c = a * b;
      Assert::AreEqual(6.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(6.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(4.0f, z(c.value), L"b", LINE_INFO());
    }

    TEST_METHOD(scalarMultiplication) {
      const RGBS a(1.0f, 2.0f, 3.0f);
      const RGBS c = a * 2.0f;
      Assert::AreEqual(2.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(4.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(6.0f, z(c.value), L"b", LINE_INFO());
    }

    TEST_METHOD(scalarMultiplicationCommutative) {
      const RGBS a(1.0f, 2.0f, 3.0f);
      const RGBS c = 2.0f * a;
      Assert::AreEqual(2.0f, x(c.value), L"r", LINE_INFO());
    }

    TEST_METHOD(addAssign) {
      RGBS a(1.0f, 2.0f, 3.0f);
      a += RGBS(4.0f, 5.0f, 6.0f);
      Assert::AreEqual(5.0f, x(a.value), L"r", LINE_INFO());
    }

    TEST_METHOD(subtractAssign) {
      RGBS a(5.0f, 7.0f, 9.0f);
      a -= RGBS(1.0f, 2.0f, 3.0f);
      Assert::AreEqual(4.0f, x(a.value), L"r", LINE_INFO());
    }

    TEST_METHOD(multiplyAssign) {
      RGBS a(2.0f, 3.0f, 4.0f);
      a *= RGBS(3.0f, 2.0f, 1.0f);
      Assert::AreEqual(6.0f, x(a.value), L"r", LINE_INFO());
    }

    TEST_METHOD(scalarMultiplyAssign) {
      RGBS a(1.0f, 2.0f, 3.0f);
      a *= 0.5f;
      Assert::AreEqual(0.5f, x(a.value), L"r", LINE_INFO());
    }

    TEST_METHOD(equalityTrue) {
      const RGBS a(1.0f, 2.0f, 3.0f);
      const RGBS b(1.0f, 2.0f, 3.0f);
      Assert::IsTrue(a == b, L"equal", LINE_INFO());
      Assert::IsFalse(a != b, L"not unequal", LINE_INFO());
    }

    TEST_METHOD(equalityFalse) {
      const RGBS a(1.0f, 2.0f, 3.0f);
      const RGBS b{Float4{1.0f, 2.0f, 3.0f, 5.0f}};
      Assert::IsFalse(a == b, L"not equal", LINE_INFO());
      Assert::IsTrue(a != b, L"unequal", LINE_INFO());
    }

    TEST_METHOD(implicitConversionInBinaryOp) {
      const RGBS a(1.0f, 2.0f, 3.0f);
      const Float4 v(4.0f, 5.0f, 6.0f, 0.0f);
      const RGBS c = a + v;
      Assert::AreEqual(5.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(7.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(9.0f, z(c.value), L"b", LINE_INFO());
    }

    TEST_METHOD(division) {
      const RGBS a(6.0f, 8.0f, 9.0f);
      const RGBS b(2.0f, 4.0f, 3.0f);
      const RGBS c = a / b;
      Assert::AreEqual(3.0f, x(c.value), L"r", LINE_INFO());
      Assert::AreEqual(2.0f, y(c.value), L"g", LINE_INFO());
      Assert::AreEqual(3.0f, z(c.value), L"b", LINE_INFO());
    }

    TEST_METHOD(divideAssign) {
      RGBS a(6.0f, 8.0f, 9.0f);
      a /= RGBS(2.0f, 4.0f, 3.0f);
      Assert::AreEqual(3.0f, x(a.value), L"r", LINE_INFO());
    }
  };
}
