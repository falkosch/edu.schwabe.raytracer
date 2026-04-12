#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_FunctionsTest) {
  public:
    TEST_METHOD(zerosWComponent) {
      const v_i32_4 given{1, 2, 3, 4};
      const v_i32_4 expected{1, 2, 3, 0};
      const auto actual = zeroW(given);
      Assert::IsTrue(allTrue(expected == actual), L"zeroW value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesAndnot) {
      const auto a = MaskAll<v_i32_4>();
      const auto b = MaskX<v_i32_4>();
      const auto actual = andnot(a, b);
      Assert::IsTrue(allTrue(Zero<v_i32_4>() == actual), L"andnot value mismatch", LINE_INFO());

      const auto actual2 = andnot(Zero<v_i32_4>(), MaskAll<v_i32_4>());
      Assert::IsTrue(allTrue(MaskAll<v_i32_4>() == actual2), L"andnot value mismatch", LINE_INFO());
    }

    TEST_METHOD(detectsNegativeValues) {
      const v_i32_4 given{-1, 0, 1, -2};
      const auto mask = isNegative(given);
      Assert::IsTrue((mask & 1) != 0, L"X should be negative", LINE_INFO());
      Assert::IsTrue((mask & 2) == 0, L"Y should not be negative", LINE_INFO());
      Assert::IsTrue((mask & 4) == 0, L"Z should not be negative", LINE_INFO());
      Assert::IsTrue((mask & 8) != 0, L"W should be negative", LINE_INFO());
    }

    TEST_METHOD(computesAbsoluteValue) {
      const v_i32_4 given{-1, 2, -3, 4};
      const v_i32_4 expected{1, 2, 3, 4};
      const auto actual = abs(given);
      Assert::IsTrue(allTrue(expected == actual), L"abs value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesComponentWiseMin) {
      const v_i32_4 a{1, 5, 3, 8};
      const v_i32_4 b{4, 2, 6, 1};
      const v_i32_4 expected{1, 2, 3, 1};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMin) {
      const v_i32_4 given{3, 1, 4, 2};
      Assert::AreEqual(Int_32{1}, min(given), L"scalar min mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMin3) {
      const v_i32_4 given{3, 1, 4, 0};
      Assert::AreEqual(Int_32{1}, min3(given), L"scalar min3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesComponentWiseMax) {
      const v_i32_4 a{1, 5, 3, 8};
      const v_i32_4 b{4, 2, 6, 1};
      const v_i32_4 expected{4, 5, 6, 8};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMax) {
      const v_i32_4 given{3, 1, 4, 2};
      Assert::AreEqual(Int_32{4}, max(given), L"scalar max mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMax3) {
      const v_i32_4 given{3, 1, 4, 99};
      Assert::AreEqual(Int_32{4}, max3(given), L"scalar max3 mismatch", LINE_INFO());
    }

    TEST_METHOD(clampsValues) {
      const v_i32_4 given{-5, 3, 15, 7};
      const v_i32_4 lo{0, 0, 0, 0};
      const v_i32_4 hi{10, 10, 10, 10};
      const v_i32_4 expected{0, 3, 10, 7};
      const auto actual = clamp(given, lo, hi);
      Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_i32_4{1, 0, 0, 0}), L"anyTrue should be true", LINE_INFO());
      Assert::IsTrue(anyTrue(v_i32_4{0, 0, 0, 1}), L"anyTrue should be true", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_i32_4>()), L"anyTrue should be false", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      Assert::IsTrue(allTrue(v_i32_4{Int_32{-1}}), L"allTrue should be true", LINE_INFO());
      Assert::IsFalse(allTrue(v_i32_4{-1, 0, -1, -1}), L"allTrue should be false", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      Assert::IsTrue(anyFalse(v_i32_4{-1, 0, -1, -1}), L"anyFalse should be true", LINE_INFO());
      Assert::IsFalse(anyFalse(MaskAll<v_i32_4>()), L"anyFalse should be false", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_i32_4>()), L"allFalse should be true", LINE_INFO());
      Assert::IsFalse(allFalse(v_i32_4{0, 1, 0, 0}), L"allFalse should be false", LINE_INFO());
    }
  };
}
