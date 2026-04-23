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

    TEST_METHOD(computesLength) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      Assert::AreEqual(UInt_32{10}, length(given), L"length mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLength3) {
      const v_ui32_4 given{1u, 2u, 3u, 99u};
      Assert::AreEqual(UInt_32{6}, length3(given), L"length3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLengthV) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      const auto actual = lengthv(given);
      Assert::AreEqual(UInt_32{10}, x(actual), L"lengthv x mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{10}, y(actual), L"lengthv y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{10}, z(actual), L"lengthv z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{10}, w(actual), L"lengthv w mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLength3V) {
      const v_ui32_4 given{1u, 2u, 3u, 99u};
      const auto actual = length3v(given);
      Assert::AreEqual(UInt_32{6}, x(actual), L"length3v x mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{6}, y(actual), L"length3v y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{6}, z(actual), L"length3v z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{6}, w(actual), L"length3v w mismatch", LINE_INFO());
    }

    TEST_METHOD(computesDistance) {
      const v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{1u, 2u, 3u, 4u};
      Assert::AreEqual(UInt_32{90}, distance(a, b), L"distance mismatch", LINE_INFO());
    }

    TEST_METHOD(computesDistance3) {
      const v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{1u, 2u, 3u, 4u};
      Assert::AreEqual(UInt_32{54}, distance3(a, b), L"distance3 mismatch", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue3) {
      Assert::IsTrue(anyTrue3(v_ui32_4{0u, 0u, 1u, 0u}), L"should detect true in Z", LINE_INFO());
      Assert::IsFalse(anyTrue3(v_ui32_4{0u, 0u, 0u, 99u}), L"W should be ignored", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse3) {
      Assert::IsTrue(anyFalse3(v_ui32_4{UInt_32(-1), 0u, UInt_32(-1), UInt_32(-1)}), L"should detect false in Y", LINE_INFO());
      Assert::IsFalse(anyFalse3(v_ui32_4{UInt_32(-1), UInt_32(-1), UInt_32(-1), 0u}), L"W should be ignored", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue3) {
      Assert::IsTrue(allTrue3(v_ui32_4{UInt_32(-1), UInt_32(-1), UInt_32(-1), 0u}), L"W should be ignored", LINE_INFO());
      Assert::IsFalse(allTrue3(v_ui32_4{UInt_32(-1), 0u, UInt_32(-1), UInt_32(-1)}), L"Y is zero", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse3) {
      Assert::IsTrue(allFalse3(v_ui32_4{0u, 0u, 0u, 99u}), L"W should be ignored", LINE_INFO());
      Assert::IsFalse(allFalse3(v_ui32_4{0u, 1u, 0u, 0u}), L"Y is nonzero", LINE_INFO());
    }
  };
}
