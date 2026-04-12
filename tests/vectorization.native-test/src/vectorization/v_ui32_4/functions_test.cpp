#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_FunctionsTest) {
  public:
    TEST_METHOD(zerosWComponent) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      const v_ui32_4 expected{1u, 2u, 3u, 0u};
      const auto actual = zeroW(given);
      Assert::IsTrue(allTrue(expected == actual), L"zeroW value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesComponentWiseMin) {
      const v_ui32_4 a{1u, 5u, 3u, 8u};
      const v_ui32_4 b{4u, 2u, 6u, 1u};
      const v_ui32_4 expected{1u, 2u, 3u, 1u};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMin) {
      const v_ui32_4 given{3u, 1u, 4u, 2u};
      Assert::AreEqual(UInt_32{1}, min(given), L"scalar min mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMin3) {
      const v_ui32_4 given{3u, 1u, 4u, 0u};
      Assert::AreEqual(UInt_32{1}, min3(given), L"scalar min3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesComponentWiseMax) {
      const v_ui32_4 a{1u, 5u, 3u, 8u};
      const v_ui32_4 b{4u, 2u, 6u, 1u};
      const v_ui32_4 expected{4u, 5u, 6u, 8u};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMax) {
      const v_ui32_4 given{3u, 1u, 4u, 2u};
      Assert::AreEqual(UInt_32{4}, max(given), L"scalar max mismatch", LINE_INFO());
    }

    TEST_METHOD(computesScalarMax3) {
      const v_ui32_4 given{3u, 1u, 4u, 99u};
      Assert::AreEqual(UInt_32{4}, max3(given), L"scalar max3 mismatch", LINE_INFO());
    }

    TEST_METHOD(clampsValues) {
      const v_ui32_4 given{0u, 3u, 15u, 7u};
      const v_ui32_4 lo{1u, 1u, 1u, 1u};
      const v_ui32_4 hi{10u, 10u, 10u, 10u};
      const v_ui32_4 expected{1u, 3u, 10u, 7u};
      const auto actual = clamp(given, lo, hi);
      Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_ui32_4{1u, 0u, 0u, 0u}), L"anyTrue should be true", LINE_INFO());
      Assert::IsTrue(anyTrue(v_ui32_4{0u, 0u, 0u, 1u}), L"anyTrue should be true", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_ui32_4>()), L"anyTrue should be false", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      Assert::IsTrue(allTrue(MaskAll<v_ui32_4>()), L"allTrue should be true", LINE_INFO());
      Assert::IsFalse(allTrue(v_ui32_4{UInt_32{0xFFFFFFFF}, 0u, UInt_32{0xFFFFFFFF}, UInt_32{0xFFFFFFFF}}),
                      L"allTrue should be false", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      Assert::IsTrue(anyFalse(v_ui32_4{UInt_32{0xFFFFFFFF}, 0u, UInt_32{0xFFFFFFFF}, UInt_32{0xFFFFFFFF}}),
                     L"anyFalse should be true", LINE_INFO());
      Assert::IsFalse(anyFalse(MaskAll<v_ui32_4>()), L"anyFalse should be false", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_ui32_4>()), L"allFalse should be true", LINE_INFO());
      Assert::IsFalse(allFalse(v_ui32_4{0u, 1u, 0u, 0u}), L"allFalse should be false", LINE_INFO());
    }
  };
}
