#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(m_f32_4x4_ConstantsTest) {
  public:
    TEST_METHOD(zeroHasAllComponentsZero) {
      const auto actual = Zero<m_f32_4x4>();
      const v_f32_4 expectedRow{0.0f};

      Assert::IsTrue(allTrue(actual.row0 == expectedRow), L"row0 not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == expectedRow), L"row1 not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == expectedRow), L"row2 not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == expectedRow), L"row3 not zero", LINE_INFO());
    }

    TEST_METHOD(identityHasOneOnDiagonalAndZeroElsewhere) {
      const auto actual = Identity<m_f32_4x4>();

      Assert::IsTrue(allTrue(actual.row0 == v_f32_4{1.0f, 0.0f, 0.0f, 0.0f}), L"row0 not {1,0,0,0}", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == v_f32_4{0.0f, 1.0f, 0.0f, 0.0f}), L"row1 not {0,1,0,0}", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == v_f32_4{0.0f, 0.0f, 1.0f, 0.0f}), L"row2 not {0,0,1,0}", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == v_f32_4{0.0f, 0.0f, 0.0f, 1.0f}), L"row3 not {0,0,0,1}", LINE_INFO());
    }

    TEST_METHOD(identityMatchesDefaultConstructedMatrix) {
      const auto defaultConstructed = m_f32_4x4{};
      const auto identity = Identity<m_f32_4x4>();

      Assert::IsTrue(allTrue(defaultConstructed.row0 == identity.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(defaultConstructed.row1 == identity.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(defaultConstructed.row2 == identity.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(defaultConstructed.row3 == identity.row3), L"row3 mismatch", LINE_INFO());
    }
  };
}
