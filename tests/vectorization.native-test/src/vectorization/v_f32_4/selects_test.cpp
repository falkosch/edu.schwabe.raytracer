#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_SelectsTest) {
  public:
    TEST_METHOD(selectsOneOfTwov_f32_4WithBoolConditionMask) {
      const v_f32_4 givenTruthy{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 givenFalsy{5.0f, 6.0f, 7.0f, 8.0f};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwov_f32_4Withv_f32_4VectorBoolTypeConditionMask) {
      const v_f32_4 givenTruthy{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 givenFalsy{5.0f, 6.0f, 7.0f, 8.0f};

      {
        const auto actual = select(v_f32_4::VectorBoolType(v_f32_4::BoolType{42}), givenTruthy, givenFalsy);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(v_f32_4::VectorBoolType(v_f32_4::BoolType{0}), givenTruthy, givenFalsy);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwov_f32_4Withv_f32_4ConditionMask) {
      const v_f32_4 givenTruthy{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 givenFalsy{5.0f, 6.0f, 7.0f, 8.0f};

      {
        const auto actual = select(v_f32_4(v_f32_4::ValueType{42.0f}), givenTruthy, givenFalsy);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(v_f32_4(v_f32_4::ValueType{0.0f}), givenTruthy, givenFalsy);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }
  };
}
