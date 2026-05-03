#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_ConstantsTest) {
  public:
    TEST_METHOD(zeroIsAllZero) {
      const auto v = Zero<v_f32_8>();
      Assert::AreEqual(0.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(oneIsAllOne) {
      const auto v = One<v_f32_8>();
      Assert::AreEqual(1.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(1.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(twoIsAllTwo) {
      const auto v = Two<v_f32_8>();
      Assert::AreEqual(2.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(halfIsAllHalf) {
      const auto v = Half<v_f32_8>();
      Assert::AreEqual(0.5f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(0.5f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(oneHalfIsAllOneHalf) {
      const auto v = OneHalf<v_f32_8>();
      Assert::AreEqual(OneHalf<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(OneHalf<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(negativeOneIsAllNegativeOne) {
      const auto v = NegativeOne<v_f32_8>();
      Assert::AreEqual(-1.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(-1.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(negativeTwoIsAllNegativeTwo) {
      const auto v = NegativeTwo<v_f32_8>();
      Assert::AreEqual(-2.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(-2.0f, x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(-2.0f, x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(-2.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(negativeZeroIsSignedZero) {
      const auto v = NegativeZero<v_f32_8>();
      // NegativeZero compares equal to positive zero
      Assert::AreEqual(0.0f, x1(v), L"x1 value", LINE_INFO());
      Assert::AreEqual(0.0f, x8(v), L"x8 value", LINE_INFO());
      // But the sign bit is set
      Assert::IsTrue(std::signbit(x1(v)), L"x1 signbit", LINE_INFO());
      Assert::IsTrue(std::signbit(x4(v)), L"x4 signbit", LINE_INFO());
      Assert::IsTrue(std::signbit(x5(v)), L"x5 signbit", LINE_INFO());
      Assert::IsTrue(std::signbit(x8(v)), L"x8 signbit", LINE_INFO());
    }

    TEST_METHOD(negativeInfinityIsBroadcast) {
      const auto v = NegativeInfinity<v_f32_8>();
      Assert::AreEqual(-std::numeric_limits<float>::infinity(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(-std::numeric_limits<float>::infinity(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(-std::numeric_limits<float>::infinity(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(-std::numeric_limits<float>::infinity(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(piIsBroadcast) {
      const auto v = Pi<v_f32_8>();
      Assert::AreEqual(x(Pi<PackedFloat4_128>()), x1(v), L"pi x1", LINE_INFO());
      Assert::AreEqual(x(Pi<PackedFloat4_128>()), x8(v), L"pi x8", LINE_INFO());
    }

    TEST_METHOD(reciprocalPiIsBroadcast) {
      const auto v = ReciprocalPi<v_f32_8>();
      Assert::AreEqual(ReciprocalPi<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(ReciprocalPi<v_f32_8::ValueType>(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(ReciprocalPi<v_f32_8::ValueType>(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(ReciprocalPi<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(degreeToRadianIsBroadcast) {
      const auto v = DegreeToRadian<v_f32_8>();
      Assert::AreEqual(DegreeToRadian<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(DegreeToRadian<v_f32_8::ValueType>(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(DegreeToRadian<v_f32_8::ValueType>(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(DegreeToRadian<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(radianToDegreeIsBroadcast) {
      const auto v = RadianToDegree<v_f32_8>();
      Assert::AreEqual(RadianToDegree<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(RadianToDegree<v_f32_8::ValueType>(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(RadianToDegree<v_f32_8::ValueType>(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(RadianToDegree<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(radianToUniformIsBroadcast) {
      const auto v = RadianToUniform<v_f32_8>();
      Assert::AreEqual(RadianToUniform<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(RadianToUniform<v_f32_8::ValueType>(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(RadianToUniform<v_f32_8::ValueType>(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(RadianToUniform<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(selfOcclusionEpsilonIsBroadcast) {
      const auto v = SelfOcclusionEpsilon<v_f32_8>();
      Assert::AreEqual(SelfOcclusionEpsilon<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(SelfOcclusionEpsilon<v_f32_8::ValueType>(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(SelfOcclusionEpsilon<v_f32_8::ValueType>(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(SelfOcclusionEpsilon<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(sin45IsBroadcast) {
      const auto v = Sin45<v_f32_8>();
      Assert::AreEqual(Sin45<v_f32_8::ValueType>(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(Sin45<v_f32_8::ValueType>(), x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(Sin45<v_f32_8::ValueType>(), x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(Sin45<v_f32_8::ValueType>(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(epsilonIsBroadcast) {
      const auto v = Epsilon<v_f32_8>();
      Assert::IsTrue(x1(v) > 0.0f, L"positive", LINE_INFO());
      Assert::AreEqual(x1(v), x8(v), L"uniform", LINE_INFO());
    }

    TEST_METHOD(infinityIsBroadcast) {
      const auto v = Infinity<v_f32_8>();
      Assert::AreEqual(std::numeric_limits<float>::infinity(), x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<float>::infinity(), x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(notANumberIsNaN) {
      const auto v = NotANumber<v_f32_8>();
      Assert::IsTrue(std::isnan(x1(v)), L"x1 NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(x8(v)), L"x8 NaN", LINE_INFO());
    }

    TEST_METHOD(maskAllIsAllBits) {
      const auto v = MaskAll<v_f32_8>();
      Assert::IsTrue(std::isnan(x1(v)), L"all bits set is NaN", LINE_INFO());
    }

    TEST_METHOD(maskNoneIsZero) {
      const auto v = MaskNone<v_f32_8>();
      Assert::AreEqual(0.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(v), L"x8", LINE_INFO());
    }
  };
}
