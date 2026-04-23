#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(m_f32_4x4_AccessorsTest) {
  public:
    TEST_METHOD(readsEveryRow) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      Assert::IsTrue(
          allTrue(row<VectorIndices::X>(given) == v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}), L"row<X> mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(row<VectorIndices::Y>(given) == v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}), L"row<Y> mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(row<VectorIndices::Z>(given) == v_f32_4{9.0f, 10.0f, 11.0f, 12.0f}), L"row<Z> mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(row<VectorIndices::W>(given) == v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}), L"row<W> mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(replacesEveryRow) {
      const auto identity = Identity<m_f32_4x4>();
      const v_f32_4 replacement{99.0f, 98.0f, 97.0f, 96.0f};

      {
        const auto actual = replaceRow<VectorIndices::X>(identity, replacement);
        Assert::IsTrue(allTrue(actual.row0 == replacement), L"replaceRow<X> mismatch", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row1 == identity.row1), L"row1 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row2 == identity.row2), L"row2 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row3 == identity.row3), L"row3 unexpectedly changed", LINE_INFO());
      }
      {
        const auto actual = replaceRow<VectorIndices::Y>(identity, replacement);
        Assert::IsTrue(allTrue(actual.row0 == identity.row0), L"row0 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row1 == replacement), L"replaceRow<Y> mismatch", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row2 == identity.row2), L"row2 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row3 == identity.row3), L"row3 unexpectedly changed", LINE_INFO());
      }
      {
        const auto actual = replaceRow<VectorIndices::Z>(identity, replacement);
        Assert::IsTrue(allTrue(actual.row0 == identity.row0), L"row0 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row1 == identity.row1), L"row1 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row2 == replacement), L"replaceRow<Z> mismatch", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row3 == identity.row3), L"row3 unexpectedly changed", LINE_INFO());
      }
      {
        const auto actual = replaceRow<VectorIndices::W>(identity, replacement);
        Assert::IsTrue(allTrue(actual.row0 == identity.row0), L"row0 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row1 == identity.row1), L"row1 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row2 == identity.row2), L"row2 unexpectedly changed", LINE_INFO());
        Assert::IsTrue(allTrue(actual.row3 == replacement), L"replaceRow<W> mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(readsEveryColumn) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      Assert::IsTrue(
          allTrue(column<VectorIndices::X>(given) == v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}), L"column<X> mismatch",
          LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(column<VectorIndices::Y>(given) == v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}), L"column<Y> mismatch",
          LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(column<VectorIndices::Z>(given) == v_f32_4{3.0f, 7.0f, 11.0f, 15.0f}), L"column<Z> mismatch",
          LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(column<VectorIndices::W>(given) == v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}), L"column<W> mismatch",
          LINE_INFO()
      );
    }

    TEST_METHOD(replacesEveryColumn) {
      const auto identity = Identity<m_f32_4x4>();
      const v_f32_4 replacement{99.0f, 98.0f, 97.0f, 96.0f};

      {
        const auto actual = replaceColumn<VectorIndices::X>(identity, replacement);
        Assert::IsTrue(
            allTrue(column<VectorIndices::X>(actual) == replacement), L"replaceColumn<X> mismatch", LINE_INFO()
        );
        Assert::IsTrue(
            allTrue(column<VectorIndices::Y>(actual) == column<VectorIndices::Y>(identity)),
            L"column Y unexpectedly changed", LINE_INFO()
        );
      }
      {
        const auto actual = replaceColumn<VectorIndices::Y>(identity, replacement);
        Assert::IsTrue(
            allTrue(column<VectorIndices::Y>(actual) == replacement), L"replaceColumn<Y> mismatch", LINE_INFO()
        );
        Assert::IsTrue(
            allTrue(column<VectorIndices::X>(actual) == column<VectorIndices::X>(identity)),
            L"column X unexpectedly changed", LINE_INFO()
        );
      }
      {
        const auto actual = replaceColumn<VectorIndices::Z>(identity, replacement);
        Assert::IsTrue(
            allTrue(column<VectorIndices::Z>(actual) == replacement), L"replaceColumn<Z> mismatch", LINE_INFO()
        );
        Assert::IsTrue(
            allTrue(column<VectorIndices::W>(actual) == column<VectorIndices::W>(identity)),
            L"column W unexpectedly changed", LINE_INFO()
        );
      }
      {
        const auto actual = replaceColumn<VectorIndices::W>(identity, replacement);
        Assert::IsTrue(
            allTrue(column<VectorIndices::W>(actual) == replacement), L"replaceColumn<W> mismatch", LINE_INFO()
        );
        Assert::IsTrue(
            allTrue(column<VectorIndices::Z>(actual) == column<VectorIndices::Z>(identity)),
            L"column Z unexpectedly changed", LINE_INFO()
        );
      }
    }
  };
}
