#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_FunctionsTest) {
  public:
    TEST_METHOD(computesAndnot) {
      const auto a = NegativeOne<v_i64_4>();
      const v_i64_4 b{10LL, 20LL, 30LL, 40LL};
      const auto actual = andnot(a, b);
      Assert::IsTrue(allTrue(Zero<v_i64_4>() == actual), L"andnot(allBits, b) should be zero", LINE_INFO());

      const auto actual2 = andnot(Zero<v_i64_4>(), NegativeOne<v_i64_4>());
      Assert::IsTrue(allTrue(NegativeOne<v_i64_4>() == actual2), L"andnot(zero, allBits) should be allBits", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_i64_4{1LL, 0LL, 0LL, 0LL}), L"anyTrue should be true with X nonzero", LINE_INFO());
      Assert::IsTrue(anyTrue(v_i64_4{0LL, 0LL, 0LL, 1LL}), L"anyTrue should be true with W nonzero", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_i64_4>()), L"anyTrue should be false for all zero", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      Assert::IsTrue(allTrue(NegativeOne<v_i64_4>()), L"allTrue should be true for all-bits-set", LINE_INFO());
      Assert::IsFalse(allTrue(v_i64_4{-1LL, 0LL, -1LL, -1LL}), L"allTrue should be false with one zero", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      Assert::IsTrue(anyFalse(v_i64_4{-1LL, 0LL, -1LL, -1LL}), L"anyFalse should be true with one zero", LINE_INFO());
      Assert::IsFalse(anyFalse(NegativeOne<v_i64_4>()), L"anyFalse should be false for all-bits-set", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_i64_4>()), L"allFalse should be true for all zero", LINE_INFO());
      Assert::IsFalse(allFalse(v_i64_4{0LL, 1LL, 0LL, 0LL}), L"allFalse should be false with one nonzero", LINE_INFO());
    }
  };
}
