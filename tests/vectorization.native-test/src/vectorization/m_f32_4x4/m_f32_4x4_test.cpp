#include "vectorization_tests.h"

namespace vectorization::test {
  auto A = Zero<Float_32>();
  auto B = NegativeOne<Float_32>();
  auto C = One<Float_32>();
  auto D = Two<Float_32>();
  auto E = static_cast<Float_32>(3);
  auto F = static_cast<Float_32>(4);
  auto G = static_cast<Float_32>(5);
  auto H = static_cast<Float_32>(6);

  TEST_CLASS(m_f32_4x4Test) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(ASizeT{4}, m_f32_4x4::ROWS, L"Rows mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{4}, m_f32_4x4::COLUMNS, L"Columns mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{16}, m_f32_4x4::SIZE, L"Size mismatch", LINE_INFO());

      Assert::IsTrue(std::is_same_v<m_f32_4x4, m_f32_4x4::MatrixType>, L"Matrix type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<m_f32_4x4, m_f32_4x4::TransposeType>, L"Transpose type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4, m_f32_4x4::RowVectorType>, L"Row vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4, m_f32_4x4::ColumnVectorType>, L"Column vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<m_f32_4x4, m_f32_4x4::MatrixType>, L"wrong matrix type spec", LINE_INFO());

      Assert::IsTrue(std::is_same_v<v_f32_4::PackedType, m_f32_4x4::PackedType>, L"Packed type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::ValueType, m_f32_4x4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::BoolType, m_f32_4x4::BoolType>, L"Bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsIdentity) {
      auto given = m_f32_4x4{};

      for (auto r = Zero<ASizeT>(); r < m_f32_4x4::ROWS; ++r) {
        for (auto c = Zero<ASizeT>(); c < m_f32_4x4::COLUMNS; ++c) {
          auto expected = r == c ? One<m_f32_4x4::ValueType>() : Zero<m_f32_4x4::ValueType>();
          auto actual = given[r * m_f32_4x4::COLUMNS + c];
          Assert::AreEqual(actual, expected, L"Empty constructor sets wrong initial values", LINE_INFO());
        }
      }
    }

    TEST_METHOD(constructsCopy) {
      const auto given =
          m_f32_4x4{v_f32_4{1, 2, 3, 4}, v_f32_4{5, 6, 7, 8}, v_f32_4{9, 10, 11, 12}, v_f32_4{13, 14, 15, 16}};
      const auto actual = m_f32_4x4{given};

      Assert::IsTrue(allTrue(row<VectorIndices::X>(given) == row<VectorIndices::X>(actual)));
      Assert::IsTrue(allTrue(row<VectorIndices::Y>(given) == row<VectorIndices::Y>(actual)));
      Assert::IsTrue(allTrue(row<VectorIndices::Z>(given) == row<VectorIndices::Z>(actual)));
      Assert::IsTrue(allTrue(row<VectorIndices::W>(given) == row<VectorIndices::W>(actual)));
    }
  };
}
