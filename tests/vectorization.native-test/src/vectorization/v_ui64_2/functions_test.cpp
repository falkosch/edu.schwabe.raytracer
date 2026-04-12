#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_FunctionsTest) {
  public:
    TEST_METHOD(computesComponentWiseMin) {
      const v_ui64_2 a{UInt_64{1}, UInt_64{5}};
      const v_ui64_2 b{UInt_64{4}, UInt_64{2}};
      const v_ui64_2 expected{UInt_64{1}, UInt_64{2}};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMin) {
      const v_ui64_2 given{UInt_64{3}, UInt_64{1}};
      Assert::AreEqual(UInt_64{1}, min(given), L"scalar min mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMinVector) {
      const v_ui64_2 given{UInt_64{3}, UInt_64{1}};
      const auto actual = minv(given);
      Assert::AreEqual(UInt_64{1}, x(actual), L"minv X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{1}, y(actual), L"minv Y mismatch", LINE_INFO());
    }

    TEST_METHOD(computesComponentWiseMax) {
      const v_ui64_2 a{UInt_64{1}, UInt_64{5}};
      const v_ui64_2 b{UInt_64{4}, UInt_64{2}};
      const v_ui64_2 expected{UInt_64{4}, UInt_64{5}};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMax) {
      const v_ui64_2 given{UInt_64{3}, UInt_64{7}};
      Assert::AreEqual(UInt_64{7}, max(given), L"scalar max mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMaxVector) {
      const v_ui64_2 given{UInt_64{3}, UInt_64{7}};
      const auto actual = maxv(given);
      Assert::AreEqual(UInt_64{7}, x(actual), L"maxv X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{7}, y(actual), L"maxv Y mismatch", LINE_INFO());
    }

    TEST_METHOD(clampsValues) {
      const v_ui64_2 given{UInt_64{0}, UInt_64{15}};
      const v_ui64_2 lo{UInt_64{1}, UInt_64{1}};
      const v_ui64_2 hi{UInt_64{10}, UInt_64{10}};
      const v_ui64_2 expected{UInt_64{1}, UInt_64{10}};
      const auto actual = clamp(given, lo, hi);
      Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_ui64_2{UInt_64{1}, UInt_64{0}}), L"anyTrue should be true", LINE_INFO());
      Assert::IsTrue(anyTrue(v_ui64_2{UInt_64{0}, UInt_64{1}}), L"anyTrue should be true", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_ui64_2>()), L"anyTrue should be false", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      Assert::IsTrue(allTrue(MaskAll<v_ui64_2>()), L"allTrue should be true", LINE_INFO());
      Assert::IsFalse(allTrue(v_ui64_2{MaskAll<v_ui64_2::BoolType>(), UInt_64{0}}), L"allTrue should be false",
                      LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      Assert::IsTrue(anyFalse(v_ui64_2{MaskAll<v_ui64_2::BoolType>(), UInt_64{0}}), L"anyFalse should be true",
                     LINE_INFO());
      Assert::IsFalse(anyFalse(MaskAll<v_ui64_2>()), L"anyFalse should be false", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_ui64_2>()), L"allFalse should be true", LINE_INFO());
      Assert::IsFalse(allFalse(v_ui64_2{UInt_64{0}, UInt_64{1}}), L"allFalse should be false", LINE_INFO());
    }
  };
}
