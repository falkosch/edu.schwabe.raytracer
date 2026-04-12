#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_SelectsTest) {
  public:
    TEST_METHOD(selectsOneOfTwov_ui64_2WithBoolConditionMask) {
      const v_ui64_2 givenTruthy{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 givenFalsy{UInt_64{5}, UInt_64{6}};

      {
        const v_ui64_2 actual = select(true, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const v_ui64_2 actual = select(false, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwov_ui64_2WithPackedConditionMask) {
      const v_ui64_2 givenTruthy{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 givenFalsy{UInt_64{5}, UInt_64{6}};

      {
        const v_ui64_2 actual = select(MaskAll<v_ui64_2>().components, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const v_ui64_2 actual = select(Zero<v_ui64_2>().components, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }
  };
}
