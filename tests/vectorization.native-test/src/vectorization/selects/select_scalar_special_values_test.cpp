#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(SelectScalarSpecialValuesTest) {
  public:
    // -- Float_32 special values --

    TEST_METHOD(selectPreservesNegativeZeroFloat32) {
      const auto negZero = NegativeZero<Float_32>();
      const auto normal = 1.0f;

      const auto actualTrue = select(true, negZero, normal);
      Assert::AreEqual(0.0f, actualTrue, L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actualTrue), L"-0.0 sign bit lost (true)", LINE_INFO());

      const auto actualFalse = select(false, normal, negZero);
      Assert::AreEqual(0.0f, actualFalse, L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actualFalse), L"-0.0 sign bit lost (false)", LINE_INFO());
    }

    TEST_METHOD(selectPreservesNaNFloat32) {
      const auto nan = NotANumber<Float_32>();
      const auto normal = 1.0f;

      Assert::IsTrue(std::isnan(select(true, nan, normal)), L"NaN not preserved (true)", LINE_INFO());
      Assert::IsTrue(std::isnan(select(false, normal, nan)), L"NaN not preserved (false)", LINE_INFO());
    }

    TEST_METHOD(selectPreservesInfinityFloat32) {
      const auto inf = Infinity<Float_32>();
      const auto negInf = NegativeInfinity<Float_32>();
      const auto normal = 1.0f;

      Assert::AreEqual(inf, select(true, inf, normal), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(negInf, select(true, negInf, normal), L"-inf not preserved", LINE_INFO());
      Assert::AreEqual(inf, select(false, normal, inf), L"+inf not preserved (false)", LINE_INFO());
    }

    TEST_METHOD(selectPreservesNegativeZeroFloat32WithBoolTypeMask) {
      const auto negZero = NegativeZero<Float_32>();
      const auto normal = 1.0f;

      const auto actual = select(BoolTypes<Float_32>::Type{42}, negZero, normal);
      Assert::AreEqual(0.0f, actual, L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actual), L"-0.0 sign bit lost (BoolType)", LINE_INFO());
    }

    // -- Float_64 special values --

    TEST_METHOD(selectPreservesNegativeZeroFloat64) {
      const auto negZero = NegativeZero<Float_64>();
      const auto normal = 1.0;

      const auto actualTrue = select(true, negZero, normal);
      Assert::AreEqual(0.0, actualTrue, L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actualTrue), L"-0.0 sign bit lost (true)", LINE_INFO());

      const auto actualFalse = select(false, normal, negZero);
      Assert::AreEqual(0.0, actualFalse, L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actualFalse), L"-0.0 sign bit lost (false)", LINE_INFO());
    }

    TEST_METHOD(selectPreservesNaNFloat64) {
      const auto nan = NotANumber<Float_64>();
      const auto normal = 1.0;

      Assert::IsTrue(std::isnan(select(true, nan, normal)), L"NaN not preserved (true)", LINE_INFO());
      Assert::IsTrue(std::isnan(select(false, normal, nan)), L"NaN not preserved (false)", LINE_INFO());
    }

    TEST_METHOD(selectPreservesInfinityFloat64) {
      const auto inf = Infinity<Float_64>();
      const auto negInf = NegativeInfinity<Float_64>();
      const auto normal = 1.0;

      Assert::AreEqual(inf, select(true, inf, normal), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(negInf, select(true, negInf, normal), L"-inf not preserved", LINE_INFO());
      Assert::AreEqual(negInf, select(false, normal, negInf), L"-inf not preserved (false)", LINE_INFO());
    }

    TEST_METHOD(selectPreservesNaNFloat64WithBoolTypeMask) {
      const auto nan = NotANumber<Float_64>();
      const auto normal = 1.0;

      const auto actual = select(BoolTypes<Float_64>::Type{42}, nan, normal);
      Assert::IsTrue(std::isnan(actual), L"NaN not preserved (BoolType)", LINE_INFO());
    }
  };
}
