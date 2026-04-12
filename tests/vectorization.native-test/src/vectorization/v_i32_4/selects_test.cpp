#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_SelectsTest) {
  public:
    TEST_METHOD(selectsOneOfTwov_i32_4WithBoolConditionMask) {
      const v_i32_4 givenTruthy{1, 2, 3, 4};
      const v_i32_4 givenFalsy{5, 6, 7, 8};

      {
        const v_i32_4 actual = select(true, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 actual = select(false, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwov_i32_4WithPackedConditionMask) {
      const v_i32_4 givenTruthy{1, 2, 3, 4};
      const v_i32_4 givenFalsy{5, 6, 7, 8};

      {
        const v_i32_4 actual = select(MaskAll<v_i32_4>().components, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 actual = select(Zero<v_i32_4>().components, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsPerLaneWithPackedConditionMask) {
      const v_i32_4 givenTruthy{1, 2, 3, 4};
      const v_i32_4 givenFalsy{5, 6, 7, 8};
      const v_i32_4 expected{1, 6, 7, 8};

      const v_i32_4 actual = select(MaskX<v_i32_4>().components, givenTruthy.components, givenFalsy.components);

      Assert::AreEqual(x(expected), x(actual), L"select value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"select value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"select value mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"select value mismatch", LINE_INFO());
    }
  };
}
