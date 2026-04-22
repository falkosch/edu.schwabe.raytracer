#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(RayTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const Ray ray{};
      Assert::IsTrue(allTrue(ray.origin == Zero<Float4>()), L"origin should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(ray.direction == Zero<Float4>()), L"direction should be zero", LINE_INFO());
      Assert::IsTrue(
          allTrue(ray.reciprocalDirection == Zero<Float4>()), L"reciprocalDirection should be zero", LINE_INFO()
      );
    }

    TEST_METHOD(constructsFromOrigin) {
      const Float4 origin{1.0f, 2.0f, 3.0f, 0.0f};
      const Ray ray{origin};
      Assert::IsTrue(allTrue(ray.origin == origin), L"origin should match", LINE_INFO());
      Assert::IsTrue(allTrue(ray.direction == Zero<Float4>()), L"direction should be zero", LINE_INFO());
      Assert::IsTrue(
          allTrue(ray.reciprocalDirection == Zero<Float4>()), L"reciprocalDirection should be zero", LINE_INFO()
      );
    }

    TEST_METHOD(constructsFromOriginAndDirection) {
      const Float4 origin{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 direction{1.0f, 0.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      Assert::IsTrue(allTrue(ray.origin == origin), L"origin should match", LINE_INFO());
      Assert::IsTrue(allTrue(ray.direction == direction), L"direction should match", LINE_INFO());
      Assert::AreEqual(1.0f, x(ray.reciprocalDirection), L"reciprocal x should be 1", LINE_INFO());
    }

    TEST_METHOD(setsDirection) {
      Ray ray{};
      const Float4 direction{0.0f, 1.0f, 0.0f, 0.0f};
      ray.setDirection(direction);
      Assert::IsTrue(allTrue(ray.direction == direction), L"direction should be updated", LINE_INFO());
      Assert::AreEqual(1.0f, y(ray.reciprocalDirection), L"reciprocal y should be 1", LINE_INFO());
    }

    TEST_METHOD(replacesDirection) {
      const Float4 origin{1.0f, 2.0f, 3.0f, 0.0f};
      const Float4 direction{1.0f, 0.0f, 0.0f, 0.0f};
      const Ray original{origin, direction};

      const Float4 newDirection{0.0f, 1.0f, 0.0f, 0.0f};
      const Ray replaced = replaceDirection(original, newDirection);

      Assert::IsTrue(allTrue(replaced.origin == origin), L"origin should be preserved", LINE_INFO());
      Assert::IsTrue(allTrue(replaced.direction == newDirection), L"direction should be new value", LINE_INFO());
      Assert::IsTrue(
          allTrue(original.direction == direction), L"original direction should be unchanged", LINE_INFO()
      );
    }

    TEST_METHOD(computesPointAlongRayScalar) {
      const Float4 origin{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 direction{0.0f, 1.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      const Float4 expected{1.0f, 2.0f, 0.0f, 0.0f};
      const Float4 actual = point(ray, 2.0f);
      Assert::IsTrue(allTrue(actual == expected), L"point should be origin + direction * t", LINE_INFO());
    }

    TEST_METHOD(computesPointAlongRayVector) {
      const Float4 origin{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 direction{0.0f, 1.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      const Float4 t{2.0f, 2.0f, 2.0f, 2.0f};
      const Float4 expected{1.0f, 2.0f, 0.0f, 0.0f};
      const Float4 actual = point(ray, t);
      Assert::IsTrue(allTrue(actual == expected), L"point should be origin + direction * value", LINE_INFO());
    }
  };
}
