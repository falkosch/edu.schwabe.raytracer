#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_4_FunctionsTest) {
  public:
    TEST_METHOD(computesAndnot) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max()};
      const v_ui64_4 b{10ULL, 20ULL, 30ULL, 40ULL};
      const auto actual = andnot(a, b);
      Assert::IsTrue(allTrue(Zero<v_ui64_4>() == actual), L"andnot(allBits, b) should be zero", LINE_INFO());

      const v_ui64_4 allBits{std::numeric_limits<UInt_64>::max()};
      const auto actual2 = andnot(Zero<v_ui64_4>(), allBits);
      Assert::IsTrue(allTrue(allBits == actual2), L"andnot(zero, allBits) should be allBits", LINE_INFO());
    }

    TEST_METHOD(computesMinBasic) {
      const v_ui64_4 a{10ULL, 50ULL, 30ULL, 40ULL};
      const v_ui64_4 b{20ULL, 5ULL, 60ULL, 40ULL};
      const v_ui64_4 expected{10ULL, 5ULL, 30ULL, 40ULL};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMinUnsignedSemantics) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max(), 0ULL, 100ULL, 1ULL};
      const v_ui64_4 b{0ULL, std::numeric_limits<UInt_64>::max(), 50ULL, 2ULL};
      const v_ui64_4 expected{0ULL, 0ULL, 50ULL, 1ULL};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min unsigned semantics mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMaxBasic) {
      const v_ui64_4 a{10ULL, 50ULL, 30ULL, 40ULL};
      const v_ui64_4 b{20ULL, 5ULL, 60ULL, 40ULL};
      const v_ui64_4 expected{20ULL, 50ULL, 60ULL, 40ULL};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMaxUnsignedSemantics) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max(), 0ULL, 100ULL, 1ULL};
      const v_ui64_4 b{0ULL, std::numeric_limits<UInt_64>::max(), 50ULL, 2ULL};
      const v_ui64_4 expected{std::numeric_limits<UInt_64>::max(), std::numeric_limits<UInt_64>::max(), 100ULL, 2ULL};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max unsigned semantics mismatch", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_ui64_4{1ULL, 0ULL, 0ULL, 0ULL}), L"anyTrue should be true with X nonzero", LINE_INFO());
      Assert::IsTrue(anyTrue(v_ui64_4{0ULL, 0ULL, 0ULL, 1ULL}), L"anyTrue should be true with W nonzero", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_ui64_4>()), L"anyTrue should be false for all zero", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      const v_ui64_4 allBits{std::numeric_limits<UInt_64>::max()};
      Assert::IsTrue(allTrue(allBits), L"allTrue should be true for all-bits-set", LINE_INFO());
      const v_ui64_4 withOneZero{std::numeric_limits<UInt_64>::max(), 0ULL, std::numeric_limits<UInt_64>::max(), std::numeric_limits<UInt_64>::max()};
      Assert::IsFalse(allTrue(withOneZero), L"allTrue should be false with one zero", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      const v_ui64_4 withOneZero{std::numeric_limits<UInt_64>::max(), 0ULL, std::numeric_limits<UInt_64>::max(), std::numeric_limits<UInt_64>::max()};
      Assert::IsTrue(anyFalse(withOneZero), L"anyFalse should be true with one zero", LINE_INFO());
      const v_ui64_4 allBits{std::numeric_limits<UInt_64>::max()};
      Assert::IsFalse(anyFalse(allBits), L"anyFalse should be false for all-bits-set", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_ui64_4>()), L"allFalse should be true for all zero", LINE_INFO());
      Assert::IsFalse(allFalse(v_ui64_4{0ULL, 1ULL, 0ULL, 0ULL}), L"allFalse should be false with one nonzero", LINE_INFO());
    }
  };
}
