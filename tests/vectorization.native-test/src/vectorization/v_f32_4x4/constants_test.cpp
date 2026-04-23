#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4x4_ConstantsTest) {
  public:
    TEST_METHOD(zeroHasAllSoaSlicesZero) {
      const auto actual = Zero<v_f32_4x4>();
      const v_f32_4 expected{0.0f};

      Assert::IsTrue(allTrue(actual.soaX == expected), L"soaX not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == expected), L"soaY not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == expected), L"soaZ not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == expected), L"soaW not zero", LINE_INFO());
    }

    TEST_METHOD(zeroMatchesDefaultConstructedMatrix) {
      const auto zero = Zero<v_f32_4x4>();
      const v_f32_4x4 defaultConstructed{};

      Assert::IsTrue(allTrue(zero.soaX == defaultConstructed.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(zero.soaY == defaultConstructed.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(zero.soaZ == defaultConstructed.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(zero.soaW == defaultConstructed.soaW), L"soaW mismatch", LINE_INFO());
    }
  };
}
