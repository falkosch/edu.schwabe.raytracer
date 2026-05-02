#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_8_ConstantsTest) {
  public:
    TEST_METHOD(computesZero) {
      const auto actual = Zero<v_i32_8>();
      for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
        Assert::AreEqual(Int_32{0}, actual[i], L"Zero mismatch", LINE_INFO());
    }

    TEST_METHOD(computesOne) {
      const auto actual = One<v_i32_8>();
      for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
        Assert::AreEqual(Int_32{1}, actual[i], L"One mismatch", LINE_INFO());
    }

    TEST_METHOD(computesTwo) {
      const auto actual = Two<v_i32_8>();
      for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
        Assert::AreEqual(Int_32{2}, actual[i], L"Two mismatch", LINE_INFO());
    }

    TEST_METHOD(computesNegativeOne) {
      const auto actual = NegativeOne<v_i32_8>();
      for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
        Assert::AreEqual(Int_32{-1}, actual[i], L"NegativeOne mismatch", LINE_INFO());
    }

    TEST_METHOD(computesNegativeTwo) {
      const auto actual = NegativeTwo<v_i32_8>();
      for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
        Assert::AreEqual(Int_32{-2}, actual[i], L"NegativeTwo mismatch", LINE_INFO());
    }
  };
}
