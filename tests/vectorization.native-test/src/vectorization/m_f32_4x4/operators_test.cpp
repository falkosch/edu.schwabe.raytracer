#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(m_f32_4x4_OperatorsTest) {
  public:
    TEST_METHOD(multipliesMatrixByScalarOnTheRight) {
      const auto identity = Identity<m_f32_4x4>();
      const auto actual = identity * 2.0f;

      Assert::IsTrue(allTrue(actual.row0 == v_f32_4{2.0f, 0.0f, 0.0f, 0.0f}), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == v_f32_4{0.0f, 2.0f, 0.0f, 0.0f}), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == v_f32_4{0.0f, 0.0f, 2.0f, 0.0f}), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == v_f32_4{0.0f, 0.0f, 0.0f, 2.0f}), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(multipliesMatrixByScalarOnTheLeft) {
      const auto identity = Identity<m_f32_4x4>();
      const auto actual = 3.0f * identity;

      Assert::IsTrue(allTrue(actual.row0 == v_f32_4{3.0f, 0.0f, 0.0f, 0.0f}), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == v_f32_4{0.0f, 3.0f, 0.0f, 0.0f}), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == v_f32_4{0.0f, 0.0f, 3.0f, 0.0f}), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == v_f32_4{0.0f, 0.0f, 0.0f, 3.0f}), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(addsMatricesComponentWise) {
      const m_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const m_f32_4x4 b{
          v_f32_4{16.0f, 15.0f, 14.0f, 13.0f}, v_f32_4{12.0f, 11.0f, 10.0f, 9.0f}, v_f32_4{8.0f, 7.0f, 6.0f, 5.0f},
          v_f32_4{4.0f, 3.0f, 2.0f, 1.0f}
      };

      const auto actual = a + b;

      const v_f32_4 expectedRow{17.0f, 17.0f, 17.0f, 17.0f};
      Assert::IsTrue(allTrue(actual.row0 == expectedRow), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == expectedRow), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == expectedRow), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == expectedRow), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(matrixTimesIdentityReturnsSameMatrix) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const auto identity = Identity<m_f32_4x4>();

      const auto actual = given * identity;

      Assert::IsTrue(allTrue(actual.row0 == given.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == given.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == given.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == given.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(identityTimesMatrixReturnsSameMatrix) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const auto identity = Identity<m_f32_4x4>();

      const auto actual = identity * given;

      Assert::IsTrue(allTrue(actual.row0 == given.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == given.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == given.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == given.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(multipliesTwoDiagonalMatrices) {
      const auto diag2 = Identity<m_f32_4x4>() * 2.0f;
      const auto diag3 = Identity<m_f32_4x4>() * 3.0f;

      const auto actual = diag2 * diag3;

      Assert::IsTrue(allTrue(actual.row0 == v_f32_4{6.0f, 0.0f, 0.0f, 0.0f}), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == v_f32_4{0.0f, 6.0f, 0.0f, 0.0f}), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == v_f32_4{0.0f, 0.0f, 6.0f, 0.0f}), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == v_f32_4{0.0f, 0.0f, 0.0f, 6.0f}), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(transformsVectorByIdentityMatrix) {
      const auto identity = Identity<m_f32_4x4>();
      const v_f32_4 v{1.0f, 2.0f, 3.0f, 1.0f};

      const auto actual = identity * v;

      Assert::IsTrue(allTrue(actual == v), L"identity transform mismatch", LINE_INFO());
    }

    TEST_METHOD(transformsVectorByDiagonalScalingMatrix) {
      const m_f32_4x4 scaling{
          v_f32_4{2.0f, 0.0f, 0.0f, 0.0f}, v_f32_4{0.0f, 3.0f, 0.0f, 0.0f}, v_f32_4{0.0f, 0.0f, 4.0f, 0.0f},
          v_f32_4{0.0f, 0.0f, 0.0f, 1.0f}
      };
      const v_f32_4 v{1.0f, 1.0f, 1.0f, 1.0f};

      const auto actual = scaling * v;

      Assert::IsTrue(allTrue(actual == v_f32_4{2.0f, 3.0f, 4.0f, 1.0f}), L"diagonal scale mismatch", LINE_INFO());
    }

    TEST_METHOD(vectorTimesIdentityMatrixReturnsSameVector) {
      const auto identity = Identity<m_f32_4x4>();
      const v_f32_4 v{5.0f, 6.0f, 7.0f, 1.0f};

      const auto actual = v * identity;

      Assert::IsTrue(allTrue(actual == v), L"identity transform mismatch", LINE_INFO());
    }

    TEST_METHOD(vectorTimesDiagonalScalingMatrixScalesEachComponent) {
      const m_f32_4x4 scaling{
          v_f32_4{2.0f, 0.0f, 0.0f, 0.0f}, v_f32_4{0.0f, 3.0f, 0.0f, 0.0f}, v_f32_4{0.0f, 0.0f, 4.0f, 0.0f},
          v_f32_4{0.0f, 0.0f, 0.0f, 1.0f}
      };
      const v_f32_4 v{1.0f, 1.0f, 1.0f, 1.0f};

      const auto actual = v * scaling;

      Assert::IsTrue(allTrue(actual == v_f32_4{2.0f, 3.0f, 4.0f, 1.0f}), L"vector-matrix mismatch", LINE_INFO());
    }
  };
}
