#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_FunctionsTrigonometryTest) {
  public:
    TEST_METHOD(computesLog) {
      const auto test = StandardSample::transcendentalFloatConstants<v_f32_4::ValueType>();

      auto matchers = {Compare::nearEqualsRelative<v_f32_4>, // Half
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::equalsOrdered<v_f32_4>, // One
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // OneHalf
                       Compare::isBothNotANumber<v_f32_4>,

                       Compare::nearEqualsRelative<v_f32_4>, // Two
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // Pi
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // ReciprocalPi
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // DegreeToRadian
                       Compare::isBothNotANumber<v_f32_4>,

                       Compare::nearEqualsRelative<v_f32_4>, // RadianToDegree
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // RadianToUniform
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // Sin45
                       Compare::isBothNotANumber<v_f32_4>,

                       Compare::nearEqualsRelative<v_f32_4>, // Epsilon
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // SelfOcclusionEpsilon
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // min
                       Compare::isBothNotANumber<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // epsilon
                       Compare::isBothNotANumber<v_f32_4>};

      auto matcher = matchers.begin();
      for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher) {
        const auto testValue = test.at(i);
        auto testVector = v_f32_4(testValue);
        auto actual = vectorization::log(testVector);
        auto expected = v_f32_4(std::log(testValue));
        auto match = (*matcher)(actual, expected);

        auto message = MessageFormat::forFunction(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
        Assert::IsTrue(allTrue(match), message.c_str(), LINE_INFO());
      }
    }

    TEST_METHOD(computesLogEdgeCases) {
      // sse_mathfun log_ps deviates from std::log for these edge cases:
      // log(0) returns -nan instead of -inf, log(inf) returns a large finite value instead of inf.
      auto logZero = vectorization::log(v_f32_4(Zero<v_f32_4::ValueType>()));
      Assert::IsTrue(allTrue(vectorization::isNaN(logZero)), L"log(0) should be NaN in sse_mathfun", LINE_INFO());

      auto logInf = vectorization::log(v_f32_4(Infinity<v_f32_4::ValueType>()));
      Assert::IsTrue(x(logInf) > Zero<v_f32_4::ValueType>(), L"log(inf) should be positive", LINE_INFO());

      // sse_mathfun log_ps does not propagate NaN — it treats NaN as an invalid input
      // similar to negative numbers and produces an implementation-defined result.
    }

    TEST_METHOD(computesExp) {
      const auto test = StandardSample::transcendentalFloatConstants<v_f32_4::ValueType>();

      auto matchers = {Compare::nearEqualsRelative<v_f32_4>, // Half
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // One
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // OneHalf
                       Compare::nearEqualsRelative<v_f32_4>,

                       Compare::nearEqualsRelative<v_f32_4>, // Two
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // Pi
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // ReciprocalPi
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // DegreeToRadian
                       Compare::nearEqualsRelative<v_f32_4>,

                       Compare::nearEqualsRelative<v_f32_4>, // RadianToDegree
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // RadianToUniform
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // Sin45
                       Compare::nearEqualsRelative<v_f32_4>,

                       Compare::nearEqualsRelative<v_f32_4>, // Epsilon
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // SelfOcclusionEpsilon
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // min
                       Compare::nearEqualsRelative<v_f32_4>,
                       Compare::nearEqualsRelative<v_f32_4>, // epsilon
                       Compare::nearEqualsRelative<v_f32_4>};

      auto matcher = matchers.begin();
      for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher) {
        const auto testValue = test.at(i);
        auto actual = vectorization::exp(v_f32_4(testValue));
        auto expected = v_f32_4(std::exp(testValue));
        auto match = (*matcher)(actual, expected);

        auto message = MessageFormat::forFunction(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
        Assert::IsTrue(allTrue(match), message.c_str(), LINE_INFO());
      }
    }

    TEST_METHOD(computesExpEdgeCases) {
      // sse_mathfun exp_ps deviates from std::exp for these edge cases:
      // exp(inf) returns a large finite value instead of inf, exp(NaN) does not return NaN.
      auto expInf = vectorization::exp(v_f32_4(Infinity<v_f32_4::ValueType>()));
      Assert::IsTrue(x(expInf) > Zero<v_f32_4::ValueType>(), L"exp(inf) should be positive", LINE_INFO());

      auto expNegInf = vectorization::exp(v_f32_4(NegativeInfinity<v_f32_4::ValueType>()));
      Assert::IsTrue(
          allTrue(expNegInf == v_f32_4(Zero<v_f32_4::ValueType>())), L"exp(-inf) should be 0", LINE_INFO()
      );
    }

    TEST_METHOD(computesPow) {
      const v_f32_4 base{2.0f, 3.0f, 4.0f, 10.0f};
      const v_f32_4 exponent{3.0f, 2.0f, 0.5f, 1.0f};
      const auto result = pow(base, exponent);
      // 2^3=8, 3^2=9, 4^0.5=2, 10^1=10
      Assert::AreEqual(8.0f, x(result), 0.01f, L"X mismatch", LINE_INFO());
      Assert::AreEqual(9.0f, y(result), 0.01f, L"Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, z(result), 0.01f, L"Z mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, w(result), 0.01f, L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesPowWithScalarExponent) {
      const v_f32_4 base{1.0f, 2.0f, 3.0f, 4.0f};
      const auto result = pow(base, 2.0f);
      // 1^2=1, 2^2=4, 3^2=9, 4^2=16
      Assert::AreEqual(1.0f, x(result), 0.01f, L"X mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, y(result), 0.01f, L"Y mismatch", LINE_INFO());
      Assert::AreEqual(9.0f, z(result), 0.01f, L"Z mismatch", LINE_INFO());
      Assert::AreEqual(16.0f, w(result), 0.01f, L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLog3) {
      const v_f32_4 given{std::exp(1.0f), std::exp(2.0f), std::exp(3.0f), 999.0f};
      const auto result = log3(given);
      // log3 computes log for X,Y,Z and preserves W from input
      Assert::AreEqual(1.0f, x(result), 0.001f, L"X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(result), 0.001f, L"Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, z(result), 0.001f, L"Z mismatch", LINE_INFO());
      Assert::AreEqual(999.0f, w(result), L"W should be preserved from input", LINE_INFO());
    }

    TEST_METHOD(computesExp3) {
      const v_f32_4 given{0.0f, 1.0f, 2.0f, 999.0f};
      const auto result = exp3(given);
      // exp3 computes exp for X,Y,Z and preserves W from input
      Assert::AreEqual(1.0f, x(result), 0.001f, L"X mismatch", LINE_INFO());
      Assert::AreEqual(std::exp(1.0f), y(result), 0.01f, L"Y mismatch", LINE_INFO());
      Assert::AreEqual(std::exp(2.0f), z(result), 0.01f, L"Z mismatch", LINE_INFO());
      Assert::AreEqual(999.0f, w(result), L"W should be preserved from input", LINE_INFO());
    }

    TEST_METHOD(computesPow3) {
      const v_f32_4 base{2.0f, 3.0f, 4.0f, 999.0f};
      const v_f32_4 exponent{3.0f, 2.0f, 0.5f, 999.0f};
      const auto result = pow3(base, exponent);
      // pow3 computes pow for X,Y,Z and preserves W from base
      Assert::AreEqual(8.0f, x(result), 0.01f, L"X mismatch", LINE_INFO());
      Assert::AreEqual(9.0f, y(result), 0.01f, L"Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, z(result), 0.01f, L"Z mismatch", LINE_INFO());
      Assert::AreEqual(999.0f, w(result), L"W should be preserved from base", LINE_INFO());
    }

    TEST_METHOD(computesPow3WithScalarExponent) {
      const v_f32_4 base{1.0f, 2.0f, 3.0f, 999.0f};
      const auto result = pow3(base, 2.0f);
      // pow3 computes pow for X,Y,Z and preserves W from base
      Assert::AreEqual(1.0f, x(result), 0.01f, L"X mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, y(result), 0.01f, L"Y mismatch", LINE_INFO());
      Assert::AreEqual(9.0f, z(result), 0.01f, L"Z mismatch", LINE_INFO());
      Assert::AreEqual(999.0f, w(result), L"W should be preserved from base", LINE_INFO());
    }
  };
}
