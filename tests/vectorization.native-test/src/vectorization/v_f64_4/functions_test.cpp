#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_FunctionsTest) {
  public:
    TEST_METHOD(computesMinAndMax) {
      const v_f64_4 a{1.0, 4.0, 2.0, 5.0};
      const v_f64_4 b{3.0, 2.0, 6.0, 1.0};

      const auto mn = min(a, b);
      Assert::AreEqual(1.0, x(mn), L"min X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(mn), L"min Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0, z(mn), L"min Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(mn), L"min W mismatch", LINE_INFO());

      const auto mx = max(a, b);
      Assert::AreEqual(3.0, x(mx), L"max X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(mx), L"max Y mismatch", LINE_INFO());
      Assert::AreEqual(6.0, z(mx), L"max Z mismatch", LINE_INFO());
      Assert::AreEqual(5.0, w(mx), L"max W mismatch", LINE_INFO());

      // scalar reductions
      const v_f64_4 v{3.0, 1.0, 5.0, 2.0};
      Assert::AreEqual(1.0, min(v), L"min(v) mismatch", LINE_INFO());
      Assert::AreEqual(5.0, max(v), L"max(v) mismatch", LINE_INFO());
    }

    TEST_METHOD(computesHorizontalSumAndDot) {
      const v_f64_4 v{1.0, 2.0, 3.0, 4.0};
      Assert::AreEqual(10.0, horizontalSum(v), L"horizontalSum mismatch", LINE_INFO());

      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};
      // dot = 1*5 + 2*6 + 3*7 + 4*8 = 5+12+21+32 = 70
      Assert::AreEqual(70.0, dot(a, b), L"dot mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLengthNormalizeAndDistance) {
      // length({3,0,4,0}) = sqrt(9+0+16+0) = 5
      const v_f64_4 v{3.0, 0.0, 4.0, 0.0};
      Assert::AreEqual(5.0, length(v), 1e-10, L"length mismatch", LINE_INFO());

      // normalize
      const auto n = normalize(v);
      Assert::AreEqual(0.6, x(n), 1e-10, L"normalize X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(n), 1e-10, L"normalize Y mismatch", LINE_INFO());
      Assert::AreEqual(0.8, z(n), 1e-10, L"normalize Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(n), 1e-10, L"normalize W mismatch", LINE_INFO());

      // distance
      const v_f64_4 a{1.0, 2.0, 3.0, 0.0};
      const v_f64_4 b{4.0, 6.0, 3.0, 0.0};
      // distance = length({3,4,0,0}) = sqrt(9+16) = 5
      Assert::AreEqual(5.0, distance(a, b), 1e-10, L"distance mismatch", LINE_INFO());
    }

    TEST_METHOD(computesSqrtReciprocalAndAbs) {
      const v_f64_4 v{4.0, 9.0, 16.0, 25.0};
      const auto s = sqrt(v);
      Assert::AreEqual(2.0, x(s), 1e-10, L"sqrt X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(s), 1e-10, L"sqrt Y mismatch", LINE_INFO());
      Assert::AreEqual(4.0, z(s), 1e-10, L"sqrt Z mismatch", LINE_INFO());
      Assert::AreEqual(5.0, w(s), 1e-10, L"sqrt W mismatch", LINE_INFO());

      const auto rec = reciprocal(v_f64_4{2.0, 4.0, 5.0, 10.0});
      Assert::AreEqual(0.5, x(rec), 1e-10, L"reciprocal X mismatch", LINE_INFO());
      Assert::AreEqual(0.25, y(rec), 1e-10, L"reciprocal Y mismatch", LINE_INFO());
      Assert::AreEqual(0.2, z(rec), 1e-10, L"reciprocal Z mismatch", LINE_INFO());
      Assert::AreEqual(0.1, w(rec), 1e-10, L"reciprocal W mismatch", LINE_INFO());

      const v_f64_4 neg{-3.0, 7.0, -5.0, -1.0};
      const auto a = abs(neg);
      Assert::AreEqual(3.0, x(a), L"abs X mismatch", LINE_INFO());
      Assert::AreEqual(7.0, y(a), L"abs Y mismatch", LINE_INFO());
      Assert::AreEqual(5.0, z(a), L"abs Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(a), L"abs W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesClampAndMix) {
      const v_f64_4 v{-1.0, 5.0, 2.0, 10.0};
      const v_f64_4 lower{0.0, 0.0, 0.0, 0.0};
      const v_f64_4 upper{3.0, 3.0, 3.0, 3.0};
      const auto clamped = clamp(v, lower, upper);
      Assert::AreEqual(0.0, x(clamped), L"clamp X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(clamped), L"clamp Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0, z(clamped), L"clamp Z mismatch", LINE_INFO());
      Assert::AreEqual(3.0, w(clamped), L"clamp W mismatch", LINE_INFO());

      // mix(a, b, 0.5) = midpoint
      const v_f64_4 a{0.0, 2.0, 4.0, 6.0};
      const v_f64_4 b{4.0, 6.0, 8.0, 10.0};
      const auto mid = mix(a, b, 0.5);
      Assert::AreEqual(2.0, x(mid), 1e-10, L"mix X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(mid), 1e-10, L"mix Y mismatch", LINE_INFO());
      Assert::AreEqual(6.0, z(mid), 1e-10, L"mix Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(mid), 1e-10, L"mix W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesIsNegativeAndPredicates) {
      const v_f64_4 v{-1.0, 2.0, -3.0, 4.0};
      const auto mask = isNegative(v);
      // X negative (bit 0) and Z negative (bit 2)
      Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::X)) != 0, L"isNegative X", LINE_INFO());
      Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::Y)) == 0, L"isNegative Y", LINE_INFO());
      Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::Z)) != 0, L"isNegative Z", LINE_INFO());
      Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::W)) == 0, L"isNegative W", LINE_INFO());

      const v_f64_4 allOnes{1.0, 1.0, 1.0, 1.0};
      const auto cmpResult = allOnes == allOnes;
      Assert::IsTrue(allTrue(cmpResult), L"allTrue should be true for equal vectors", LINE_INFO());
      Assert::IsFalse(anyFalse(cmpResult), L"anyFalse should be false for equal vectors", LINE_INFO());
    }

    TEST_METHOD(computesCeilFloorRoundFract) {
      const v_f64_4 v{1.3, 2.7, -0.3, 3.5};

      const auto c = ceil(v);
      Assert::AreEqual(2.0, x(c), L"ceil X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(c), L"ceil Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0, z(c), L"ceil Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(c), L"ceil W mismatch", LINE_INFO());

      const auto f = floor(v);
      Assert::AreEqual(1.0, x(f), L"floor X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(f), L"floor Y mismatch", LINE_INFO());
      Assert::AreEqual(-1.0, z(f), L"floor Z mismatch", LINE_INFO());
      Assert::AreEqual(3.0, w(f), L"floor W mismatch", LINE_INFO());

      // fract = v - floor(v)
      const auto fr = fract(v_f64_4{1.75, 2.25, 3.5, 4.125});
      Assert::AreEqual(0.75, x(fr), 1e-10, L"fract X mismatch", LINE_INFO());
      Assert::AreEqual(0.25, y(fr), 1e-10, L"fract Y mismatch", LINE_INFO());
      Assert::AreEqual(0.5, z(fr), 1e-10, L"fract Z mismatch", LINE_INFO());
      Assert::AreEqual(0.125, w(fr), 1e-10, L"fract W mismatch", LINE_INFO());
    }
  };
}
