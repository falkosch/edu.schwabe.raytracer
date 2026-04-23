#include "vectorization_tests.h"

#include <array>

namespace vectorization::test {
  TEST_CLASS(m_f32_4x4_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(ASizeT{4}, m_f32_4x4::ROWS, L"Rows mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{4}, m_f32_4x4::COLUMNS, L"Columns mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{16}, m_f32_4x4::SIZE, L"Size mismatch", LINE_INFO());

      Assert::IsTrue(std::is_same_v<m_f32_4x4, m_f32_4x4::MatrixType>, L"Matrix type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<m_f32_4x4, m_f32_4x4::TransposeType>, L"Transpose type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4, m_f32_4x4::RowVectorType>, L"Row vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4, m_f32_4x4::ColumnVectorType>, L"Column vector type mismatch", LINE_INFO());

      Assert::IsTrue(std::is_same_v<v_f32_4::PackedType, m_f32_4x4::PackedType>, L"Packed type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::ValueType, m_f32_4x4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::BoolType, m_f32_4x4::BoolType>, L"Bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsIdentityByDefault) {
      const m_f32_4x4 given{};

      for (auto r = Zero<ASizeT>(); r < m_f32_4x4::ROWS; ++r) {
        for (auto c = Zero<ASizeT>(); c < m_f32_4x4::COLUMNS; ++c) {
          const auto expected = r == c ? One<m_f32_4x4::ValueType>() : Zero<m_f32_4x4::ValueType>();
          const auto actual = given[r * m_f32_4x4::COLUMNS + c];
          Assert::AreEqual(expected, actual, L"Empty constructor sets wrong initial values", LINE_INFO());
        }
      }
    }

    TEST_METHOD(constructsFromFourRowVectors) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      Assert::IsTrue(allTrue(given.row0 == v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row1 == v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row2 == v_f32_4{9.0f, 10.0f, 11.0f, 12.0f}), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row3 == v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromCopy) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const m_f32_4x4 actual{given};

      Assert::IsTrue(allTrue(given.row0 == actual.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row1 == actual.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row2 == actual.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row3 == actual.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromSingleRowVector) {
      const v_f32_4 seed{1.0f, 2.0f, 3.0f, 4.0f};
      const m_f32_4x4 given{seed};

      Assert::IsTrue(allTrue(given.row0 == seed), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row1 == seed), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row2 == seed), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row3 == seed), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromSinglePackedType) {
      const auto seed = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const m_f32_4x4 given{seed};

      const v_f32_4 expected{1.0f, 2.0f, 3.0f, 4.0f};
      Assert::IsTrue(allTrue(given.row0 == expected), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row1 == expected), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row2 == expected), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(given.row3 == expected), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromValueArray) {
      constexpr std::array<m_f32_4x4::ValueType, m_f32_4x4::SIZE> seed{
          1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
      };

      const m_f32_4x4 given{seed};

      for (auto i = Zero<ASizeT>(); i < m_f32_4x4::SIZE; ++i) {
        Assert::AreEqual(seed.at(i), given[i], L"value at index mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(constructsFromValuePointer) {
      constexpr std::array<m_f32_4x4::ValueType, m_f32_4x4::SIZE> seed{
          1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
      };

      const m_f32_4x4 given{seed.data()};

      for (auto i = Zero<ASizeT>(); i < m_f32_4x4::SIZE; ++i) {
        Assert::AreEqual(seed.at(i), given[i], L"value at index mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(readsValueByIntIndex) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      for (int i = 0; i < static_cast<int>(m_f32_4x4::SIZE); ++i) {
        Assert::AreEqual(static_cast<Float_32>(i + 1), given[i], L"int index read mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(writesValueByIndex) {
      m_f32_4x4 given{
          v_f32_4{0.0f}, v_f32_4{0.0f}, v_f32_4{0.0f}, v_f32_4{0.0f}
      };

      for (auto i = Zero<ASizeT>(); i < m_f32_4x4::SIZE; ++i) {
        given[i] = static_cast<Float_32>(i + 1);
      }

      for (auto i = Zero<ASizeT>(); i < m_f32_4x4::SIZE; ++i) {
        Assert::AreEqual(static_cast<Float_32>(i + 1), given[i], L"index write mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(assignsAllRowsFromPackedRow) {
      m_f32_4x4 given{};
      const auto packed = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);

      given = packed;

      const v_f32_4 expected{1.0f, 2.0f, 3.0f, 4.0f};
      Assert::IsTrue(allTrue(given.row0 == expected), L"row0 mismatch after assignment", LINE_INFO());
      Assert::IsTrue(allTrue(given.row1 == expected), L"row1 mismatch after assignment", LINE_INFO());
      Assert::IsTrue(allTrue(given.row2 == expected), L"row2 mismatch after assignment", LINE_INFO());
      Assert::IsTrue(allTrue(given.row3 == expected), L"row3 mismatch after assignment", LINE_INFO());
    }
  };
}
