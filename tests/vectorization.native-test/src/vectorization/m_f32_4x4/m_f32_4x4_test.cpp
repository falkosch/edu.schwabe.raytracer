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

    TEST_METHOD(constructsZeroMatrix) {
      const auto m = Zero<m_f32_4x4>();
      Assert::AreEqual(0.0f, x(m.row0), L"row0.x", LINE_INFO());
      Assert::AreEqual(0.0f, y(m.row0), L"row0.y", LINE_INFO());
      Assert::AreEqual(0.0f, x(m.row3), L"row3.x", LINE_INFO());
      Assert::AreEqual(0.0f, w(m.row3), L"row3.w", LINE_INFO());
    }

    TEST_METHOD(readsAndReplacesRows) {
      const auto identity = Identity<m_f32_4x4>();
      // row<X> should be {1,0,0,0}
      const auto r0 = row<VectorIndices::X>(identity);
      Assert::AreEqual(1.0f, x(r0), L"row0.x", LINE_INFO());
      Assert::AreEqual(0.0f, y(r0), L"row0.y", LINE_INFO());
      // row<Y> should be {0,1,0,0}
      const auto r1 = row<VectorIndices::Y>(identity);
      Assert::AreEqual(0.0f, x(r1), L"row1.x", LINE_INFO());
      Assert::AreEqual(1.0f, y(r1), L"row1.y", LINE_INFO());
      // Replace row 0
      const v_f32_4 newRow{5.0f, 6.0f, 7.0f, 8.0f};
      const auto modified = replaceRow<VectorIndices::X>(identity, newRow);
      Assert::AreEqual(5.0f, x(row<VectorIndices::X>(modified)), L"replaced row0.x", LINE_INFO());
      Assert::AreEqual(1.0f, y(row<VectorIndices::Y>(modified)), L"row1 unchanged", LINE_INFO());
    }

    TEST_METHOD(readsAndReplacesColumns) {
      const auto identity = Identity<m_f32_4x4>();
      // column<X> of identity should be {1,0,0,0}
      const auto c0 = column<VectorIndices::X>(identity);
      Assert::AreEqual(1.0f, x(c0), L"col0.row0", LINE_INFO());
      Assert::AreEqual(0.0f, y(c0), L"col0.row1", LINE_INFO());
      // column<Y> should be {0,1,0,0}
      const auto c1 = column<VectorIndices::Y>(identity);
      Assert::AreEqual(0.0f, x(c1), L"col1.row0", LINE_INFO());
      Assert::AreEqual(1.0f, y(c1), L"col1.row1", LINE_INFO());
      // Replace column
      const v_f32_4 newCol{9.0f, 10.0f, 11.0f, 12.0f};
      const auto modified = replaceColumn<VectorIndices::Z>(identity, newCol);
      const auto modCol = column<VectorIndices::Z>(modified);
      Assert::AreEqual(9.0f, x(modCol), L"col2.row0", LINE_INFO());
      Assert::AreEqual(10.0f, y(modCol), L"col2.row1", LINE_INFO());
    }

    TEST_METHOD(transposesMatrix) {
      const m_f32_4x4 m(
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      );
      const auto t = transpose(m);
      // First row of transpose = first column of original
      Assert::IsTrue(allTrue(t.row0 == v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(t.row1 == v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}), L"row1 mismatch", LINE_INFO());
    }

    TEST_METHOD(multipliesMatrixByScalar) {
      const auto identity = Identity<m_f32_4x4>();
      const auto scaled = identity * 2.0f;
      Assert::AreEqual(2.0f, x(scaled.row0), L"diag 2x", LINE_INFO());
      Assert::AreEqual(0.0f, y(scaled.row0), L"off-diag 0", LINE_INFO());
      Assert::AreEqual(2.0f, y(scaled.row1), L"diag 2y", LINE_INFO());
      // commutative
      const auto scaled2 = 2.0f * identity;
      Assert::AreEqual(2.0f, x(scaled2.row0), L"commutative", LINE_INFO());
    }

    TEST_METHOD(addsMatrices) {
      const auto identity = Identity<m_f32_4x4>();
      const auto sum = identity + identity;
      Assert::AreEqual(2.0f, x(sum.row0), L"diag sum", LINE_INFO());
      Assert::AreEqual(0.0f, y(sum.row0), L"off-diag sum", LINE_INFO());
      Assert::AreEqual(2.0f, y(sum.row1), L"diag sum y", LINE_INFO());
    }

    TEST_METHOD(multipliesMatrices) {
      const auto identity = Identity<m_f32_4x4>();
      const m_f32_4x4 m(
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      );
      // M * I = M
      const auto result = m * identity;
      Assert::IsTrue(allTrue(result.row0 == m.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(result.row3 == m.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(transformsVectorByMatrix) {
      const auto identity = Identity<m_f32_4x4>();
      const v_f32_4 v{1.0f, 2.0f, 3.0f, 1.0f};
      // I * v = v
      const auto result = identity * v;
      Assert::IsTrue(allTrue(result == v), L"identity transform mismatch", LINE_INFO());
    }

    TEST_METHOD(inversesIdentity) {
      const auto identity = Identity<m_f32_4x4>();
      const auto inv = inverse(identity);
      Assert::IsTrue(allTrue(inv.row0 == identity.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(inv.row1 == identity.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(inv.row2 == identity.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(inv.row3 == identity.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(inversesScaleMatrix) {
      const auto identity = Identity<m_f32_4x4>();
      const auto scaled = scale(identity, v_f32_4{2.0f, 4.0f, 5.0f, 1.0f});
      const auto inv = inverse(scaled);
      const auto product = scaled * inv;
      Assert::IsTrue(allTrue(product.row0 == identity.row0), L"row0 not identity", LINE_INFO());
      Assert::IsTrue(allTrue(product.row1 == identity.row1), L"row1 not identity", LINE_INFO());
      Assert::IsTrue(allTrue(product.row2 == identity.row2), L"row2 not identity", LINE_INFO());
      Assert::IsTrue(allTrue(product.row3 == identity.row3), L"row3 not identity", LINE_INFO());
    }

    TEST_METHOD(scalesMatrix) {
      const auto identity = Identity<m_f32_4x4>();
      const auto result = scale(identity, v_f32_4{2.0f, 3.0f, 4.0f, 1.0f});
      Assert::AreEqual(2.0f, x(result.row0), L"scale x", LINE_INFO());
      Assert::AreEqual(3.0f, y(result.row1), L"scale y", LINE_INFO());
      Assert::AreEqual(4.0f, z(result.row2), L"scale z", LINE_INFO());
      Assert::AreEqual(1.0f, w(result.row3), L"scale w", LINE_INFO());
    }

    TEST_METHOD(translatesMatrix) {
      const auto identity = Identity<m_f32_4x4>();
      const auto result = translate(identity, v_f32_4{10.0f, 20.0f, 30.0f, 0.0f});
      const v_f32_4 point{0.0f, 0.0f, 0.0f, 1.0f};
      const auto transformed = result * point;
      Assert::AreEqual(10.0f, x(transformed), L"translated x", LINE_INFO());
      Assert::AreEqual(20.0f, y(transformed), L"translated y", LINE_INFO());
      Assert::AreEqual(30.0f, z(transformed), L"translated z", LINE_INFO());
    }

    TEST_METHOD(rotatesMatrixAroundAxis) {
      const auto identity = Identity<m_f32_4x4>();
      // 90 degrees around Z axis
      const auto rotated = rotate(identity, 90.0f, v_f32_4{0.0f, 0.0f, 1.0f, 0.0f});
      // Transform X unit vector: should become Y
      const v_f32_4 xAxis{1.0f, 0.0f, 0.0f, 1.0f};
      const auto result = rotated * xAxis;
      // x -> ~0, y -> ~1, z -> 0
      Assert::AreEqual(0.0f, x(result), 0.001f, L"X should be ~0", LINE_INFO());
      Assert::AreEqual(1.0f, y(result), 0.001f, L"Y should be ~1", LINE_INFO());
      Assert::AreEqual(0.0f, z(result), 0.001f, L"Z should be 0", LINE_INFO());
    }

    TEST_METHOD(constructsOrthoProjection) {
      const auto proj = ortho(-1.0f, 1.0f, 1.0f, -1.0f);
      // Ortho projection maps [-1,1] to [-1,1], so for symmetric bounds the diagonal contains scale factors
      // rRL = 1/(1-(-1)) = 0.5, so row0.x = 2*0.5 = 1.0
      Assert::AreEqual(1.0f, x(proj.row0), 0.001f, L"row0.x should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, y(proj.row0), L"off-diag should be 0", LINE_INFO());
      // rTB = 1/(1-(-1)) = 0.5, so row1.y = 2*0.5 = 1.0
      Assert::AreEqual(1.0f, y(proj.row1), 0.001f, L"row1.y should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, x(proj.row1), L"row1.x should be 0", LINE_INFO());
      // row2.z should be -1
      Assert::AreEqual(-1.0f, z(proj.row2), 0.001f, L"row2.z should be -1", LINE_INFO());
    }

    TEST_METHOD(constructsPerspectiveProjection) {
      const auto proj = perspectiveFov(90.0f, 800.0f, 600.0f, 0.1f, 100.0f);
      // Perspective projection: row0.x and row1.y should be the focal length (non-zero)
      Assert::IsTrue(x(proj.row0) != 0.0f, L"focal x should be non-zero", LINE_INFO());
      Assert::IsTrue(y(proj.row1) != 0.0f, L"focal y should be non-zero", LINE_INFO());
      // row3 = OneZ, so row3.x = 0, row3.z = 1, row3.w = 0
      Assert::AreEqual(0.0f, x(proj.row3), L"row3.x should be 0", LINE_INFO());
      Assert::AreEqual(1.0f, z(proj.row3), L"row3.z should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, w(proj.row3), L"row3.w should be 0", LINE_INFO());
    }

    TEST_METHOD(unprojectsScreenPosition) {
      const auto identity = Identity<m_f32_4x4>();
      // viewport stores (offsetX, offsetY, 1/width, 1/height) per the function's contract
      const v_f32_4 viewport{0.0f, 0.0f, 1.0f / 800.0f, 1.0f / 600.0f};
      // Screen center
      const v_f32_4 screenPos{400.0f, 300.0f, 0.0f, 0.0f};
      const auto result = unproject(screenPos, identity, viewport);
      // With identity inverse VP, the result should be a finite normalized device coordinate
      Assert::IsFalse(std::isinf(x(result)), L"result x should be finite", LINE_INFO());
      Assert::IsFalse(std::isnan(x(result)), L"result x should not be NaN", LINE_INFO());
      Assert::IsFalse(std::isinf(y(result)), L"result y should be finite", LINE_INFO());
      Assert::IsFalse(std::isnan(y(result)), L"result y should not be NaN", LINE_INFO());
    }
  };
}
