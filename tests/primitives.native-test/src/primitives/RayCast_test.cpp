#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(RayCastTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const RayCast rayCast{};
      Assert::AreEqual(std::numeric_limits<Float>::max(), rayCast.maxDistance, L"maxDistance should be FLT_MAX", LINE_INFO());
    }

    TEST_METHOD(constructsWithParameters) {
      const Float4 origin{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 direction{0.0f, 1.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      const auto cullingMask = Zero<Float4::VectorBoolType>();
      const Size2 originIds{10, 20};
      const Float maxDist = 100.0f;

      const RayCast rayCast{ray, cullingMask, originIds, maxDist};

      Assert::IsTrue(allTrue(rayCast.ray.origin == origin), L"origin should match", LINE_INFO());
      Assert::IsTrue(allTrue(rayCast.ray.direction == direction), L"direction should match", LINE_INFO());
      Assert::AreEqual(maxDist, rayCast.maxDistance, L"maxDistance should match", LINE_INFO());
    }

    TEST_METHOD(detectsBackfaceCulling) {
      const Ray ray{};
      const auto cullingMask = _mm_castps_si128(MaskX<Float4::PackedType>());
      const RayCast rayCast{ray, cullingMask, Zero<Size2>(), std::numeric_limits<Float>::max()};

      Assert::IsTrue(backfaceCulled(rayCast), L"should detect backface culling", LINE_INFO());
      Assert::IsFalse(frontfaceCulled(rayCast), L"should not detect frontface culling", LINE_INFO());
    }

    TEST_METHOD(detectsFrontfaceCulling) {
      const Ray ray{};
      const auto cullingMask = _mm_castps_si128(MaskY<Float4::PackedType>());
      const RayCast rayCast{ray, cullingMask, Zero<Size2>(), std::numeric_limits<Float>::max()};

      Assert::IsTrue(frontfaceCulled(rayCast), L"should detect frontface culling", LINE_INFO());
      Assert::IsFalse(backfaceCulled(rayCast), L"should not detect backface culling", LINE_INFO());
    }

    TEST_METHOD(detectsNotCulled) {
      const Ray ray{};
      const auto cullingMask = Zero<Float4::VectorBoolType>();
      const RayCast rayCast{ray, cullingMask, Zero<Size2>(), std::numeric_limits<Float>::max()};

      Assert::IsTrue(notCulled(rayCast), L"should detect no culling", LINE_INFO());
    }

    TEST_METHOD(detectsSelfOcclusion) {
      const Ray ray{};
      const Size2 originIds{10, 20};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), originIds, std::numeric_limits<Float>::max()};

      Assert::IsTrue(selfOcclusion(rayCast, 20), L"should detect self occlusion when y matches", LINE_INFO());
    }

    TEST_METHOD(detectsNotSelfOcclusion) {
      const Ray ray{};
      const Size2 originIds{10, 20};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), originIds, std::numeric_limits<Float>::max()};

      Assert::IsFalse(selfOcclusion(rayCast, 99), L"should not detect self occlusion when y differs", LINE_INFO());
    }

    TEST_METHOD(detectsOutOfReach) {
      const Ray ray{};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Zero<Size2>(), 10.0f};

      Assert::IsTrue(outOfReach(rayCast, 10.0f), L"t == maxDistance should be out of reach", LINE_INFO());
      Assert::IsTrue(outOfReach(rayCast, 15.0f), L"t > maxDistance should be out of reach", LINE_INFO());
    }

    TEST_METHOD(detectsInReach) {
      const Ray ray{};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Zero<Size2>(), 10.0f};

      Assert::IsFalse(outOfReach(rayCast, 5.0f), L"t < maxDistance should be in reach", LINE_INFO());
    }

    TEST_METHOD(detectsOutOfReachPerLane) {
      // The Float4 and Float8 overloads decide alike per lane, NaN distances and a NaN maxDistance included.
      const auto nan = std::numeric_limits<Float>::quiet_NaN();
      const Ray ray{};
      const Float8 distances{5.0f, 10.0f, 15.0f, nan, -1.0f, 9.99f, 10.01f, 0.0f};
      for (const auto maxDistance : {10.0f, nan}) {
        const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Zero<Size2>(), maxDistance};
        const auto lanes = isNegative(outOfReach(rayCast, distances));
        const auto lo = isNegative(outOfReach(rayCast, Float4{distances[0], distances[1], distances[2], distances[3]}));
        const auto hi = isNegative(outOfReach(rayCast, Float4{distances[4], distances[5], distances[6], distances[7]}));
        // isNegative of a v_i32_4 repeats its 4 lane bits in every nibble
        Assert::AreEqual((lo & 0xF) | ((hi & 0xF) << 4), lanes, L"Float8 lanes match the Float4 overload", LINE_INFO());
      }
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Zero<Size2>(), 10.0f};
      Assert::AreEqual(0b0100'1110, isNegative(outOfReach(rayCast, distances)), L"out of reach lanes", LINE_INFO());
    }

    TEST_METHOD(computesFarPoint) {
      const Float4 origin{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 direction{0.0f, 1.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Zero<Size2>(), 5.0f};

      const Float4 expected{1.0f, 5.0f, 0.0f, 0.0f};
      const Float4 actual = farPoint(rayCast);
      Assert::IsTrue(allTrue(actual == expected), L"farPoint should be origin + direction * maxDistance", LINE_INFO());
    }
  };
}
