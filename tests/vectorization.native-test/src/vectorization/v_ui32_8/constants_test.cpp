#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_ConstantsTest) {
  public:
    TEST_METHOD(computesZero) {
      const auto actual = Zero<v_ui32_8>();
      for (ASizeT i = 0; i < v_ui32_8::SIZE; ++i)
        Assert::AreEqual(UInt_32{0}, actual[i], L"Zero mismatch", LINE_INFO());
    }

    TEST_METHOD(computesOne) {
      const auto actual = One<v_ui32_8>();
      for (ASizeT i = 0; i < v_ui32_8::SIZE; ++i)
        Assert::AreEqual(UInt_32{1}, actual[i], L"One mismatch", LINE_INFO());
    }

    TEST_METHOD(computesTwo) {
      const auto actual = Two<v_ui32_8>();
      for (ASizeT i = 0; i < v_ui32_8::SIZE; ++i)
        Assert::AreEqual(UInt_32{2}, actual[i], L"Two mismatch", LINE_INFO());
    }
  };
}
