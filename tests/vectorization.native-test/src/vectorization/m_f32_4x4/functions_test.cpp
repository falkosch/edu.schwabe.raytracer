#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(m_f32_4x4_FunctionsTest) {
  public:
    TEST_METHOD(transposesFullMatrix) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = transpose(given);

      Assert::IsTrue(
          allTrue(actual.row0 == v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}), L"row0 mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.row1 == v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}), L"row1 mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.row2 == v_f32_4{3.0f, 7.0f, 11.0f, 15.0f}), L"row2 mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.row3 == v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}), L"row3 mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(transposeOfTransposeReturnsOriginal) {
      const m_f32_4x4 given{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = transpose(transpose(given));

      Assert::IsTrue(allTrue(actual.row0 == given.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == given.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == given.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == given.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(inverseOfIdentityIsIdentity) {
      const auto identity = Identity<m_f32_4x4>();
      const auto actual = inverse(identity);

      Assert::IsTrue(allTrue(actual.row0 == identity.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == identity.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == identity.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == identity.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(inverseOfScaleMatrixTimesOriginalReturnsIdentity) {
      const auto scaled = scale(Identity<m_f32_4x4>(), v_f32_4{2.0f, 4.0f, 5.0f, 1.0f});
      const auto inv = inverse(scaled);
      const auto actual = scaled * inv;

      const auto identity = Identity<m_f32_4x4>();
      Assert::IsTrue(allTrue(actual.row0 == identity.row0), L"row0 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row1 == identity.row1), L"row1 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row2 == identity.row2), L"row2 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.row3 == identity.row3), L"row3 mismatch", LINE_INFO());
    }

    TEST_METHOD(translateMovesPointByTranslationVector) {
      const auto matrix = translate(Identity<m_f32_4x4>(), v_f32_4{10.0f, 20.0f, 30.0f, 0.0f});
      const v_f32_4 point{0.0f, 0.0f, 0.0f, 1.0f};

      const auto actual = matrix * point;

      Assert::AreEqual(10.0f, x(actual), L"translated x mismatch", LINE_INFO());
      Assert::AreEqual(20.0f, y(actual), L"translated y mismatch", LINE_INFO());
      Assert::AreEqual(30.0f, z(actual), L"translated z mismatch", LINE_INFO());
    }

    TEST_METHOD(scaleMultipliesDiagonalComponents) {
      const auto actual = scale(Identity<m_f32_4x4>(), v_f32_4{2.0f, 3.0f, 4.0f, 1.0f});

      Assert::AreEqual(2.0f, x(actual.row0), L"scale x mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, y(actual.row1), L"scale y mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, z(actual.row2), L"scale z mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, w(actual.row3), L"scale w mismatch", LINE_INFO());
    }

    TEST_METHOD(rotateByNinetyDegreesAroundZAxisMapsXAxisToYAxis) {
      const auto rotated = rotate(Identity<m_f32_4x4>(), 90.0f, v_f32_4{0.0f, 0.0f, 1.0f, 0.0f});
      const v_f32_4 xAxis{1.0f, 0.0f, 0.0f, 1.0f};

      const auto actual = rotated * xAxis;

      Assert::AreEqual(0.0f, x(actual), 0.001f, L"X should be ~0", LINE_INFO());
      Assert::AreEqual(1.0f, y(actual), 0.001f, L"Y should be ~1", LINE_INFO());
      Assert::AreEqual(0.0f, z(actual), 0.001f, L"Z should be 0", LINE_INFO());
    }

    TEST_METHOD(rotateByThreeSixtyDegreesApproximatesIdentity) {
      const auto rotated = rotate(Identity<m_f32_4x4>(), 360.0f, v_f32_4{0.0f, 0.0f, 1.0f, 0.0f});
      const v_f32_4 xAxis{1.0f, 0.0f, 0.0f, 1.0f};

      const auto actual = rotated * xAxis;

      Assert::AreEqual(1.0f, x(actual), 0.001f, L"X should be ~1", LINE_INFO());
      Assert::AreEqual(0.0f, y(actual), 0.001f, L"Y should be ~0", LINE_INFO());
      Assert::AreEqual(0.0f, z(actual), 0.001f, L"Z should be 0", LINE_INFO());
    }

    TEST_METHOD(orthoProjectionOfSymmetricBoundsProducesUnitDiagonalOnRow0AndRow1) {
      const auto actual = ortho(-1.0f, 1.0f, 1.0f, -1.0f);

      Assert::AreEqual(1.0f, x(actual.row0), 0.001f, L"row0.x should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, y(actual.row0), L"row0.y should be 0", LINE_INFO());
      Assert::AreEqual(1.0f, y(actual.row1), 0.001f, L"row1.y should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, x(actual.row1), L"row1.x should be 0", LINE_INFO());
      Assert::AreEqual(-1.0f, z(actual.row2), 0.001f, L"row2.z should be -1", LINE_INFO());
    }

    TEST_METHOD(orthoProjectionOfAsymmetricBoundsScalesByReciprocalOfSpan) {
      // left=-2, right=2 -> span 4, 2/span = 0.5 on row0.x
      // top=3, bottom=-1 -> span 4, 2/span = 0.5 on row1.y
      const auto actual = ortho(-2.0f, 2.0f, 3.0f, -1.0f);

      Assert::AreEqual(0.5f, x(actual.row0), 0.001f, L"row0.x should be 0.5", LINE_INFO());
      Assert::AreEqual(0.5f, y(actual.row1), 0.001f, L"row1.y should be 0.5", LINE_INFO());
    }

    TEST_METHOD(perspectiveProjectionHasNonZeroFocalLengthAndMatchingRow3) {
      const auto actual = perspectiveFov(90.0f, 800.0f, 600.0f, 0.1f, 100.0f);

      Assert::IsTrue(x(actual.row0) != 0.0f, L"focal x should be non-zero", LINE_INFO());
      Assert::IsTrue(y(actual.row1) != 0.0f, L"focal y should be non-zero", LINE_INFO());
      Assert::AreEqual(0.0f, x(actual.row3), L"row3.x should be 0", LINE_INFO());
      Assert::AreEqual(1.0f, z(actual.row3), L"row3.z should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, w(actual.row3), L"row3.w should be 0", LINE_INFO());
    }

    TEST_METHOD(unprojectReturnsFiniteCoordinatesForCenterOfScreen) {
      const auto identity = Identity<m_f32_4x4>();
      const v_f32_4 viewport{0.0f, 0.0f, 1.0f / 800.0f, 1.0f / 600.0f};
      const v_f32_4 screenPos{400.0f, 300.0f, 0.0f, 0.0f};

      const auto actual = unproject(screenPos, identity, viewport);

      Assert::IsFalse(std::isinf(x(actual)), L"x should be finite", LINE_INFO());
      Assert::IsFalse(std::isnan(x(actual)), L"x should not be NaN", LINE_INFO());
      Assert::IsFalse(std::isinf(y(actual)), L"y should be finite", LINE_INFO());
      Assert::IsFalse(std::isnan(y(actual)), L"y should not be NaN", LINE_INFO());
    }
  };
}
