#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_SpecialValuesTest) {
  public:
    TEST_METHOD(nanPropagatesInAddition) {
      const auto nan = NotANumber<v_f32_8>();
      const auto r = nan + One<v_f32_8>();
      Assert::IsTrue(std::isnan(x1(r)), L"NaN + 1 = NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(x8(r)), L"NaN + 1 = NaN x8", LINE_INFO());
    }

    TEST_METHOD(nanPropagatesInMultiplication) {
      const auto nan = NotANumber<v_f32_8>();
      const auto r = nan * Two<v_f32_8>();
      Assert::IsTrue(std::isnan(x1(r)), L"NaN * 2 = NaN", LINE_INFO());
    }

    TEST_METHOD(infinityPlusOneIsInfinity) {
      const auto inf = Infinity<v_f32_8>();
      const auto r = inf + One<v_f32_8>();
      Assert::AreEqual(std::numeric_limits<float>::infinity(), x1(r), L"inf + 1 = inf", LINE_INFO());
    }

    TEST_METHOD(negativeInfinityTimesNegativeOneIsInfinity) {
      const auto negInf = NegativeInfinity<v_f32_8>();
      const auto r = negInf * NegativeOne<v_f32_8>();
      Assert::AreEqual(std::numeric_limits<float>::infinity(), x1(r), L"-inf * -1 = inf", LINE_INFO());
    }

    TEST_METHOD(zeroTimesInfinityIsNaN) {
      const auto r = Zero<v_f32_8>() * Infinity<v_f32_8>();
      Assert::IsTrue(std::isnan(x1(r)), L"0 * inf = NaN", LINE_INFO());
    }

    TEST_METHOD(infinityMinusInfinityIsNaN) {
      const auto inf = Infinity<v_f32_8>();
      const auto r = inf - inf;
      Assert::IsTrue(std::isnan(x1(r)), L"inf - inf = NaN", LINE_INFO());
    }

    TEST_METHOD(absOfNegativeZeroIsPositiveZero) {
      const auto nz = NegativeZero<v_f32_8>();
      const auto r = abs(nz);
      Assert::AreEqual(0.0f, x1(r), L"abs(-0) = 0", LINE_INFO());
    }

    TEST_METHOD(comparisonWithNaN) {
      const auto nan = NotANumber<v_f32_8>();
      Assert::IsTrue(allFalse(nan == nan), L"NaN != NaN", LINE_INFO());
      Assert::IsTrue(allTrue(nan != nan), L"NaN != NaN is true", LINE_INFO());
    }
  };
}
