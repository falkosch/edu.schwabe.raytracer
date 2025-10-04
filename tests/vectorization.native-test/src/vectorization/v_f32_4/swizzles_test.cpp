#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_SwizzlesTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{x(given), y(given), z(given), w(given)};
        auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{w(given), z(given), y(given), x(given)};
        auto actual = swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{x(given), x(given), x(given), x(given)};
        auto actual = swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), y(given), y(given), y(given)};
        auto actual = swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), z(given), z(given), z(given)};
        auto actual = swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{w(given), w(given), w(given), w(given)};
        auto actual = swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesDuplicateForms) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{x(given), x(given), y(given), y(given)};
        auto actual = swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), x(given), z(given), z(given)};
        auto actual = swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), x(given), y(given)};
        auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), y(given), w(given), w(given)};
        auto actual = swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), z(given), w(given), w(given)};
        auto actual = swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), w(given), z(given), w(given)};
        auto actual = swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfXFirst) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{x(given), x(given), x(given), x(given)};
        auto actual = xxxx(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), x(given), x(given), z(given)};
        auto actual = xxxz(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), x(given), y(given), y(given)};
        auto actual = xxyy(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), x(given), z(given), z(given)};
        auto actual = xxzz(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), x(given), y(given)};
        auto actual = xyxy(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), z(given), z(given), z(given)};
        auto actual = xzzz(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfYFirst) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{y(given), y(given), y(given), y(given)};
        auto actual = yyyy(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), x(given), x(given), y(given)};
        auto actual = yxxy(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), x(given), w(given), z(given)};
        auto actual = yxwz(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), y(given), w(given), w(given)};
        auto actual = yyww(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), z(given), x(given), w(given)};
        auto actual = yzxw(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{y(given), z(given), w(given), x(given)};
        auto actual = yzwx(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfZFirst) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{z(given), z(given), z(given), z(given)};
        auto actual = zzzz(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), x(given), y(given), w(given)};
        auto actual = zxyw(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), z(given), y(given), y(given)};
        auto actual = zzyy(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), z(given), w(given), w(given)};
        auto actual = zzww(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), w(given), x(given), y(given)};
        auto actual = zwxy(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{z(given), w(given), z(given), w(given)};
        auto actual = zwzw(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfWFirst) {
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{w(given), w(given), w(given), w(given)};
        auto actual = wwww(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{w(given), x(given), y(given), z(given)};
        auto actual = wxyz(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{w(given), z(given), y(given), x(given)};
        auto actual = wzyx(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }
  };
}
